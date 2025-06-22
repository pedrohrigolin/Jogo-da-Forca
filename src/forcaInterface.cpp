#include "forcaInterface.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <nlohmann/json.hpp>

#include <exception>
#include <typeinfo>

#if defined(__GNUC__) || defined(__clang__)

    #include <cxxabi.h>
    #include <memory>

#endif

#include "forcaFiles.h"
#include "forcaEncrypt.h"
#include "forcaRegex.h"
#include "forcaStrings.h"
#include "forcaUtils.h"


// =================================================================================================
// --- DEFINIÇÃO DE MEMBROS ESTÁTICOS ---
// =================================================================================================

ForcaInterfaceGPU::gGPUHandler ForcaInterfaceGPU::GPUHandler;


// =================================================================================================
// --- IMPLEMENTAÇÃO DAS CLASSES ---
// =================================================================================================

// Registra uma função no mapa, associando um nome (string) a uma função C++.
void ApiRouter::RegisterFunction(const std::string& name, FuncType func) {

    function_map_[name] = func;

}

// Procura uma função pelo nome no mapa e a executa com os argumentos fornecidos.
bool ApiRouter::HandleCall(const std::string& name, CefRefPtr<CefListValue> args) {

    auto it = function_map_.find(name);

    if (it == function_map_.end()) {

        std::cout << "[ApiRouter] Erro: Funcao '" << name << "' nao foi registrada no C++." << std::endl;

        return false;

    }

    // Chama a função C++ (lambda) que foi armazenada no mapa.
    it->second(args);

    return true;

}

ApiBridgeHandler::ApiBridgeHandler() {}

bool ApiBridgeHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) {
    
    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("ApiBridgeMsg");

    CefRefPtr<CefListValue> args = msg->GetArgumentList();

    args->SetString(0, name);

    for (size_t i = 0; i < arguments.size(); ++i) {

        if (arguments[i]->IsString()) args->SetString(i + 1, arguments[i]->GetStringValue());
        else if (arguments[i]->IsInt()) args->SetInt(i + 1, arguments[i]->GetIntValue());

    }

    CefV8Context::GetCurrentContext()->GetFrame()->SendProcessMessage(PID_BROWSER, msg);

    return true;

}

// Implementação do Roteador Nativo
void NativeApiRouter::RegisterFunction(const std::string& name, FuncType func) {
    function_map_[name] = func;
}

bool NativeApiRouter::HandleCall(const CefString& name, const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) {
    auto it = function_map_.find(name);
    if (it == function_map_.end()) return false;
    return it->second(args, retval, exception);
}

ForcaCefClient::ForcaCefClient() {

    router_ = std::make_unique<ApiRouter>();

    // O registro de funções agora é feito de forma limpa no construtor.
    RegisterApiFunctions();

}

// Desabilita o menu de contexto
void ForcaCefClient::OnBeforeContextMenu(CefRefPtr<CefBrowser> b, CefRefPtr<CefFrame> f, CefRefPtr<CefContextMenuParams> p, CefRefPtr<CefMenuModel> m) { m->Clear(); }

// Captura a referência do browser quando ele é criado para usá-la nas lambdas.
void ForcaCefClient::OnAfterCreated(CefRefPtr<CefBrowser> browser) {

    CEF_REQUIRE_UI_THREAD();

    if (browser->IsPopup()) {
        return;
    }

    browser_ = browser;

    // --- LÓGICA PARA DEFINIR O ÍCONE ---
    #if defined(OS_WIN)

        // No Windows, pegamos o HWND (handle da janela)
        HWND hwnd = browser->GetHost()->GetWindowHandle();

        if (hwnd) {

            // IMPORTANTE: Mude "caminho\\para\\seu\\icone.ico" para o caminho real do seu ícone!
            // Use barras invertidas duplas (\\) ou barras normais (/)
            HICON hIcon = (HICON)LoadImage(NULL, L"forca_icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
            
            if (hIcon) {

                // Define o ícone grande (usado em ALT+TAB)
                SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

                // Define o ícone pequeno (usado na barra de título e barra de tarefas)
                SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

            }

        }

    #elif defined(OS_LINUX)

        // No Linux, pegamos a janela GTK de nível superior
        CefRefPtr<CefWindow> window = CefWindow::GetForBrowser(browser->GetIdentifier());

        if (window) {

            // GtkWidget* é o "handle" da janela no GTK
            GtkWindow* gtk_window = GTK_WINDOW( window->GetWindowHandle() );

            if (gtk_window) {

                GError* err = nullptr;
                // IMPORTANTE: Mude "caminho/para/seu/icone.png" para o caminho real!

                // TODO: ALTERAR O ICONE NO LINUX
                GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file("forca_icon.ico", &err);

                if (pixbuf) {

                    // O GTK usa uma lista de ícones (mesmo que seja só um)
                    GList* icon_list = NULL;
                    icon_list = g_list_append(icon_list, pixbuf);
                    gtk_window_set_icon_list(gtk_window, icon_list);
                    
                    // Limpa a memória
                    g_list_free(icon_list);
                    g_object_unref(pixbuf);

                }

            }

        }

    #endif

    // Quando o processo principal é finalmente iniciado, já pode remover a flag de falha de gpu, se ela existir
    if( ForcaInterfaceGPU::GPUHandler.getStatus() ) ForcaInterfaceGPU::GPUHandler.deleteTempFlag();

}

void ForcaCefClient::OnBeforeClose(CefRefPtr<CefBrowser> browser) {}

// --- MÉTODO DE DETECÇÃO DE CRASH GPU ---
// Método de detecção de crash
void ForcaCefClient::OnRenderProcessTerminated(
    CefRefPtr<CefBrowser> browser,
    TerminationStatus status,
    int error_code,
    const CefString& error_string
) {

    if ( ( status == TS_PROCESS_CRASHED || status == TS_PROCESS_WAS_KILLED ) && !ForcaInterfaceGPU::GPUHandler.getStatus() ) {
        
        ForcaInterfaceGPU::GPUHandler.createTempFlag();

        ForcaInterfaceGPU::GPUHandler.showGPUError();

        CefQuitMessageLoop();
        CefShutdown();

    } 

}

// Recebe a mensagem do Renderer Process e a despacha para o nosso ApiRouter.
// Este método agora é extremamente simples e não precisa mais mudar.
bool ForcaCefClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> b, CefRefPtr<CefFrame> f, CefProcessId pid, CefRefPtr<CefProcessMessage> msg) {
    
    CEF_REQUIRE_UI_THREAD();

    if (msg->GetName() == "ApiBridgeMsg") {
        
        router_->HandleCall(msg->GetArgumentList()->GetString(0), msg->GetArgumentList());

        return true;

    }

    return false;

}

