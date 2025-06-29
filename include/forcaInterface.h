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

/**
 * @class ForcaInterfaceGPU
 * @brief Gerencia o estado da GPU e flags de inicialização para fallback seguro em caso de falha gráfica.
 *
 * Responsável por criar, remover e verificar flags de desabilitação de GPU, além de exibir mensagens de erro
 * específicas para cada plataforma (Windows/Linux) quando ocorre falha no subsistema gráfico.
 */
class ForcaInterfaceGPU {

private:

    ForcaInterfaceGPU() {}

    /**
     * @struct ForcaInterfaceGPU::gGPUHandler
     * @brief Estrutura responsável por gerenciar o estado da GPU e flags de fallback gráfico.
     *
     * Esta struct encapsula toda a lógica de controle de fallback gráfico, incluindo:
     * - Controle do status de desabilitação da GPU durante a execução do programa.
     * - Criação e remoção de flags temporárias e permanentes para desabilitar a GPU.
     * - Detecção automática de flags via argumentos de linha de comando ou arquivos de flag.
     * - Exibição de mensagens de erro específicas para o usuário em caso de falha gráfica, com suporte a Windows e Linux.
     * - Verificação de códigos de erro relacionados à GPU para acionar o modo seguro.
     *
     * O objetivo é garantir que, caso ocorra uma falha no subsistema gráfico, a aplicação possa reiniciar em modo seguro,
     * evitando redefinições acidentais do status da GPU e fornecendo feedback claro ao usuário.
     */    
    struct gGPUHandler {

        private:

            // Propriedade que identifica se a GPU foi desabilitada ou não
            bool gpu_disabled = false;

            // Nome da flag de desabilitar a GPU temporariamente
            std::string temp_flag = "temp_disabled_gpu.flag";

            // Nome da flag de desabilitar a GPU permanentemente
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

            /**
             * @brief Retorna o status atual da GPU.
             *
             * Indica se a GPU foi desabilitada durante a execução do programa, seja por flag temporária, permanente
             * ou argumento de linha de comando. Retorna true se a GPU está desabilitada, false caso contrário.
             *
             * @return true se a GPU está desabilitada, false caso contrário.
             */
            bool getStatus() {
                return gGPUHandler::gpu_disabled;
            }

            /**
             * @brief Cria uma flag temporária para desabilitar a GPU.
             *
             * Gera um arquivo temporário no sistema de arquivos indicando que a GPU deve ser desabilitada
             * na próxima inicialização do programa. Utilizado como mecanismo de fallback gráfico em caso de falha.
             */            
            void createTempFlag() {

                std::ofstream flag_file(gGPUHandler::temp_flag);

                if (flag_file.is_open()) {

                    flag_file << "TRUE";
                    flag_file.close();

                }

            }

            /**
             * @brief Remove a flag temporária que desabilita a GPU.
             *
             * Exclui o arquivo temporário do sistema de arquivos, permitindo que a GPU seja reabilitada
             * na próxima inicialização do programa, caso não haja outras flags ativas.
             */            
            void deleteTempFlag() {

                (void)std::filesystem::remove(gGPUHandler::temp_flag);

            }

            /**
             * @brief Cria uma flag permanente para desabilitar a GPU.
             *
             * Gera um arquivo permanente no sistema de arquivos indicando que a GPU deve ser desabilitada
             * em todas as inicializações futuras do programa, até que a flag seja removida manualmente.
             */            
            void createAlwaysFlag() {

                std::ofstream flag_file(gGPUHandler::always_flag);

                if (flag_file.is_open()) {

                    flag_file << "TRUE";
                    flag_file.close();

                }

            }

            /**
             * @brief Remove a flag permanente que desabilita a GPU.
             *
             * Exclui o arquivo permanente do sistema de arquivos, permitindo que a GPU seja reabilitada
             * em futuras inicializações do programa.
             */            
            void deleteAlwaysFlag() {

                (void)std::filesystem::remove(gGPUHandler::always_flag);

            }

