/*
|=================================================================
|                       INCLUDES DO PROJETO
|=================================================================
|
| BIBLIOTECAS PADRÃO C++
| ----------------------
| <iostream>  - Operações básicas de I/O
| <cstdlib>   - Funções gerais da biblioteca padrão
| <string>    - Manipulação de strings
| <cctype>    - Classificação e transformação de caracteres
| <fstream>   - Operações de arquivo
| <vector>    - Container dinâmico de elementos
|
| BIBLIOTECAS DO PROJETO
| ---------------------
| "forcaUtils.h" - Utilitários gerais do jogo da forca
|                  Contém funções auxiliares como clear_screen()
|                  e outras utilidades compartilhadas
|=================================================================
*/

// Lib para regex. PCRE2 É O MESMO MOTOR REGEX USADO NO PHP.
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

// #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
//     #include <curses.h>
// #else
//     #include <ncurses.h>
//     #include <form.h>
//     #include <menu.h>
//     #include <panel.h>
// #endif

#include <iostream>
#include <cstdlib>
#include <string>
#include <cctype>
#include <fstream>
#include <vector>
#include <map>
#include <stdexcept>
#include "forcaUtils.h"
#include "forcaStrings.h"
#include "forcaFiles.h"
#include "forcaInitialize.h"
#include "forcaCore.h"
#include "forcaRegex.h"

/*
|================================================================
|   FUNÇÕES DE VERIFICAÇÃO DE ARQUIVO, CONTEUDO E INTEGRIDADE
|================================================================
*/




/* 
|==================
|   FUNÇÃO MAIN
|==================
*/

/**
 * @file main.cpp
 * @author Pedro 
 * @brief Aplicação de estudo avançada do CEF para Linux.
 * @version 3.0 - Para CEF v137+
 * @date 09 de Junho de 2025
 *
 * --- CONCEITOS DEMONSTRADOS ---
 * 1.  Arquitetura Multi-Processo e uso do framework "CEF Views".
 * 2.  Carregamento de UI a partir de um arquivo local via Data URL (Base64).
 * 3.  Implementação de um padrão de design "Roteador de Comandos" (Command Router)
 * usando std::map e std::function para uma ponte JS <-> C++ limpa e escalável.
 * 4.  Comunicação bidirecional C++/JS com aparência de API nativa (window.meuApp.funcao()).
 * 5.  Desativação do menu de contexto para uma aparência de aplicação nativa.
 */

// --- Includes Padrão e do C++ Moderno ---
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional> // Essencial para std::function
#include <map>        // Essencial para std::map
#include <memory>     // Essencial para std::unique_ptr

// --- Includes do CEF ---
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_v8.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"