void ForcaCefClient::ResolvePromise(CefRefPtr<CefFrame> frame, const CefString& promise_id, const CefString& data) {

    if (frame && frame->IsValid()) {

        std::string js_code = "callUserFunc.cefResolve('" + promise_id.ToString() + "', true, '" + data.ToString() + "');";
        
        frame->ExecuteJavaScript(js_code, frame->GetURL(), 0);

    }

}

void ForcaCefClient::RejectPromise(CefRefPtr<CefFrame> frame, const CefString& promise_id, const CefString& error_message) {

    if (frame && frame->IsValid()) {

        std::string js_code = "callUserFunc.cefResolve('" + promise_id.ToString() + "', false, '" + error_message.ToString() + "');";
        
        frame->ExecuteJavaScript(js_code, frame->GetURL(), 0);

    }

}

// Centraliza o registro de todas as funções da nossa API C++.
void ForcaCefClient::RegisterApiFunctions() {

    // Para adicionar uma nova função, basta adicionar uma nova chamada de RegisterFunction aqui.
    router_->RegisterFunction("abrirDevTools", 
        [this](CefRefPtr<CefListValue> args) {
            if (browser_) {
                browser_->GetHost()->ShowDevTools(CefWindowInfo(), new ForcaCefClient(), CefBrowserSettings(), CefPoint());
            }
        }
    );

    router_->RegisterFunction("salvarUsuario", 
        [this](CefRefPtr<CefListValue> args) {
            std::string nome = args->GetString(1);
            int idade = args->GetInt(2);
            std::cout << "[C++] Rota via Mapa: 'salvarUsuario' com Nome: " << nome << ", Idade: " << idade << std::endl;
        }
    );

    router_->RegisterFunction("getAppVersion", 
        [this](CefRefPtr<CefListValue> args) {
            if (!browser_) {
                return;
            }

            // 1. Sua lógica C++ para obter o valor.
            std::string versaoDoApp = "1.0.0-beta"; // Poderia vir de um arquivo, etc.
            std::cout << "[C++] Função 'getAppVersion' chamada. Retornando a versão: " << versaoDoApp << std::endl;

            // 2. Prepara o código JavaScript para chamar a função de callback no lado do JS.
            // É crucial escapar quaisquer caracteres especiais na string de retorno.
            // Para este exemplo simples, aspas são suficientes.
            std::string js_code = "receberVersaoDoApp('" + versaoDoApp + "'); promise.then('ola');";

            // 3. Envia o comando de volta para o frame principal do navegador.
            CefRefPtr<CefFrame> frame = browser_->GetMainFrame();
            frame->ExecuteJavaScript(js_code, frame->GetURL(), 0);
        }
    );

    // --- NOSSA NOVA FUNÇÃO ASSÍNCRONA ---
    router_->RegisterFunction("getUserData",
        [this](CefRefPtr<CefListValue> args) {
            if (!browser_) return;

            // 1. O primeiro argumento (índice 1) será o ID do usuário que queremos buscar.
            if (args->GetSize() < 3 || args->GetType(2) != VTYPE_INT) {

                // Se não recebemos um int como primeiro argumento, algo está errado.
                // Precisamos do ID do usuário e do ID da promise.
                std::cerr << "[C++] Erro: getUserData chamada com argumentos invalidos." << std::endl;

                // Rejeita a promise se possível
                CefString promiseId = args->GetString(1);

                if(!promiseId.empty()) {
                    RejectPromise(browser_->GetMainFrame(), promiseId, "Argumentos invalidos para getUserData");
                }
                return;
            }

            int userId = args->GetInt(2);

            // 2. O ID da promise continua sendo o último argumento.
            CefString promiseId = args->GetString(1);

            std::cout << "[C++] Funcao 'getUserData' chamada para o usuario ID: " << userId 
                      << " (Promise ID: " << promiseId.ToString() << ")" << std::endl;

            CefRefPtr<CefFrame> frame = browser_->GetMainFrame();

            // 3. Simula uma busca no banco de dados.
            if (userId == 123) {

                nlohmann::json userDataJson;

                userDataJson["id"] = 123;

                userDataJson["nome"] = "Pedro Cef";

                userDataJson["nivel"] = 99;

                ResolvePromise(frame, promiseId, userDataJson.dump());

            } else {

                // FALHA: Usuário não encontrado!
                RejectPromise(frame, promiseId, "Usuario com ID " + std::to_string(userId) + " nao encontrado.");

            }
        }
    );

}