            /**
             * @brief Verifica a existência de flags ou argumentos para desabilitar a GPU.
             *
             * Analisa os argumentos de linha de comando e a existência dos arquivos de flag temporária e permanente.
             * Se encontrar qualquer indicação para desabilitar a GPU, atualiza o status interno para refletir isso.
             *
             * @param argc Quantidade de argumentos da linha de comando.
             * @param argv Vetor de argumentos da linha de comando.
             */            
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

            /**
             * @brief Exibe uma mensagem de erro ao usuário em caso de falha gráfica.
             *
             * Mostra uma mensagem apropriada para a plataforma (Windows ou Linux) informando que ocorreu um erro
             * no subsistema gráfico e que o programa será reiniciado em modo seguro.
             */            
            void showGPUError(){

                #if defined(OS_WIN)

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

            /**
             * @brief Verifica se o código de erro está relacionado a falha de GPU.
             *
             * Analisa o código de erro retornado pelo CEF para identificar se corresponde a falhas conhecidas de GPU,
             * como CEF_RESULT_CODE_GPU_DEAD_ON_ARRIVAL ou CEF_RESULT_CODE_GPU_EXIT_ON_CONTEXT_LOST.
             *
             * @param code Código de erro retornado pelo CEF.
             * @return true se o erro está relacionado à GPU, false caso contrário.
             */            
            bool isGPUError(int code){

                if( code == CEF_RESULT_CODE_GPU_DEAD_ON_ARRIVAL || code == CEF_RESULT_CODE_GPU_EXIT_ON_CONTEXT_LOST ){
                    return true;
                }

                return false;

            }

    };

public:

    /**
     * @var ForcaInterfaceGPU::GPUHandler
     * @brief Instância global para gerenciamento do estado da GPU e flags de fallback.
     */
    static ForcaInterfaceGPU::gGPUHandler GPUHandler;

};

/**
 * @class ApiRouter
 * @brief Roteador de chamadas de API do JavaScript para funções C++.
 *
 * Permite registrar funções C++ (lambdas ou funções livres) associadas a nomes, e despacha chamadas vindas do JS
 * para a função correta, substituindo grandes blocos if/else por um mapa de funções.
 */
class ApiRouter {

public:

    // Define um tipo "FuncType" para nossas funções: recebe os argumentos do CEF e não retorna nada.
    using FuncType = std::function<void(CefRefPtr<CefListValue>)>;

    /**
     * @brief Registra uma função C++ para ser chamada via JS.
     * @param name Nome da função (string) que será chamada do JS.
     * @param func Função C++ (lambda ou std::function) a ser executada.
     */
    void RegisterFunction(const std::string& name, FuncType func);

    /**
     * @brief Executa a função registrada correspondente ao nome informado.
     * @param name Nome da função a ser chamada.
     * @param args Argumentos recebidos do JS via CEF.
     * @return true se a função foi encontrada e executada, false caso contrário.
     */    
    bool HandleCall(const std::string& name, CefRefPtr<CefListValue> args);

private:

    std::map<std::string, FuncType> function_map_;

};

/**
 * @class ApiBridgeHandler
 * @brief Handler genérico para despachar chamadas JS para o processo do navegador via IPC.
 * 
 * Atua no Processo de Renderização. É um despachante genérico que pega qualquer
 * chamada de uma função exposta, empacota o nome e os argumentos, e envia para o
 * Processo do Navegador via IPC. Ele não precisa de nenhuma lógica específica.
 */
class ApiBridgeHandler : public CefV8Handler {

public:

    ApiBridgeHandler();

    /**
     * @brief Executa a chamada JS, empacota argumentos e envia para o processo do navegador.
     * @param name Nome da função JS chamada.
     * @param object Objeto JS.
     * @param arguments Lista de argumentos JS.
     * @param retval Valor de retorno para o JS.
     * @param exception Exceção, se houver.
     * @return true se a mensagem foi enviada com sucesso.
     */    
    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

private:

    // Pega o contexto atual (JavaScript)
    CefRefPtr<CefV8Context> jsContext = CefV8Context::GetCurrentContext();
    CefRefPtr<CefV8Value> jsGlobal = jsContext->GetGlobal();

    // Pega o objeto global String
    CefRefPtr<CefV8Value> jsStringObj = jsGlobal->GetValue("String");

    // Pega o objeto global Blob
    CefRefPtr<CefV8Value> jsBlobObj = jsGlobal->GetValue("Blob");

    // Pega o objeto global FileReader
    CefRefPtr<CefV8Value> jsFileReaderObj = jsGlobal->GetValue("FileReader");

    IMPLEMENT_REFCOUNTING(ApiBridgeHandler);

};

/**
 * @class NativeApiRouter
 * @brief Roteador para funções síncronas nativas expostas ao JS.
 *
 * Permite registrar funções C++ que podem ser chamadas diretamente do JS, retornando valores e exceções.
 */
class NativeApiRouter {

public:

    using FuncType = std::function< bool(const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception) >;

    /**
     * @brief Registra uma função nativa síncrona.
     * @param name Nome da função.
     * @param func Função C++ a ser executada.
     */    
    void RegisterFunction(const std::string& name, FuncType func);

    /**
     * @brief Executa a função registrada correspondente ao nome informado.
     * @param name Nome da função.
     * @param args Argumentos JS.
     * @param retval Valor de retorno.
     * @param exception Exceção, se houver.
     * @return true se a função foi encontrada e executada.
     */    
    bool HandleCall(const CefString& name, const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& exception);

private:

    std::map<std::string, FuncType> function_map_;

};

/**
 * @class NativeFunctionHandler
 * @brief Handler para executar funções nativas síncronas no Renderer Process.
 *
 * Recebe chamadas JS, roteia para a função C++ correta e retorna o valor ou exceção para o JS.
 */
class NativeFunctionHandler : public CefV8Handler {

public:

    NativeFunctionHandler();

    /**
     * @brief Executa a função nativa correspondente ao nome informado.
     * @param name Nome da função.
     * @param object Objeto JS.
     * @param arguments Argumentos JS.
     * @param retval Valor de retorno.
     * @param exception Exceção, se houver.
     * @return true se a função foi executada.
     */    
    virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) override;

private:
    
    std::unique_ptr<NativeApiRouter> router_;

    // Pega o contexto atual (JavaScript)
    CefRefPtr<CefV8Context> jsContext = CefV8Context::GetCurrentContext();
    CefRefPtr<CefV8Value> jsGlobal = jsContext->GetGlobal();

    // Pega o objeto global String
    CefRefPtr<CefV8Value> jsStringObj = jsGlobal->GetValue("String");

    // Pega o objeto global Blob
    CefRefPtr<CefV8Value> jsBlobObj = jsGlobal->GetValue("Blob");

    // Pega o objeto global FileReader
    CefRefPtr<CefV8Value> jsFileReaderObj = jsGlobal->GetValue("FileReader");
    
    IMPLEMENT_REFCOUNTING(NativeFunctionHandler);

};

/**
 * @class ForcaCefClient
 * @brief Gerencia eventos do navegador, janela e centraliza o roteamento de funções JS <-> C++.
 *
 * Responsável por registrar funções da API, tratar eventos do navegador, manipular promessas JS e gerenciar a ponte entre JS e C++.
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

    /**
     * @brief Desabilita o menu de contexto do navegador.
     */
    void OnBeforeContextMenu(CefRefPtr<CefBrowser> b, CefRefPtr<CefFrame> f, CefRefPtr<CefContextMenuParams> p, CefRefPtr<CefMenuModel> m) override;

    /**
     * @brief Captura referência do browser ao ser criado.
     */    
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

    /**
     * @brief Evento chamado antes do navegador ser fechado.
     */
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    /**
     * @brief Detecta crash da GPU e executa fallback seguro.
     */
    void OnRenderProcessTerminated(
        CefRefPtr<CefBrowser> browser,
        TerminationStatus status,
        int error_code,
        const CefString& error_string
    ) override;

    /**
     * @brief Recebe mensagens do Renderer Process e despacha para o ApiRouter.
     */
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> b, CefRefPtr<CefFrame> f, CefProcessId pid, CefRefPtr<CefProcessMessage> msg) override;