// =================================================================================================
// --- SEÇÃO 1: FUNÇÕES AUXILIARES GLOBAIS ---
// =================================================================================================
std::string ReadFileToString(const std::string& file_path) {
    std::ifstream file_stream(file_path, std::ios::in);
    if (!file_stream.is_open()) {
        std::cerr << "ERRO: Nao foi possivel abrir o arquivo: " << file_path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    return buffer.str();
}

std::string base64_encode(unsigned char const* bytes, unsigned int len) {
    static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string ret; int i=0,j=0; unsigned char a[3],b[4]; while(len--){ a[i++]=*(bytes++); if(i==3){ b[0]=(a[0]&0xfc)>>2; b[1]=((a[0]&0x03)<<4)+((a[1]&0xf0)>>4); b[2]=((a[1]&0x0f)<<2)+((a[2]&0xc0)>>6); b[3]=a[2]&0x3f; for(i=0;i<4;i++)ret+=base64_chars[b[i]]; i=0;}} if(i){for(j=i;j<3;j++)a[j]='\0'; b[0]=(a[0]&0xfc)>>2; b[1]=((a[0]&0x03)<<4)+((a[1]&0xf0)>>4); b[2]=((a[1]&0x0f)<<2)+((a[2]&0xc0)>>6); b[3]=a[2]&0x3f; for(j=0;j<i+1;j++)ret+=base64_chars[b[j]]; while(i++<3)ret+='=';} return ret;
}


// =================================================================================================
// --- SEÇÃO 2: ARQUITETURA DA PONTE JS <-> C++ ---
// =================================================================================================

/**
 * @brief Roteia chamadas de API do JavaScript para as funções C++ correspondentes.
 * Este é o coração da nossa arquitetura "bonita", substituindo o bloco if/else.
 */
class ApiRouter {
public:
    // Define um tipo "FuncType" para nossas funções: recebe os argumentos do CEF e não retorna nada.
    using FuncType = std::function<void(CefRefPtr<CefListValue>)>;

    // Registra uma função no mapa, associando um nome (string) a uma função C++.
    void RegisterFunction(const std::string& name, FuncType func) {
        function_map_[name] = func;
    }

    // Procura uma função pelo nome no mapa e a executa com os argumentos fornecidos.
    bool HandleCall(const std::string& name, CefRefPtr<CefListValue> args) {
        auto it = function_map_.find(name);
        if (it == function_map_.end()) {
            std::cout << "[ApiRouter] Erro: Funcao '" << name << "' nao foi registrada no C++." << std::endl;
            return false;
        }
        // Chama a função C++ (lambda) que foi armazenada no mapa.
        it->second(args);
        return true;
    }

private:
    std::map<std::string, FuncType> function_map_;
};

/**
 * @brief Atua no Processo de Renderização. É um despachante genérico que pega qualquer
 * chamada de uma função exposta, empacota o nome e os argumentos, e envia para o
 * Processo do Navegador via IPC. Ele não precisa de nenhuma lógica específica.
 */
class ApiBridgeHandler : public CefV8Handler {
public:
    ApiBridgeHandler() {}
    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override {
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
private:
    IMPLEMENT_REFCOUNTING(ApiBridgeHandler);
};

// =================================================================================================
// --- SEÇÃO 3: LÓGICA DA APLICAÇÃO (BROWSER PROCESS) ---
// =================================================================================================

/**
 * @brief Gerencia eventos do navegador e da janela. Contém nosso ApiRouter.
 */
class EstudoCefClient : public CefClient, public CefContextMenuHandler, public CefLifeSpanHandler {
public:
    EstudoCefClient() {
        router_ = std::make_unique<ApiRouter>();
        // O registro de funções agora é feito de forma limpa no construtor.
        RegisterApiFunctions();
    }

    // Getters para os handlers
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

    // Desabilita o menu de contexto
    void OnBeforeContextMenu(CefRefPtr<CefBrowser> b, CefRefPtr<CefFrame> f, CefRefPtr<CefContextMenuParams> p, CefRefPtr<CefMenuModel> m) override { m->Clear(); }

    // Captura a referência do browser quando ele é criado para usá-la nas lambdas.
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override { if (!browser->IsPopup()) { browser_ = browser; } }
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {}

    // Recebe a mensagem do Renderer Process e a despacha para o nosso ApiRouter.
    // Este método agora é extremamente simples e não precisa mais mudar.
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> b, CefRefPtr<CefFrame> f, CefProcessId pid, CefRefPtr<CefProcessMessage> msg) override {
        CEF_REQUIRE_UI_THREAD();
        if (msg->GetName() == "ApiBridgeMsg") {
            router_->HandleCall(msg->GetArgumentList()->GetString(0), msg->GetArgumentList());
            return true;
        }
        return false;
    }

private:

    // Função auxiliar para escapar strings para uso em JavaScript.
    std::string EscapeJsString(const std::string& s) {
        std::ostringstream o;
        for (auto c : s) {
            switch (c) {
                case '"': o << "\\\""; break;
                case '\\': o << "\\\\"; break;
                case '\b': o << "\\b"; break;
                case '\f': o << "\\f"; break;
                case '\n': o << "\\n"; break;
                case '\r': o << "\\r"; break;
                case '\t': o << "\\t"; break;
                default:
                    if ('\x00' <= c && c <= '\x1f') {
                        o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                    } else {
                        o << c;
                    }
            }
        }
        return o.str();
    }

    void ResolvePromise(CefRefPtr<CefFrame> frame, const CefString& promise_id, const CefString& data) {
        if (frame && frame->IsValid()) {
            std::string js_code = "callUserFunc.cefResolve('" + promise_id.ToString() + "', true, '" + EscapeJsString(data.ToString()) + "');";
            frame->ExecuteJavaScript(js_code, frame->GetURL(), 0);
        }
    }

    void RejectPromise(CefRefPtr<CefFrame> frame, const CefString& promise_id, const CefString& error_message) {
        if (frame && frame->IsValid()) {
            std::string js_code = "callUserFunc.cefResolve('" + promise_id.ToString() + "', false, '" + EscapeJsString(error_message.ToString()) + "');";
            frame->ExecuteJavaScript(js_code, frame->GetURL(), 0);
        }
    }

    // Centraliza o registro de todas as funções da nossa API C++.
    void RegisterApiFunctions() {
        // Para adicionar uma nova função, basta adicionar uma nova chamada de RegisterFunction aqui.
        router_->RegisterFunction("abrirDevTools", 
            [this](CefRefPtr<CefListValue> args) {
                if (browser_) {
                    browser_->GetHost()->ShowDevTools(CefWindowInfo(), new EstudoCefClient(), CefBrowserSettings(), CefPoint());
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
                if (args->GetSize() < 3 || args->GetType(1) != VTYPE_INT) {
                    // Se não recebemos um int como primeiro argumento, algo está errado.
                    // Precisamos do ID do usuário e do ID da promise.
                    std::cerr << "[C++] Erro: getUserData chamada com argumentos invalidos." << std::endl;
                    // Rejeita a promise se possível
                    CefString promiseId = args->GetString(args->GetSize() - 1);
                    if(!promiseId.empty()) {
                        RejectPromise(browser_->GetMainFrame(), promiseId, "Argumentos invalidos para getUserData");
                    }
                    return;
                }
                int userId = args->GetInt(1);

                // 2. O ID da promise continua sendo o último argumento.
                CefString promiseId = args->GetString(args->GetSize() - 1);

                std::cout << "[C++] Funcao 'getUserData' chamada para o usuario ID: " << userId 
                        << " (Promise ID: " << promiseId.ToString() << ")" << std::endl;

                CefRefPtr<CefFrame> frame = browser_->GetMainFrame();

                // 3. Simula uma busca no banco de dados.
                if (userId == 123) {
                    // SUCESSO: Usuário encontrado! Prepara um JSON como resposta.
                    std::string userDataJson = "{\"id\": 123, \"nome\": \"Pedro Cef\", \"nivel\": 99}";
                    ResolvePromise(frame, promiseId, userDataJson);
                } else {
                    // FALHA: Usuário não encontrado!
                    RejectPromise(frame, promiseId, "Usuario com ID " + std::to_string(userId) + " nao encontrado.");
                }
            }
        );

    }

    std::unique_ptr<ApiRouter> router_;
    CefRefPtr<CefBrowser> browser_;
    IMPLEMENT_REFCOUNTING(EstudoCefClient);
};

/**
 * @brief Delegate da janela para o framework CEF Views.
 */
class EstudoWindowDelegate : public CefWindowDelegate {
public:
    explicit EstudoWindowDelegate(CefRefPtr<CefBrowserView> browser_view) : browser_view_(browser_view) {}
    void OnWindowCreated(CefRefPtr<CefWindow> window) override {
        window->AddChildView(browser_view_);
        window->Show();
    }
    void OnWindowDestroyed(CefRefPtr<CefWindow> window) override { browser_view_ = nullptr; CefQuitMessageLoop(); }
    bool CanClose(CefRefPtr<CefWindow> window) override { return true; }
private:
    CefRefPtr<CefBrowserView> browser_view_;
    IMPLEMENT_REFCOUNTING(EstudoWindowDelegate);
};

// =================================================================================================
// --- SEÇÃO 5: CLASSE PRINCIPAL DA APLICAÇÃO E PONTO DE ENTRADA ---
// =================================================================================================

class EstudoCefApp : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler {
public:
    EstudoCefApp() {}
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

    // Cria a UI principal quando o CEF está pronto.
    void OnContextInitialized() override {
        CEF_REQUIRE_UI_THREAD();
        
        // Carrega o HTML e cria o Data URL
        std::string html_path = "/media/pedro/HD_EXFAT/Programacao/C++/IFSP/Projeto_forca/files/teste.html"; // <-- MUDE ESTE CAMINHO
        std::string html_content = ReadFileToString(html_path);
        if(html_content.empty()) html_content = "<html><body><h1>Erro ao ler arquivo UI.</h1></body></html>";
        std::string b64 = base64_encode(reinterpret_cast<const unsigned char*>(html_content.c_str()), html_content.length());
        std::string url = "data:text/html;base64," + b64;
        
        // Cria os handlers e a view do navegador
        CefRefPtr<EstudoCefClient> handler = new EstudoCefClient();
        CefBrowserSettings browser_settings;
        CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(handler, url, browser_settings, nullptr, nullptr, nullptr);
        
        // Cria a janela nativa
        CefWindow::CreateTopLevelWindow(new EstudoWindowDelegate(browser_view));
    }

    // Anuncia as funções para o JavaScript no Processo de Renderização.
    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override {
        CefRefPtr<CefV8Value> global = context->GetGlobal();
        CefRefPtr<CefV8Value> meuAppObj = CefV8Value::CreateObject(nullptr, nullptr);
        global->SetValue("meuApp", meuAppObj, V8_PROPERTY_ATTRIBUTE_NONE);
        
        CefRefPtr<CefV8Handler> handler = new ApiBridgeHandler();
        
        meuAppObj->SetValue("salvarUsuario", CefV8Value::CreateFunction("salvarUsuario", handler), V8_PROPERTY_ATTRIBUTE_NONE);
        meuAppObj->SetValue("abrirDevTools", CefV8Value::CreateFunction("abrirDevTools", handler), V8_PROPERTY_ATTRIBUTE_NONE);
        meuAppObj->SetValue("getAppVersion", CefV8Value::CreateFunction("getAppVersion", handler), V8_PROPERTY_ATTRIBUTE_NONE);

        // --- ADICIONA A NOVA FUNÇÃO AQUI ---
        meuAppObj->SetValue("getUserData", CefV8Value::CreateFunction("getUserData", handler), V8_PROPERTY_ATTRIBUTE_NONE);

    }
private:
    IMPLEMENT_REFCOUNTING(EstudoCefApp);
};

// --- Ponto de Entrada Principal ---
int main(int argc, char* argv[]) {
    CefMainArgs main_args(argc, argv);
    CefRefPtr<EstudoCefApp> app(new EstudoCefApp());
    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) return exit_code;

    CefSettings settings;
    settings.no_sandbox = true;
    CefInitialize(main_args, settings, app, nullptr);
    CefRunMessageLoop();
    CefShutdown();
    return 0;
}



/*int main(int argc, char* argv[]){

    // forcaInitialize::init();

    // ASCII Art - Título "FORCA"
    std::cout << "\n";
    std::cout << "  ███████╗ ██████╗ ██████╗  ██████╗ ███████╗\n";
    std::cout << "  ██╔════╝██╔═══██╗██╔══██╗██╔════╝ ██╔══██╗\n";
    std::cout << "  █████╗  ██║   ██║██████╔╝██║      ███████║\n";
    std::cout << "  ██╔══╝  ██║   ██║██╔══██╗██║      ██╔══██║\n";
    std::cout << "  ██║     ╚██████╔╝██║  ██║╚██████╗ ██║  ██║\n";
    std::cout << "  ╚═╝      ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝\n";
    std::cout << "\n";
    
    // Subtítulo decorativo
    std::cout << "         ═══════════════════════════════════\n";
    std::cout << "              🎮 JOGO DA FORCA EM C++ 🎮\n";
    std::cout << "         ═══════════════════════════════════\n";
    std::cout << "\n";

        // ASCII Art - Título "FORCA" estilo DOOM
    std::cout << "\n";
    std::cout << "  ████████  ██████  ██████   ██████  ████████\n";
    std::cout << "  ██       ██    ██ ██   ██ ██       ██    ██\n";
    std::cout << "  ██████   ██    ██ ██████  ██       ████████\n";
    std::cout << "  ██       ██    ██ ██   ██ ██       ██    ██\n";
    std::cout << "  ██        ██████  ██   ██  ██████  ██    ██\n";
    std::cout << "\n";
    std::cout << "   ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n";
    std::cout << "  ██                                        ██\n";
    std::cout << "  ██         >>> JOGO DA FORCA <<<          ██\n";
    std::cout << "  ██              [ C++ EDITION ]           ██\n";
    std::cout << "  ██                                        ██\n";
    std::cout << "   ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n";
    std::cout << "\n";

    // ASCII Art - Título "FORCA" (4 linhas)
    std::cout << "\n";
    std::cout << "  ███████  ██████  ██████   ██████  ██████ \n";
    std::cout << "  ██       ██  ██  ██   ██  ██      ██  ██ \n";
    std::cout << "  █████    ██  ██  ██████   ██      ██████ \n";
    std::cout << "  ██       ██████  ██   ██  ██████  ██  ██ \n";
    std::cout << "         ════════ 🎮 JOGO DA FORCA 🎮 ════════\n";
    std::cout << "\n";

    return 0;

    // std::cout<<forcaRegex::normalizePattern("  /[a-zA-Z].+?[0-9]/imu ")<<std::endl;

    // std::cout<<"B"<<forcaStrings::rtrim("  AAAA   ")<<"B"<<std::endl;

    // std::string teste = " AB  C D EFGH IJ K  \n adsa AQUI áááâ üü  da \t adada  dsada \r dsads adadas \v dadagfa fdafa \f dasdsa dsada  ";

    // teste = forcaStrings::normalizeWord(teste);

    // if( forcaStrings::checkAlphaCharacters(teste) ){
    //     forcaUtils::clear_screen();
    //     std::cout<<"Esta correto!"<<std::endl;
    // }
    // else{
    //     forcaUtils::clear_screen();
    //     std::cout<<"Contem caracteres nao permitidos!"<<std::endl;
    // }

    // // std::cout<<teste<<std::endl;

    // std::string filepath = "../files/words/custom/levels/easy.txt";

    // forcaFiles::validate::validateFiles();

    // std::cout<<forcaFiles::utils::isEmpty(filepath)<<std::endl;

    // std::string normalized = normalizePath(filepath);

    // std::cout<<normalized<<std::endl;

    // forcaUtils::clear_screen();

    // std::string content = getContent(filepath);

    // std::cout<<content<<std::endl;

/*    
    // Inicializa o modo curses
    initscr();              // Inicia a tela
    cbreak();               // Desativa o buffer de linha (recebe input imediatamente)
    noecho();               // Não mostra os caracteres digitados
    keypad(stdscr, TRUE);   // Habilita teclas especiais (setas, etc.)

    mvprintw(0, 0, "Iniciando o jogo da forca...\n\n");

    if( forcaInitialize::init() ){

        mvprintw(2, 0, "Inicializacao concluida!\n\n");

        forcaUtils::clear_screen();

    }
    else{
        mvprintw(2, 0, "Erro ao iniciar o jogo da forca!\n\n");
    }

    // Mensagem inicial
    mvprintw(4, 0, "Pressione qualquer tecla para testar o pdcurses...");
    
    refresh();              // Atualiza a tela

    getch();                // Espera por uma tecla

    mvprintw(6, 0, "Funcionando! Pressione outra tecla para testar o pcre2 (regex)...");

    refresh();

    getch();

    const char patternString[] = "hello";
    const char subjectString[] = "hello world";

    std::string patternStringChar = patternString;
    std::string subjectStringChar = subjectString;

    // Regex pattern
    PCRE2_SPTR pattern = reinterpret_cast<PCRE2_SPTR>( patternStringChar.data() );

    // String para buscar
    PCRE2_SPTR subject = reinterpret_cast<PCRE2_SPTR>( subjectStringChar.data() );

    int errornumber;
    PCRE2_SIZE erroroffset;

    // Compilar regex
    pcre2_code *re = pcre2_compile(
        pattern,               // padrão
        PCRE2_ZERO_TERMINATED, // comprimento da string
        0,                     // flags
        &errornumber,          // para código de erro
        &erroroffset,          // posição do erro
        NULL                   // contexto de compilação
    );

    if (re == NULL) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        std::cerr << "Erro ao compilar regex: " << buffer << " na posição " << erroroffset << std::endl;
        return 1;
    }

    // Criar o "match data" necessário para busca
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);

    // Tentar casar
    int rc = pcre2_match(
        re,                   // código regex
        subject,              // string alvo
        PCRE2_ZERO_TERMINATED, // tamanho da string
        0,                    // início da busca
        0,                    // flags
        match_data,           // estrutura pra resultado
        NULL                  // contexto
    );

    mvprintw(8, 0, "Padrao a se buscar: ");
    mvprintw(8, 21, patternString);

    mvprintw(10, 0, "String onde sera realizada a busca: ");
    mvprintw(10, 37, subjectString);

    if (rc >= 0) {

        mvprintw(12, 0, "Resultado: o padrao regex ");
        mvprintw(12, 27, patternString);
        mvprintw(12, 27 + strlen(patternString), "  foi encontrado em  ");
        mvprintw(12, 27 + strlen(patternString) + 22, subjectString);

    } else {

        mvprintw(12, 0, "Resultado: o padrao regex ");
        mvprintw(12, 27, patternString);
        mvprintw(12, 27 + strlen(patternString), "  NAO foi encontrado em  ");
        mvprintw(12, 27 + strlen(patternString) + 26, subjectString);

    }

    // Limpar alocação
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    mvprintw(14, 0, "Aperte uma tecla para sair...");

    refresh();

    getch();

    // Finaliza o modo curses
    endwin(); */
/*
    std::string texto = "Email: lucas123@example.com e jose@test.com";
    
    // Teste 1 — preg_match
    std::string padrao1 = "/\\w+@\\w+\\.com/";
    forcaRegex::RegexResult r1 = forcaRegex::preg_match(padrao1, texto, 0);

    std::cout << "\n[Teste 1: preg_match]\n";
    if (r1.match) {
        for (int key : r1.int_keys) {
            std::vector<forcaRegex::RegexStructData>* grupos = r1.get(key);
            for (const auto& g : *grupos) {
                std::cout << "[" << key << "] " << g.match << " (start: " << g.start << ", end: " << g.end << ")\n";
            }
        }
    } else {
        std::cout << "Nenhum match encontrado.\n";
    }

    // Teste 2 — preg_match_all
    std::string padrao2 = "/\\w+@\\w+\\.com/";
    forcaRegex::RegexResult r2 = forcaRegex::preg_match_all(padrao2, texto, 0);

    std::cout << "\n[Teste 2: preg_match_all]\n";
    if (r2.match) {
        for (int key : r2.int_keys) {
            std::vector<forcaRegex::RegexStructData>* grupos = r2.get(key);
            for (const auto& g : *grupos) {
                std::cout << "[" << key << "] " << g.match << " (start: " << g.start << ", end: " << g.end << ")\n";
            }
        }
    } else {
        std::cout << "Nenhum match encontrado.\n";
    }

    // Teste 3 — grupos nomeados
    std::string texto3 = "Usuário: lucas, ID: 42";
    std::string padrao3 = "/Usuário:\\s*(?<nome>\\w+),\\s*ID:\\s*(?<id>\\d+)/";

    forcaRegex::RegexResult r3 = forcaRegex::preg_match(padrao3, texto3, 0);

    std::cout << "\n[Teste 3: grupos nomeados]\n";
    if (r3.match) {
        for (const std::string& key : r3.string_keys) {
            std::vector<forcaRegex::RegexStructData>* grupos = r3.get(key);
            for (const auto& g : *grupos) {
                std::cout << "$" << key << ": " << g.match << " (start: " << g.start << ", end: " << g.end << ")\n";
            }
        }
    } else {
        std::cout << "Nenhum match encontrado.\n";
    }

    std::cout << "[Teste 1: substituição simples]" << std::endl;
    std::cout << forcaRegex::preg_replace("/foo/", "foo bar", "baz", 0) << std::endl;
    // Esperado: baz bar

    std::cout << "[Teste 2: case-insensitive (i)]" << std::endl;
    std::cout << forcaRegex::preg_replace("/FOO/i", "foo bar", "baz", 0) << std::endl;
    // Esperado: baz bar

    std::cout << "[Teste 3: nenhum casamento]" << std::endl;
    std::cout << forcaRegex::preg_replace("/xyz/", "foo bar", "baz", 0) << std::endl;
    // Esperado: foo bar

    std::cout << "[Teste 4: grupo capturado]" << std::endl;
    std::cout << forcaRegex::preg_replace("/(foo)/", "foo bar", "[$1]", 0) << std::endl;
    // Esperado: [foo] bar

    std::cout << "[Teste 5: múltiplas ocorrências]" << std::endl;
    std::cout << forcaRegex::preg_replace("/a/", "banana", "o", 0) << std::endl;
    // Esperado: bonono

    std::cout << "[Teste 6: offset > 0]" << std::endl;
    std::cout << forcaRegex::preg_replace("/a/", "banana", "o", 5) << std::endl;
    // Esperado: banano  (somente o 'a' no final é substituído)

    std::cout << "[Teste 7: usando \\d]" << std::endl;
    std::cout << forcaRegex::preg_replace("/\\d+/", "User42 has 7 apples", "#", 0) << std::endl;
    // Esperado: User# has # apples

    std::string result;

    // Teste 1
    result = forcaRegex::preg_replace("/(?P<word>\\w+)/", "Hello world", "[$word]", 0);
    std::cout << "Teste 1: " << result << std::endl; // Esperado: [Hello] [world]

    // Teste 2
    result = forcaRegex::preg_replace("/(?P<first>\\w+)\\s(?P<second>\\w+)/", "alpha beta", "$second $first", 0);
    std::cout << "Teste 2: " << result << std::endl; // Esperado: beta alpha

    // Teste 3
    result = forcaRegex::preg_replace("/(?P<num>\\d+)/", "Order 123, code 456", "#$num#", 0);
    std::cout << "Teste 3: " << result << std::endl; // Esperado: Order #123#, code #456#

    // Teste 4
    result = forcaRegex::preg_replace("/\\[(?P<tag>[a-z]+)\\](?P<content>.*?)\\[\\/(?P=tag)\\]/", "[b]bold[/b] and [i]italic[/i]", "<$tag>$content</$tag>", 0);
    std::cout << "Teste 4: " << result << std::endl; // Esperado: <b>bold</b> and <i>italic</i>

    return 0;
}  */