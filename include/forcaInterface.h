#ifndef FORCA_INTERFACE_H
#define FORCA_INTERFACE_H

#if defined(OS_WIN) || defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

#define NOMINMAX
#include <windows.h>

#elif defined(OS_LINUX)

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#endif

#include <string>
#include <functional>
#include <map>
#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include <typeinfo>

#include "include/cef_base.h"
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_v8.h"
#include "include/cef_life_span_handler.h"
#include "include/cef_request_handler.h"
#include "include/cef_load_handler.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"

// =================================================================================================
// --- SEÇÃO 1: FUNÇÕES AUXILIARES GLOBAIS ---
// =================================================================================================

class ForcaInterfaceGPU {

private:

    ForcaInterfaceGPU() {}

    struct gGPUHandler {

        private:

            bool gpu_disabled = false;
            std::string temp_flag = "temp_disabled_gpu.flag";
            std::string always_flag = "always_disabled_gpu.flag";

        public:

            /*
                Isso daqui é tipo uma trava permanente da variável gpu_disabled, pois entende que se o programa 
                definiu como verdadeiro, tem que continuar assim até o final da execução, então isso previne 
                redefinições acidentais para false. 
            */
            void setStatus( bool status ) {

                if( status ){
                    gGPUHandler::gpu_disabled = true;
                }

            }

            bool getStatus() {
                return gGPUHandler::gpu_disabled;
            }

            void createTempFlag() {

                std::ofstream flag_file(gGPUHandler::temp_flag);

                if (flag_file.is_open()) {

                    flag_file << "TRUE";
                    flag_file.close();

                }

            }

            void deleteTempFlag() {

                (void)std::filesystem::remove(gGPUHandler::temp_flag);

            }

            void createAlwaysFlag() {

                std::ofstream flag_file(gGPUHandler::always_flag);

                if (flag_file.is_open()) {

                    flag_file << "TRUE";
                    flag_file.close();

                }

            }

            void deleteAlwaysFlag() {

                (void)std::filesystem::remove(gGPUHandler::always_flag);

            }

            void checkFlag( int argc, char* argv[] ){
                
                if( gGPUHandler::gpu_disabled ) return;

                int i;

                for(i=1; i<argc; i++){

                    if(std::strncmp(argv[i], "--disable-gpu", 13) == 0) gGPUHandler::setStatus(true);

                }

                if( gGPUHandler::gpu_disabled ) return;

                std::ifstream flag_file(temp_flag);

                if( flag_file.is_open() ){

                    gGPUHandler::setStatus(true);

                    flag_file.close();

                }

                if( gGPUHandler::gpu_disabled ) return;

                flag_file.open(always_flag);

                if( flag_file.is_open() ){

                    gGPUHandler::setStatus(true);

                    flag_file.close();

                }

            }

            void showGPUError(){

                #if defined(OS_WIN)

                    // CORREÇÃO 3: Usar MessageBoxW para strings Unicode (com o L"...")
                    MessageBoxW(NULL,
                            L"Ocorreu um erro no subsistema grafico.\nAgora o programa sera fechado e voce precisara abrir de novo.\nNa proxima vez, a aplicacao iniciara em um modo de renderizacao mais seguro.",
                            L"Erro de GPU",
                            MB_OK | MB_ICONERROR);

                #elif defined(OS_LINUX)

                    GtkWidget* dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Ocorreu um erro no subsistema gráfico.");
                    
                    gtk_message_dialog_format_secondary_text(

                        GTK_MESSAGE_DIALOG(dialog),
                        "Agora o programa sera fechado e voce precisara abrir de novo.\nNa proxima vez, a aplicacao iniciara em um modo de renderizacao mais seguro.");
                        gtk_dialog_run(GTK_DIALOG(dialog)

                    );

                    gtk_widget_destroy(dialog);

                #endif

            }

            bool isGPUError(int code){

                if( code == CEF_RESULT_CODE_GPU_DEAD_ON_ARRIVAL || code == CEF_RESULT_CODE_GPU_EXIT_ON_CONTEXT_LOST ){
                    return true;
                }

                return false;

            }

    };

public:

    static ForcaInterfaceGPU::gGPUHandler GPUHandler;

};


// =================================================================================================
// --- SEÇÃO 2: ARQUITETURA DA PONTE JS <-> C++ ---
// =================================================================================================

/**
 * Roteia chamadas de API do JavaScript para as funções C++ correspondentes.
 * Este é o coração da nossa arquitetura "bonita", substituindo o bloco if/else.
 */
class ApiRouter {

public:

    // Define um tipo "FuncType" para nossas funções: recebe os argumentos do CEF e não retorna nada.
    using FuncType = std::function<void(CefRefPtr<CefListValue>)>;

    // Registra uma função no mapa, associando um nome (string) a uma função C++.
    void RegisterFunction(const std::string& name, FuncType func);

    // Procura uma função pelo nome no mapa e a executa com os argumentos fornecidos.
    bool HandleCall(const std::string& name, CefRefPtr<CefListValue> args);

private:

    std::map<std::string, FuncType> function_map_;

};

/**
 * Atua no Processo de Renderização. É um despachante genérico que pega qualquer
 * chamada de uma função exposta, empacota o nome e os argumentos, e envia para o
 * Processo do Navegador via IPC. Ele não precisa de nenhuma lógica específica.
 */