private:

    /**
     * @brief Resolve uma Promise JS com sucesso, enviando o resultado.
     * @param frame Frame JS.
     * @param promise_id ID da Promise.
     * @param data Dados de retorno.
     */
    void ResolvePromise(CefRefPtr<CefFrame> frame, const CefString& promise_id, const CefString& data);

    /**
     * @brief Rejeita uma Promise JS, enviando mensagem de erro.
     * @param frame Frame JS.
     * @param promise_id ID da Promise.
     * @param error_message Mensagem de erro.
     */    
    void RejectPromise(CefRefPtr<CefFrame> frame, const CefString& promise_id, const CefString& error_message);

    /**
     * @brief Centraliza o registro de todas as funções da API C++ expostas ao JS.
     *
     * Cada chamada de RegisterFunction associa um nome JS a uma função C++ (lambda).
     * Exemplo de lambda registrada:
     * @code
     * router_->RegisterFunction("abrirDevTools", [this](CefRefPtr<CefListValue> args) {
     *     if (browser_) browser_->GetHost()->ShowDevTools(...);
     * });
     * @endcode
     */
    void RegisterApiFunctions();

    std::unique_ptr<ApiRouter> router_;

    CefRefPtr<CefBrowser> browser_;

    IMPLEMENT_REFCOUNTING(ForcaCefClient);

};

/**
 * @class ForcaWindowDelegate
 * @brief Delegate da janela para o framework CEF Views.
 *
 * Gerencia criação, destruição e propriedades da janela principal do app.
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

/**
 * @class ForcaCefApp
 * @brief Classe principal da aplicação CEF, ponto de entrada e gerenciamento de contexto.
 *
 * Gerencia inicialização, configuração de switches, criação da UI e exposição das funções C++ para o JS.
 */
class ForcaCefApp : public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler {

public:

    ForcaCefApp();

    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }

    /**
     * @brief Monitora processos filhos, especialmente GPU, para fallback seguro.
     */    
    void OnBeforeChildProcessLaunch(
        CefRefPtr<CefCommandLine> command_line) override;

    /**
     * @brief Configura switches de linha de comando antes da inicialização.
     */        
    void OnBeforeCommandLineProcessing (
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) override;

    /**
     * @brief Cria a UI principal quando o CEF está pronto.
     */
    void OnContextInitialized() override;

    /**
     * @brief Expõe funções C++ para o JS no Renderer Process.
     */
    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;

private:

    IMPLEMENT_REFCOUNTING(ForcaCefApp);

};

/**
 * @class ForcaInterface
 * @brief Funções auxiliares globais para manipulação de exceções e inicialização da aplicação.
 */
class ForcaInterface {

public: 

    /**
     * @brief Torna o nome de exceção mais legível (demangle).
     * @param name Nome do tipo.
     * @return Nome legível.
     */
    static std::string demangle( const char* name );

    /**
     * @brief Retorna texto detalhado da exceção para o JS.
     * @param e Exceção capturada.
     * @return Texto detalhado.
     */    
    static std::string exceptionText( const std::exception& e );

    /**
     * @brief Inicializa a aplicação CEF.
     * @param argc Argumentos.
     * @param argv Argumentos.
     * @return Código de saída.
     */    
    static bool init( int argc, char* argv[] );

};

#endif