// Implementação do Handler Nativo
NativeFunctionHandler::NativeFunctionHandler() {

    router_ = std::make_unique<NativeApiRouter>();

    // --- Registro das suas funções nativas síncronas ---
    router_->RegisterFunction("toUpperCase",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::to_uppercase(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }                

        }

    );

    router_->RegisterFunction("toLowerCase",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
           
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::to_lowercase(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }        

        }

    );    

    router_->RegisterFunction("normalize",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::size_t argsSize = args.size();

                if(argsSize < 1){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }      
                
                std::string string = args[0]->GetStringValue();

                if(string.empty()){
                    retval = CefV8Value::CreateString("");
                    return true;
                }
                
                std::string form = "NFC";

                if(argsSize > 1){

                    if( ! args[1]->IsString() && ! args[1]->IsUndefined() ){
                        exception = "O primeiro parâmetro form deve ser do tipo string!";
                        return true;
                    }

                    if(args[1]->IsString()){

                        std::string temp = forcaStrings::to_uppercase(args[1]->GetStringValue());

                        if( temp != "NFC" && temp != "NFD" && temp != "NFKC" && temp != "NFKD" ){
                            exception = "Uncaught RangeError: The normalization form should be one of NFC, NFD, NFKC, NFKD.";
                            return true;
                        }

                        form = temp;

                    }

                }

                std::string normalize = forcaStrings::normalize(string, form);

                retval = CefV8Value::CreateString(normalize);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }        

        }

    );

    router_->RegisterFunction("removeAcentos",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::removeAcentos(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }        

        }

    );

    router_->RegisterFunction("removeSpaces",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::removeSpaces(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );

    router_->RegisterFunction("trim",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::trim(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );

    router_->RegisterFunction("rtrim",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::rtrim(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );

    router_->RegisterFunction("ltrim",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::ltrim(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );

    router_->RegisterFunction("normalizeWord",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::normalizeWord(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );

    router_->RegisterFunction("normalizeLineBreaks",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                string = forcaStrings::normalizeLineBreaks(string);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );

    router_->RegisterFunction("removeExtraLineBreaks",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                bool normalize = true;

                if(args.size() > 1){

                    if( ! args[1]->IsBool() ){
                        exception = "O parâmetro normalize deve ser do tipo boolean!";
                        return true;
                    }

                    normalize = args[1]->GetBoolValue();

                }                

                string = forcaStrings::removeExtraLineBreaks(string, normalize);

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );

    router_->RegisterFunction("checkAlphaCharacters",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::string string = args[0]->GetStringValue();

                bool isAlpha = forcaStrings::checkAlphaCharacters(string);

                retval = CefV8Value::CreateBool(isAlpha);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );

    router_->RegisterFunction("concat",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                std::size_t i;

                std::string string = args[0]->GetStringValue();

                for(i=1; i<argsSize; i++){

                    if( ! args[i]->IsString() ){
                        exception = "Só é permitido concatenar strings com outras strings.";
                        return true;                    
                    }

                    string += args[i]->GetStringValue();

                }

                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }

    );    

    router_->RegisterFunction("includes",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                if( ! args[1]->IsString() ){
                    exception = "O primeiro parâmetro separator deve ser do tipo String!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                std::string find = args[1]->GetStringValue();

                std::string::size_type pos = string.find(find);

                CefRefPtr<CefV8Value> includes;

                if( pos == std::string::npos ){
                    includes = CefV8Value::CreateBool(false);
                }
                else{
                    includes = CefV8Value::CreateBool(true);
                }

                retval = includes;

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }
    );

    router_->RegisterFunction("firstIndexOf",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                if( ! args[1]->IsString() ){
                    exception = "O primeiro parâmetro separator deve ser do tipo String!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                std::string find = args[1]->GetStringValue();

                std::string::size_type pos = forcaStrings::firstIndexOf(string, find);

                CefRefPtr<CefV8Value> index;

                if( pos == std::string::npos ){
                    index = CefV8Value::CreateInt(-1);
                }
                else{
                    index = CefV8Value::CreateInt(pos);
                }

                retval = index;

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }
    );

    router_->RegisterFunction("lastIndexOf",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                if( ! args[1]->IsString() ){
                    exception = "O primeiro parâmetro separator deve ser do tipo String!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                std::string find = args[1]->GetStringValue();

                std::string::size_type pos = forcaStrings::lastIndexOf(string, find);

                CefRefPtr<CefV8Value> index;

                if( pos == std::string::npos ){
                    index = CefV8Value::CreateInt(-1);
                }
                else{
                    index = CefV8Value::CreateInt(pos);
                }

                retval = index;

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }
    );   
    
    router_->RegisterFunction("startsWith",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                if( ! args[1]->IsString() ){
                    exception = "O primeiro parâmetro separator deve ser do tipo String!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                std::string find = args[1]->GetStringValue();

                std::string::size_type pos = string.find(find);

                CefRefPtr<CefV8Value> result;

                if( pos == std::string::npos || pos != 0 ){
                    result = CefV8Value::CreateBool(false);
                }
                else{
                    result = CefV8Value::CreateBool(true);
                }

                retval = result;

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }
    );     
    
    router_->RegisterFunction("endsWith",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                if( ! args[1]->IsString() ){
                    exception = "O primeiro parâmetro separator deve ser do tipo String!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                std::string find = args[1]->GetStringValue();

                std::string::size_type pos = string.rfind(find);

                CefRefPtr<CefV8Value> result;

                if( pos == std::string::npos || pos != (string.length() - find.length()) ){
                    result = CefV8Value::CreateBool(false);
                }
                else{
                    result = CefV8Value::CreateBool(true);
                }

                retval = result;

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }
    );         

    router_->RegisterFunction("charAt",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }
                
                if( ! args[1]->IsInt() && ! args[1]->IsDouble() && ! args[1]->IsUInt() ){
                    exception = "O segundo parâmetro limit deve ser do tipo inteiro e positivo!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                if(string.empty()){
                    retval = CefV8Value::CreateString("");
                    return true;
                }

                std::size_t length = forcaStrings::Length(string);

                std::string::size_type charAt;

                if( ! args[1]->IsUndefined() ){

                    double value = args[1]->GetDoubleValue();

                    if( value > (length - 1) ){
                        
                        retval = CefV8Value::CreateString("");

                        return true;

                    }
                    else if(value < 0){
                        exception = "O parâmetro index deve ser igual ou maior que 0!";
                        return true;
                    }

                    charAt = value;

                }
                else{
                    charAt = 0;
                }

                std::string charString = forcaStrings::charAt(string, charAt);

                retval = CefV8Value::CreateString(charString);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    
            
        }
    );

    router_->RegisterFunction("repeat",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {
                
                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }
                
                if( ! args[1]->IsInt() && ! args[1]->IsDouble() && ! args[1]->IsUInt() ){
                    exception = "O segundo parâmetro count deve ser do tipo inteiro e positivo!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                if(string.empty()){
                    retval = CefV8Value::CreateString("");
                    return true;
                }    
                
                double value = args[1]->GetDoubleValue();

                if(value < 0){
                    exception = "O segundo parâmetro count deve ser do tipo inteiro e positivo!";
                    return true;
                }

                if(value == 0){
                    retval = CefV8Value::CreateString("");
                    return true;                    
                }

                std::string::size_type count = value;

                std::string repeat = forcaStrings::repeat(string, count);

                retval = CefV8Value::CreateString(repeat);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }                 

        }
    );

    router_->RegisterFunction("slice",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }
                
                if( ! args[1]->IsInt() && ! args[1]->IsDouble() && ! args[1]->IsUInt() ){
                    exception = "O primeiro parâmetro start deve ser do tipo inteiro!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                std::string::size_type stringLength = forcaStrings::Length(string);

                double startValue = args[1]->GetDoubleValue();

                if(startValue < 0){

                    startValue = stringLength + startValue;

                    if(startValue < 0) startValue = 0;

                }

                if(startValue >= stringLength){
                    retval = CefV8Value::CreateString("");
                    return true;
                }

                std::string::size_type endValue = stringLength;

                if( argsSize > 2 ){

                    if( ! args[2]->IsInt() && ! args[2]->IsDouble() && ! args[2]->IsUInt() && ! args[2]->IsUndefined() ){
                        exception = "O segundo parâmetro end deve ser do tipo inteiro!";
                        return true;
                    }

                    if( ! args[2]->IsUndefined() ){

                        double end = args[2]->GetDoubleValue();

                        if(end < 0){

                            end = stringLength + end;

                            if(end < 0) end = 0;

                        }

                        if(end <= startValue){
                            exception = "O segundo parâmetro end, deve ser maior que o primeiro parâmetro start!";
                            return true;
                        }

                        if(end < endValue){
                            endValue = end;
                        }

                    }

                }
                
                std::string substring = forcaStrings::substring( string, startValue, (endValue - startValue) );

                retval = CefV8Value::CreateString(substring);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }
    );

    router_->RegisterFunction("substring",
        [](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }
                
                if( ! args[1]->IsInt() && ! args[1]->IsDouble() && ! args[1]->IsUInt() ){
                    exception = "O primeiro parâmetro start deve ser do tipo inteiro e positivo!";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                std::string::size_type stringLength = forcaStrings::Length(string);

                double startValue = args[1]->GetDoubleValue();

                if(startValue < 0){
                    exception = "O primeiro parâmetro start deve ser maior ou igual a 0!";
                    return true;
                }

                if(startValue >= stringLength){
                    retval = CefV8Value::CreateString("");
                    return true;
                }

                std::string::size_type endValue = stringLength;

                if( argsSize > 2 ){

                    if( ! args[2]->IsInt() && ! args[2]->IsDouble() && ! args[2]->IsUInt() && ! args[2]->IsUndefined() ){
                        exception = "O segundo parâmetro end deve ser do tipo inteiro e positivo!";
                        return true;
                    }

                    if( ! args[2]->IsUndefined() ){

                        double end = args[2]->GetDoubleValue();

                        if(end <= startValue){
                            exception = "O segundo parâmetro end, deve ser maior que o primeiro parâmetro start!";
                            return true;
                        }

                        if(end < endValue){
                            endValue = end;
                        }

                    }

                }

                std::string substring = forcaStrings::substring( string, startValue, (endValue - startValue) );

                retval = CefV8Value::CreateString(substring);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }    

        }
    );

    router_->RegisterFunction("explode",
        [=](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                CefRefPtr<CefV8Value> search = args[1];

                std::string searchValue;

                if( ! search->IsObject() && ! search->IsString() ){
                    exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                    return true;
                }

                bool isRegex = false;
                
                if( search->IsObject() && jsStringObj && jsStringObj->IsFunction() ) {

                    CefV8ValueList argsString;
                    argsString.push_back(search);

                    // Executa String(value)
                    CefRefPtr<CefV8Value> result = jsStringObj->ExecuteFunction(nullptr, argsString);

                    if (result && result->IsString()) {
                    
                        searchValue = result->GetStringValue().ToString();

                        if(searchValue.length() < 3){
                            exception = "Expressão regular mal formada no primeiro parâmetro search!";
                            return true;
                        }

                        if(searchValue[0] != '/' && searchValue[0] != '#'){
                            exception = "Expressão regular mal formada no primeiro parâmetro search!";
                            return true;
                        }

                        std::string::size_type pos = searchValue.find_last_of(searchValue[0]);

                        if( pos != std::string::npos && pos < 2 ){
                            exception = "Expressão regular mal formada no primeiro parâmetro search!";
                            return true;
                        }

                        isRegex = true;

                    }
                    else{
                        exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                        return true;
                    }
                    
                }
                else if( search->IsString() ){
                    searchValue = search->GetStringValue();
                }
                else{
                    exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                    return true;
                }
                
                std::size_t limit = std::numeric_limits<size_t>::max();

                if( argsSize > 2 ){

                    if( ! args[2]->IsInt() && ! args[2]->IsDouble() && ! args[2]->IsUInt() && ! args[2]->IsUndefined() ){
                        exception = "O segundo parâmetro limit deve ser do tipo inteiro e positivo!";
                        return true;
                    }

                    // Como o parâmetro é opcional, o tipo undefined é permitido, já que ele representa que o parâmetro 
                    // não foi passado na função JS, porém, não tem nenhuma ação aqui.
                    if( ! args[2]->IsUndefined() ){

                        // Como o JS tem limites maiores para number, uso o double aqui, ainda que não vai chegar no limite,
                        // é mais correto usar assim.
                        double value = args[2]->GetDoubleValue();
        
                        if(value < 0){
                            exception = "O segundo parâmetro limit deve ser do tipo inteiro e positivo!";
                            return true;
                        }

                        if(limit == 0){

                            CefRefPtr<CefV8Value> array = CefV8Value::CreateArray(1);

                            array->SetValue( 0, CefV8Value::CreateString(string) );

                            retval = array;

                            return true;

                        }
                        else{
                            limit = value;
                        }

                    }

                }
                
                std::vector<std::string> explode;

                if(isRegex){
                    explode = forcaRegex::preg_split(searchValue, string, limit);
                }
                else{
                    forcaStrings::explode(string, searchValue, &explode, limit);
                }

                std::size_t i, explodeSize = explode.size();

                CefRefPtr<CefV8Value> array = CefV8Value::CreateArray(explodeSize);

                for(i=0; i<explodeSize; i++){

                    array->SetValue(i, CefV8Value::CreateString(explode[i]));

                }

                retval = array;

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }
      
        }
    );

    router_->RegisterFunction("implode",
        [=](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {
            
            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                if( ! args[1]->IsString() && ! args[1]->IsUndefined() ){
                    exception = "O primeiro parâmetro separator deve ser do tipo String!";
                    return true;
                }

                std::string separator;

                if( args[1]->IsUndefined() ) {
                    separator = ",";
                }
                else {
                    separator = args[1]->GetStringValue();
                }

                std::vector<std::string> destiny;

                CefRefPtr<CefV8Value> array = args[0];

                std::size_t i, arraySize = array->GetArrayLength();

                for(i=0; i<arraySize; i++){

                    /**
                     * Solução para converter qualquer tipo de variavel JS em string C++
                     * Pega o objeto String JS e executa a função nativa String.toString do JS
                     */
                    if( ! array->GetValue(i)->IsString() ){

                        if (jsStringObj && jsStringObj->IsFunction()) {

                            CefV8ValueList argsString;
                            argsString.push_back(array->GetValue(i));

                            // Executa String(value)
                            CefRefPtr<CefV8Value> result = jsStringObj->ExecuteFunction(nullptr, argsString);

                            if (result && result->IsString()) {
                                destiny.push_back( result->GetStringValue().ToString() );
                            }
                            
                        }

                    }
                    else{

                        destiny.push_back(array->GetValue(i)->GetStringValue());

                    }

                }

                std::string implode = forcaStrings::implode(destiny, separator);

                retval = CefV8Value::CreateString(implode);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }

        }

    );

    router_->RegisterFunction("str_replace",
        [=](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 3){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                CefRefPtr<CefV8Value> search = args[1];

                std::string searchValue;

                bool isRegex = false;

                /* 
                    O porâmetro search pode ser tanto uma string como uma expressão regular RegExp, que é um objeto JS.
                    Com isso essa parte se encarrega de converter esse objeto para string e verificar se ele é uma expressão
                    regular válida. Usa a função nativa ToString para isso, e para verificar a estrutura usa a mesma lógica
                    usada no arquivo forcaRegex na função createPattern.
                */
                if( search->IsObject() ){
                    
                    if ( jsStringObj && jsStringObj->IsFunction() ) {

                        CefV8ValueList argsString;
                        argsString.push_back(search);

                        // Executa String(value)
                        CefRefPtr<CefV8Value> result = jsStringObj->ExecuteFunction(nullptr, argsString);

                        if (result && result->IsString()) {
                        
                            std::string resultString = result->GetStringValue().ToString();

                            std::string::size_type resultLength = resultString.length();

                            if(resultLength < 3){
                                exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                                return true;
                            }

                            if(resultString[0] != '/'){
                                exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                                return true;
                            }

                            std::string::size_type pos = resultString.find_last_of('/');

                            if( pos != std::string::npos && pos < 2 ){
                                exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                                return true;
                            }

                            searchValue = resultString;
                        
                            isRegex = true;

                        }
                        else{
                            exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                            return true;
                        }
                        
                    }
                    else{
                        exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                        return true;
                    }

                }
                else if( search->IsString() ){
                    searchValue = search->GetStringValue();
                }
                else{                
                    exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                    return true;
                }

                if( ! args[2]->IsString() ){
                    exception = "O segundo parâmetro replaceValue deve ser do tipo string!";
                    return true;
                }

                std::string replaceValue = args[2]->GetStringValue();

                std::size_t offset = 0;

                if( argsSize > 3 ){

                    if( ! args[3]->IsInt() && ! args[3]->IsDouble() && ! args[3]->IsUInt() && ! args[3]->IsUndefined() ){
                        exception = "O terceiro parâmetro offset deve ser do tipo inteiro e positivo!";
                        return true;
                    }

                    // Como o parâmetro é opcional, o tipo undefined é permitido, já que ele representa que o parâmetro 
                    // não foi passado na função JS, porém, não tem nenhuma ação aqui.
                    if( ! args[3]->IsUndefined() ){

                        // Como o JS tem limites maiores para number, uso o double aqui, ainda que não vai chegar no limite,
                        // é mais correto usar assim.
                        double value = args[3]->GetDoubleValue();
        
                        if(value < 0){
                            exception = "O terceiro parâmetro offset deve ser do tipo inteiro e positivo!";
                            return true;
                        }

                        if(value >= string.length()){

                            retval = CefV8Value::CreateString(string);

                            return true;

                        }
                        else{
                            offset = value;
                        }

                    }

                }

                // Faz a substituição baseada em regex
                if(isRegex){

                    string = forcaRegex::preg_replace(searchValue, string, replaceValue, offset, 1);
                    
                    retval = CefV8Value::CreateString(string);

                    return true;

                }
                else{   // Faz a substituição baseado em string

                    std::string::size_type searchPos = string.find(searchValue, offset);

                    if(searchPos != std::string::npos){

                        std::string::size_type searchLenght = searchValue.length();

                        string.replace(searchPos, searchLenght, replaceValue);

                    }

                    retval = CefV8Value::CreateString(string);

                    return true;

                }

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }

        }
    );

    router_->RegisterFunction("str_replace_all",
        [=](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 3){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                CefRefPtr<CefV8Value> search = args[1];

                std::string searchValue;

                bool isRegex = false;

                /* 
                    O porâmetro search pode ser tanto uma string como uma expressão regular RegExp, que é um objeto JS.
                    Com isso essa parte se encarrega de converter esse objeto para string e verificar se ele é uma expressão
                    regular válida. Usa a função nativa ToString para isso, e para verificar a estrutura usa a mesma lógica
                    usada no arquivo forcaRegex na função createPattern.
                */
                if( search->IsObject() ){
                    
                    if ( jsStringObj && jsStringObj->IsFunction() ) {

                        CefV8ValueList argsString;
                        argsString.push_back(search);

                        // Executa String(value)
                        CefRefPtr<CefV8Value> result = jsStringObj->ExecuteFunction(nullptr, argsString);

                        if (result && result->IsString()) {
                        
                            std::string resultString = result->GetStringValue().ToString();

                            std::string::size_type resultLength = resultString.length();

                            if(resultLength < 3){
                                exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                                return true;
                            }

                            if(resultString[0] != '/'){
                                exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                                return true;
                            }

                            std::string::size_type pos = resultString.find_last_of('/');

                            if( pos != std::string::npos && pos < 2 ){
                                exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                                return true;
                            }

                            searchValue = resultString;
                        
                            isRegex = true;

                        }
                        else{
                            exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                            return true;
                        }
                        
                    }
                    else{
                        exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                        return true;
                    }

                }
                else if( search->IsString() ){
                    searchValue = search->GetStringValue();
                }
                else{                
                    exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                    return true;
                }

                if( ! args[2]->IsString() ){
                    exception = "O segundo parâmetro replaceValue deve ser do tipo string!";
                    return true;
                }

                std::string replaceValue = args[2]->GetStringValue();

                std::size_t offset = 0;

                if( argsSize > 3 ){

                    if( ! args[3]->IsInt() && ! args[3]->IsDouble() && ! args[3]->IsUInt() && ! args[3]->IsUndefined() ){
                        exception = "O terceiro parâmetro offset deve ser do tipo inteiro e positivo!";
                        return true;
                    }

                    // Como o parâmetro é opcional, o tipo undefined é permitido, já que ele representa que o parâmetro 
                    // não foi passado na função JS, porém, não tem nenhuma ação aqui.
                    if( ! args[3]->IsUndefined() ){

                        // Como o JS tem limites maiores para number, uso o double aqui, ainda que não vai chegar no limite,
                        // é mais correto usar assim.
                        double value = args[3]->GetDoubleValue();
        
                        if(value < 0){
                            exception = "O terceiro parâmetro offset deve ser do tipo inteiro e positivo!";
                            return true;
                        }

                        if(value >= string.length()){

                            retval = CefV8Value::CreateString(string);

                            return true;

                        }
                        else{
                            offset = value;
                        }

                    }

                }

                std::size_t limit = std::numeric_limits<size_t>::max();

                if( argsSize > 4 ){

                    if( ! args[4]->IsInt() && ! args[4]->IsDouble() && ! args[4]->IsUInt() && ! args[4]->IsUndefined() ){
                        exception = "O quarto parâmetro limit deve ser do tipo inteiro e positivo!";
                        return true;
                    }

                    // Como o parâmetro é opcional, o tipo undefined é permitido, já que ele representa que o parâmetro 
                    // não foi passado na função JS, porém, não tem nenhuma ação aqui.
                    if( ! args[4]->IsUndefined() ){

                        // Como o JS tem limites maiores para number, uso o double aqui, ainda que não vai chegar no limite,
                        // é mais correto usar assim.
                        double value = args[4]->GetDoubleValue();
        
                        if(value < 0){
                            exception = "O quarto parâmetro limit deve ser do tipo inteiro e positivo!";
                            return true;
                        }

                        if(limit == 0){

                            retval = CefV8Value::CreateString(string);

                            return true;

                        }
                        else{
                            limit = value;
                        }

                    }

                }

                // Faz a substituição baseada em regex
                if(isRegex){

                    string = forcaRegex::preg_replace(searchValue, string, replaceValue, offset, limit);
                    
                    retval = CefV8Value::CreateString(string);

                    return true;

                }
                else{   // Faz a substituição baseado em string

                    std::string::size_type searchPos = string.find(searchValue, offset);

                    std::size_t count = 0;

                    std::string::size_type replaceLength = replaceValue.length();

                    while(searchPos != std::string::npos && count < limit){

                        std::string::size_type searchLenght = searchValue.length();

                        string.replace(searchPos, searchLenght, replaceValue);

                        searchPos = string.find( searchValue, (searchPos + replaceLength) );

                        count++;

                    }

                    retval = CefV8Value::CreateString(string);

                    return true;

                }

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }

        }
    );

    router_->RegisterFunction("preg_replace",
        [=](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 3){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                CefRefPtr<CefV8Value> search = args[1];

                std::string searchValue;

                if( ! search->IsObject() && ! search->IsString() ){
                    exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                    return true;
                }
                
                if( search->IsObject() && jsStringObj && jsStringObj->IsFunction() ) {

                    CefV8ValueList argsString;
                    argsString.push_back(search);

                    // Executa String(value)
                    CefRefPtr<CefV8Value> result = jsStringObj->ExecuteFunction(nullptr, argsString);

                    if (result && result->IsString()) {
                    
                        searchValue = result->GetStringValue().ToString();

                    }
                    else{
                        exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                        return true;
                    }
                    
                }
                else if( search->IsString() ){
                    searchValue = forcaStrings::trim( search->GetStringValue() );
                }
                else{
                    exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                    return true;
                }
                
                std::string::size_type resultLength = searchValue.length();

                if(resultLength < 3){
                    exception = "Expressão regular mal formada no primeiro parâmetro search!";
                    return true;
                }

                if(searchValue[0] != '/' && searchValue[0] != '#'){
                    exception = "Expressão regular mal formada no primeiro parâmetro search!";
                    return true;
                }

                std::string::size_type pos = searchValue.find_last_of(searchValue[0]);

                if( pos != std::string::npos && pos < 2 ){
                    exception = "Expressão regular mal formada no primeiro parâmetro search!";
                    return true;
                }

                if( ! args[2]->IsString() ){
                    exception = "O segundo parâmetro replaceValue deve ser do tipo string!";
                    return true;
                }

                std::string replaceValue = args[2]->GetStringValue();

                std::size_t offset = 0;

                if( argsSize > 3 ){

                    if( ! args[3]->IsInt() && ! args[3]->IsDouble() && ! args[3]->IsUInt() && ! args[3]->IsUndefined() ){
                        exception = "O terceiro parâmetro offset deve ser do tipo inteiro e positivo!";
                        return true;
                    }

                    // Como o parâmetro é opcional, o tipo undefined é permitido, já que ele representa que o parâmetro 
                    // não foi passado na função JS, porém, não tem nenhuma ação aqui.
                    if( ! args[3]->IsUndefined() ){

                        // Como o JS tem limites maiores para number, uso o double aqui, ainda que não vai chegar no limite,
                        // é mais correto usar assim.
                        double value = args[3]->GetDoubleValue();
        
                        if(value < 0){
                            exception = "O terceiro parâmetro offset deve ser do tipo inteiro e positivo!";
                            return true;
                        }

                        if(value >= string.length()){

                            retval = CefV8Value::CreateString(string);

                            return true;

                        }
                        else{
                            offset = value;
                        }

                    }

                }

                std::size_t limit = std::numeric_limits<size_t>::max();

                if( argsSize > 4 ){

                    if( ! args[4]->IsInt() && ! args[4]->IsDouble() && ! args[4]->IsUInt() && ! args[4]->IsUndefined() ){
                        exception = "O quarto parâmetro limit deve ser do tipo inteiro e positivo!";
                        return true;
                    }

                    // Como o parâmetro é opcional, o tipo undefined é permitido, já que ele representa que o parâmetro 
                    // não foi passado na função JS, porém, não tem nenhuma ação aqui.
                    if( ! args[4]->IsUndefined() ){

                        // Como o JS tem limites maiores para number, uso o double aqui, ainda que não vai chegar no limite,
                        // é mais correto usar assim.
                        double value = args[4]->GetDoubleValue();
        
                        if(value < 0){
                            exception = "O quarto parâmetro limit deve ser do tipo inteiro e positivo!";
                            return true;
                        }

                        if(limit == 0){

                            retval = CefV8Value::CreateString(string);

                            return true;

                        }
                        else{
                            limit = value;
                        }

                    }

                }
                
                string = forcaRegex::preg_replace(searchValue, string, replaceValue, offset, limit);
                
                retval = CefV8Value::CreateString(string);

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }                

        }
    );    

    router_->RegisterFunction("preg_split",
        [=](const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) -> bool {

            try {

                std::size_t argsSize = args.size();

                if(argsSize < 2){
                    exception = "Quantidade insuficiente de parâmetros fornecidos para a função.";
                    return true;
                }

                std::string string = args[0]->GetStringValue();

                CefRefPtr<CefV8Value> search = args[1];

                std::string searchValue;

                if( ! search->IsObject() && ! search->IsString() ){
                    exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                    return true;
                }
                
                if( search->IsObject() && jsStringObj && jsStringObj->IsFunction() ) {

                    CefV8ValueList argsString;
                    argsString.push_back(search);

                    // Executa String(value)
                    CefRefPtr<CefV8Value> result = jsStringObj->ExecuteFunction(nullptr, argsString);

                    if (result && result->IsString()) {
                    
                        searchValue = result->GetStringValue().ToString();

                    }
                    else{
                        exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                        return true;
                    }
                    
                }
                else if( search->IsString() ){
                    searchValue = forcaStrings::trim( search->GetStringValue() );
                }
                else{
                    exception = "O primeiro parâmetro search deve ser do tipo string ou uma expressão regular!";
                    return true;
                }
                
                std::string::size_type resultLength = searchValue.length();

                if(resultLength < 3){
                    exception = "Expressão regular mal formada no primeiro parâmetro search!";
                    return true;
                }

                if(searchValue[0] != '/' && searchValue[0] != '#'){
                    exception = "Expressão regular mal formada no primeiro parâmetro search!";
                    return true;
                }

                std::string::size_type pos = searchValue.find_last_of(searchValue[0]);

                if( pos != std::string::npos && pos < 2 ){
                    exception = "Expressão regular mal formada no primeiro parâmetro search!";
                    return true;
                }

                std::size_t limit = std::numeric_limits<size_t>::max();

                if( argsSize > 2 ){

                    if( ! args[2]->IsInt() && ! args[2]->IsDouble() && ! args[2]->IsUInt() && ! args[2]->IsUndefined() ){
                        exception = "O segundo parâmetro limit deve ser do tipo inteiro e positivo!";
                        return true;
                    }

                    // Como o parâmetro é opcional, o tipo undefined é permitido, já que ele representa que o parâmetro 
                    // não foi passado na função JS, porém, não tem nenhuma ação aqui.
                    if( ! args[2]->IsUndefined() ){

                        // Como o JS tem limites maiores para number, uso o double aqui, ainda que não vai chegar no limite,
                        // é mais correto usar assim.
                        double value = args[2]->GetDoubleValue();
        
                        if(value < 0){
                            exception = "O segundo parâmetro limit deve ser do tipo inteiro e positivo!";
                            return true;
                        }

                        if(limit == 0){

                            CefRefPtr<CefV8Value> array = CefV8Value::CreateArray(1);

                            array->SetValue( 0, CefV8Value::CreateString(string) );

                            retval = array;

                            return true;

                        }
                        else{
                            limit = value;
                        }

                    }

                }
                
                std::vector<std::string> explode = forcaRegex::preg_split(searchValue, string, limit);
                
                std::size_t i, explodeSize = explode.size();

                CefRefPtr<CefV8Value> array = CefV8Value::CreateArray(explodeSize);

                for(i=0; i<explodeSize; i++){

                    array->SetValue(i, CefV8Value::CreateString(explode[i]));

                }

                retval = array;

                return true;

            } catch (const std::exception& e) {

                exception = ForcaInterface::exceptionText(e);

                return true;

            } catch (...) {

                exception = "Erro ao executar função no backend! \n\nTipo de exceção: Desconhecido \n\nMensagem: Exceção desconhecida!\n";

                return true;

            }                

        }
    );        

}