class ApiBridgeHandler : public CefV8Handler {

public:

    ApiBridgeHandler();

    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

private:

    // Pega o contexto atual (JavaScript)
    CefRefPtr<CefV8Context> jsContext = CefV8Context::GetCurrentContext();
    CefRefPtr<CefV8Value> jsGlobal = jsContext->GetGlobal();

    // Pega o objeto global String
    CefRefPtr<CefV8Value> jsStringObj = jsGlobal->GetValue("String");

    IMPLEMENT_REFCOUNTING(ApiBridgeHandler);

};

/**
 * Roteador para as funções V8 nativas e síncronas.
 */
class NativeApiRouter {

public:

    using FuncType = std::function< bool(const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) >;

    void RegisterFunction(const std::string& name, FuncType func);

    bool HandleCall(const CefString& name, const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception);

private:

    std::map<std::string, FuncType> function_map_;

};

/**
 * Handler para executar funções nativas no Renderer Process.
 */
class NativeFunctionHandler : public CefV8Handler {

public:

    NativeFunctionHandler();

    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

private:
    
    std::unique_ptr<NativeApiRouter> router_;

    // Pega o contexto atual (JavaScript)
    CefRefPtr<CefV8Context> jsContext = CefV8Context::GetCurrentContext();
    CefRefPtr<CefV8Value> jsGlobal = jsContext->GetGlobal();

    // Pega o objeto global String
    CefRefPtr<CefV8Value> jsStringObj = jsGlobal->GetValue("String");
    
    IMPLEMENT_REFCOUNTING(NativeFunctionHandler);

};

// =================================================================================================
// --- SEÇÃO 3: LÓGICA DA APLICAÇÃO (BROWSER PROCESS) ---
// =================================================================================================

/**
 * Gerencia eventos do navegador e da janela. Contém nosso ApiRouter.
 */
class ForcaCefClient : 
    public CefClient, 
    public CefContextMenuHandler, 
    public CefLifeSpanHandler, 
    public CefRequestHandler,
    public CefLoadHandler
{

public:

    ForcaCefClient();

    // Getters para os handlers
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override { return this; }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }

    // Desabilita o menu de contexto
    void OnBeforeContextMenu(CefRefPtr<CefBrowser> b, CefRefPtr<CefFrame> f, CefRefPtr<CefContextMenuParams> p, CefRefPtr<CefMenuModel> m) override;

    // Captura a referência do browser quando ele é criado para usá-la nas lambdas.
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // --- MÉTODO DE DETECÇÃO DE CRASH GPU ---
    // Método de detecção de crash
    void OnRenderProcessTerminated(
        CefRefPtr<CefBrowser> browser,
        TerminationStatus status,
        int error_code,
        const CefString& error_string
    ) override;

    // Recebe a mensagem do Renderer Process e a despacha para o nosso ApiRouter.
    // Este método agora é extremamente simples e não precisa mais mudar.
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> b, CefRefPtr<CefFrame> f, CefProcessId pid, CefRefPtr<CefProcessMessage> msg) override;

private:

    void ResolvePromise(CefRefPtr<CefFrame> frame, const CefString& promise_id, const CefString& data);

    void RejectPromise(CefRefPtr<CefFrame> frame, const CefString& promise_id, const CefString& error_message);

    // Centraliza o registro de todas as funções da nossa API C++.
    void RegisterApiFunctions();

    std::unique_ptr<ApiRouter> router_;

    CefRefPtr<CefBrowser> browser_;

    IMPLEMENT_REFCOUNTING(ForcaCefClient);

};

/**
 * Delegate da janela para o framework CEF Views.
 */
class ForcaWindowDelegate : public CefWindowDelegate {

public:

    explicit ForcaWindowDelegate(CefRefPtr<CefBrowserView> browser_view);

    void OnWindowCreated(CefRefPtr<CefWindow> window) override;

    void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;

    bool CanClose(CefRefPtr<CefWindow> window) override;
    
    // Este método é chamado pelo framework para saber o tamanho mínimo da janela.
    CefSize GetMinimumSize(CefRefPtr<CefView> view) override;

private:

    CefRefPtr<CefBrowserView> browser_view_;

    IMPLEMENT_REFCOUNTING(ForcaWindowDelegate);

};


// =================================================================================================
// --- SEÇÃO 5: CLASSE PRINCIPAL DA APLICAÇÃO E PONTO DE ENTRADA ---
// =================================================================================================

class ForcaCefApp : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler {

public:

    ForcaCefApp();

    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

    void OnBeforeChildProcessLaunch(
        CefRefPtr<CefCommandLine> command_line) override;

    void OnBeforeCommandLineProcessing (
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) override;

    // Cria a UI principal quando o CEF está pronto.
    void OnContextInitialized() override;

    // Anuncia as funções para o JavaScript no Processo de Renderização.
    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

private:

    IMPLEMENT_REFCOUNTING(ForcaCefApp);

};

class ForcaInterface {

public: 

    static std::string demangle( const char* name );

    static std::string exceptionText( const std::exception& e );

    static bool init( int argc, char* argv[] );

};

#endif