bool NativeFunctionHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) {
    return router_->HandleCall(name, arguments, retval, exception);
}

ForcaWindowDelegate::ForcaWindowDelegate(CefRefPtr<CefBrowserView> browser_view) : browser_view_(browser_view) {}

void ForcaWindowDelegate::OnWindowCreated(CefRefPtr<CefWindow> window) {

    window->SetTitle("Jogo da Forca");
    window->AddChildView(browser_view_);
    window->Show();

    // --- LÓGICA PARA DEFINIR O ÍCONE ---
    // O código é duplicado, pois já existe um desse tipo acima, mas esse aqui garante que o icone utilizado será o correto
    #if defined(OS_WIN)

        // Pega o "handle" nativo da janela que acabou de ser criada
        HWND hwnd = window->GetWindowHandle();

        if (hwnd) {

            // IMPORTANTE: Use o mesmo caminho do ícone que você usou no seu arquivo .rc
            // ou carregue-o do recurso do executável.
            
            // Opção A: Carregar de um arquivo .ico
            HICON hIcon = (HICON)LoadImage(NULL, L"forca_icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);

            // Opção B (Melhor): Carregar o ícone que já está embutido no .exe
            // HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(MAINICON)); 
            // (Assumindo que você nomeou seu ícone como "MAINICON" no arquivo .rc)

            if (hIcon) {

                // Define o ícone grande (usado em ALT+TAB)
                SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

                // Define o ícone pequeno (usado na barra de título e barra de tarefas)
                SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

            }

        }

    #elif defined(OS_LINUX)

        GtkWindow* gtk_window = GTK_WINDOW(window->GetWindowHandle());

        if (gtk_window) {

            // No Linux, o ícone geralmente é definido pelo arquivo .desktop,
            // mas podemos forçar aqui também se necessário.

            // TODO: LEMBRAR DE ALTERAR O ICONE NO LINUX
            gtk_window_set_icon_from_file(gtk_window, "forca_icon.ico", NULL);

        }

    #endif

}

void ForcaWindowDelegate::OnWindowDestroyed(CefRefPtr<CefWindow> window) { browser_view_ = nullptr; CefQuitMessageLoop(); }

bool ForcaWindowDelegate::CanClose(CefRefPtr<CefWindow> window) { return true; }

// Este método é chamado pelo framework para saber o tamanho mínimo da janela.
CefSize ForcaWindowDelegate::GetMinimumSize(CefRefPtr<CefView> view) {

    // Defina aqui a largura e altura mínimas desejadas.
    return CefSize(800, 600); 

}

ForcaCefApp::ForcaCefApp() {}

void ForcaCefApp::OnBeforeChildProcessLaunch(
    CefRefPtr<CefCommandLine> command_line) {
    
    // Monitora especificamente o processo da GPU
    if (command_line->HasSwitch("type") && 
        command_line->GetSwitchValue("type") == "gpu-process" && !ForcaInterfaceGPU::GPUHandler.getStatus() ) {
        
        ForcaInterfaceGPU::GPUHandler.createTempFlag();

        ForcaInterfaceGPU::GPUHandler.showGPUError();

        CefQuitMessageLoop();
        CefShutdown();

    }

}

void ForcaCefApp::OnBeforeCommandLineProcessing (
    const CefString& process_type,
    CefRefPtr<CefCommandLine> command_line) {
    
    // Ativa o modo escuro para todas as UIs do Chromium, pq eu prefiro.
    command_line->AppendSwitch("force-dark-mode");

    command_line->AppendSwitchWithValue("enable-features", "WebUIDarkMode,CSSColorSchemeUARendering");

    // A aceleração por GPU nem sempre é suportada, então se for detectada que não é suportado, esse trecho de código
    // desabilita a aceleração por GPU.
    if (ForcaInterfaceGPU::GPUHandler.getStatus()) {

        command_line->AppendSwitch("disable-gpu");
        command_line->AppendSwitch("disable-gpu-compositing");
        command_line->AppendSwitch("enable-begin-frame-scheduling");

    }
    else{
        
        // 1. Diz ao Chromium para ignorar a lista negra de GPUs.
        // Isso força o CEF a tentar a aceleração por hardware mesmo em hardware
        // conhecido por ser problemático.
        command_line->AppendSwitch("ignore-gpu-blocklist");

        // 2. Tenta desativar o fallback para o renderizador de software.
        // Isso encurrala o Chromium: ele não pode usar a rota segura da CPU
        // e é forçado a usar a GPU, que pode falhar.
        command_line->AppendSwitch("disable-software-rasterizer");

    }

}

// Cria a UI principal quando o CEF está pronto.
void ForcaCefApp::OnContextInitialized() {

    CEF_REQUIRE_UI_THREAD();

    // Carrega o HTML
    std::string url = "file:///" + forcaFiles::utils::root_realpath("files/web/html/game.html");

    // Cria os handlers e a view do navegador
    CefRefPtr<ForcaCefClient> handler = new ForcaCefClient();

    CefBrowserSettings browser_settings;
    
    CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(handler, url, browser_settings, nullptr, nullptr, nullptr);
    
    // Cria a janela nativa
    CefWindow::CreateTopLevelWindow(new ForcaWindowDelegate(browser_view));

}

// Anuncia as funções para o JavaScript no Processo de Renderização.
void ForcaCefApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {

    CefRefPtr<CefV8Value> global = context->GetGlobal();

    CefRefPtr<CefV8Value> ForcaAppObj = CefV8Value::CreateObject(nullptr, nullptr);

    global->SetValue("ForcaApp", ForcaAppObj, V8_PROPERTY_ATTRIBUTE_NONE);
    
    // Handler para funções assincronas e somente chamada
    CefRefPtr<CefV8Handler> asyncBridgeHandler = new ApiBridgeHandler();

    // Handler para funções síncronas
    CefRefPtr<CefV8Handler> nativeSyncHandler = new NativeFunctionHandler();

    // Funções assíncronas ou somente chamada
    
    ForcaAppObj->SetValue("salvarUsuario", CefV8Value::CreateFunction("salvarUsuario", asyncBridgeHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("abrirDevTools", CefV8Value::CreateFunction("abrirDevTools", asyncBridgeHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("getAppVersion", CefV8Value::CreateFunction("getAppVersion", asyncBridgeHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("getUserData", CefV8Value::CreateFunction("getUserData", asyncBridgeHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    // Funções síncronas

    ForcaAppObj->SetValue("toUpperCase", CefV8Value::CreateFunction("toUpperCase", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("toLowerCase", CefV8Value::CreateFunction("toLowerCase", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("removeSpaces", CefV8Value::CreateFunction("removeSpaces", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("normalize", CefV8Value::CreateFunction("normalize", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);
    
    ForcaAppObj->SetValue("removeAcentos", CefV8Value::CreateFunction("removeAcentos", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("trim", CefV8Value::CreateFunction("trim", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("ltrim", CefV8Value::CreateFunction("ltrim", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("rtrim", CefV8Value::CreateFunction("rtrim", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("normalizeWord", CefV8Value::CreateFunction("normalizeWord", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("normalizeLineBreaks", CefV8Value::CreateFunction("normalizeLineBreaks", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("removeExtraLineBreaks", CefV8Value::CreateFunction("removeExtraLineBreaks", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("checkAlphaCharacters", CefV8Value::CreateFunction("checkAlphaCharacters", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("includes", CefV8Value::CreateFunction("includes", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("indexOf", CefV8Value::CreateFunction("indexOf", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("firstIndexOf", CefV8Value::CreateFunction("firstIndexOf", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("lastIndexOf", CefV8Value::CreateFunction("lastIndexOf", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("startsWith", CefV8Value::CreateFunction("startsWith", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("endsWith", CefV8Value::CreateFunction("endsWith", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("charAt", CefV8Value::CreateFunction("charAt", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("repeat", CefV8Value::CreateFunction("repeat", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("slice", CefV8Value::CreateFunction("slice", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("substring", CefV8Value::CreateFunction("substring", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("explode", CefV8Value::CreateFunction("explode", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("implode", CefV8Value::CreateFunction("implode", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("str_replace", CefV8Value::CreateFunction("str_replace", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("str_replace_all", CefV8Value::CreateFunction("str_replace_all", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("preg_replace", CefV8Value::CreateFunction("preg_replace", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

    ForcaAppObj->SetValue("preg_split", CefV8Value::CreateFunction("preg_split", nativeSyncHandler), V8_PROPERTY_ATTRIBUTE_NONE);

}

/**
 * Tenta torna o texto da exceção mais legível, pois é comum que 
 * dependendo do tipo do compilador o texto seja menos legível.
 *
 * @global
 * @param	char*	name	
 * @return	std::string
 */
std::string ForcaInterface::demangle( const char* name ) {

    #if defined(__GNUC__) || defined(__clang__)

        int status = -1;

        std::unique_ptr<char, void(*)(void*)> res{

            abi::__cxa_demangle(name, nullptr, nullptr, &status),
            std::free

        };

        return (status == 0) ? res.get() : name;

    #else

        // MSVC geralmente já retorna nome legível
        return name;

    #endif

}

/**
 * Retorna um texto mais legível com o tipo de exceção e com a mensagem,
 * para retornar para o JavaScript.
 *
 * @global
 * @param	mixed	:exception	
 * @return	std::string
 */
std::string ForcaInterface::exceptionText( const std::exception& e ) {

    std::string tipo = demangle( typeid(e).name() );

    std::string text = "Erro ao executar função no backend! \n\nTipo de exceção: " + tipo + "\n\nMensagem: " + e.what() + "\n";

    return text;

}

bool ForcaInterface::init( int argc, char* argv[] ){

    // Cria os argumentos principais da aplicação de forma específica para cada plataforma
    #if defined(OS_WIN)

        // No Windows, pegamos o HINSTANCE manualmente para o CefMainArgs
        CefMainArgs main_args(GetModuleHandle(NULL));

    #else

        // No Linux, usamos os argumentos da linha de comando
        CefMainArgs main_args(argc, argv);

    #endif
    
    ForcaInterfaceGPU::GPUHandler.checkFlag(argc, argv);

    CefRefPtr<ForcaCefApp> app(new ForcaCefApp());
    
    int exit_code = CefExecuteProcess(main_args, app, nullptr);

    if (exit_code >= 0) {

        if( ForcaInterfaceGPU::GPUHandler.isGPUError( exit_code ) ){

            ForcaInterfaceGPU::GPUHandler.createTempFlag();

            ForcaInterfaceGPU::GPUHandler.showGPUError();

        }

        return exit_code;

    }

    CefSettings settings;
    settings.no_sandbox = true;

    CefInitialize(main_args, settings, app, nullptr);

    CefRunMessageLoop();
    CefShutdown();

    return 0;

}