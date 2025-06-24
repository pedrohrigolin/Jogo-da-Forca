#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <stdexcept>
#include "forcaStrings.h"
#include "forcaUtils.h"
#include "forcaFiles.h"

// Include das libs para usar funções próprias do SO
#if defined(OS_WIN) || defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

    #define NOMINMAX
    #include <windows.h>

#elif defined(OS_LINUX)

    #include <unistd.h>
    #include <limits.h>

#endif

namespace forcaFiles {

    namespace utils {

        /*
        |=========================================================
        |   FUNÇÕES UTILITÁRIAS PARA A MANIPULAÇÃO DE ARQUIVOS
        |=========================================================
        */

        /**
         * Normaliza um caminho de arquivo, removendo espaços, convertendo todas as barras invertidas (\)
         * para barras normais (/) e eliminando barras duplicadas, se desejado.
         *
         * Esta função é útil para padronizar caminhos de arquivos, evitando problemas de compatibilidade
         * entre sistemas operacionais e inconsistências causadas por múltiplas barras ou espaços.
         *
         * @param path   Caminho do arquivo a ser normalizado.
         * @param unify  (Opcional) Se true (padrão), remove barras '/' duplicadas. Se false, mantém múltiplas barras.
         * @return       Caminho normalizado, sem espaços, com barras '/' e, se unify=true, sem duplicidade.
         */
        std::string normalizePath( std::string path, bool unify ) {

            path = forcaStrings::removeSpaces(path);
            
            std::string::size_type pos = path.find("\\");

            // Remove / invertidas, comium no windows mas que pode gerar problemas no codigo.
            while( pos != std::string::npos ){

                path.replace(pos, 1, "/");

                pos = path.find("\\", pos);

            }

            if( ! unify ) return path;

            pos = path.find("/");

            // Remove / sequenciais
            while( pos != std::string::npos ){

                if( path[ pos - 1 ] == '/' ) path.erase(pos, 1);
                else pos++;

                pos = path.find("/", pos);

            }

            return path;

        }

        /**
         * Retorna o caminho absoluto do diretório do executável ou de um caminho relativo a partir dele.
         *
         * Se nenhum parâmetro for informado, retorna o caminho absoluto do diretório onde está o executável.
         * Caso um caminho relativo seja passado em `path`, retorna o caminho absoluto correspondente a partir do diretório do executável.
         * O parâmetro `unify` permite controlar se barras duplicadas serão removidas na normalização do caminho.
         *
         * @param path  (Opcional) Caminho relativo a ser convertido em absoluto a partir do diretório do executável. Se vazio, retorna apenas o diretório absoluto do executável.
         * @param unify (Opcional) Se true (padrão), remove barras '/' duplicadas no caminho final. Se false, mantém múltiplas barras.
         * @return      Caminho absoluto normalizado.
         */
        std::string root_realpath( std::string path, bool unify ) {
            
            static const std::filesystem::path current_dir = []() -> std::filesystem::path {

                #if defined(OS_WIN)

                    wchar_t path[MAX_PATH] = {0};

                    GetModuleFileNameW(NULL, path, MAX_PATH);

                    return std::filesystem::path(path).parent_path();

                #elif defined(OS_LINUX)

                    char result[PATH_MAX];

                    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);

                    return std::filesystem::path(std::string(result, (count > 0) ? count : 0)).parent_path();

                #else

                    // Fallback
                    return std::filesystem::current_path();

                #endif

            }();

            std::filesystem::path merge_path = current_dir / path;

            merge_path = merge_path.lexically_normal();

            std::string final_path = forcaFiles::utils::normalizePath( merge_path.string(), unify );

            return final_path;

        }

        /**
         * Retorna o caminho absoluto do diretório de execução atual ou de um caminho relativo a partir dele.
         *
         * Se nenhum parâmetro for informado, retorna o caminho absoluto do diretório de onde o programa foi executado.
         * Caso um caminho relativo seja passado em `path`, retorna o caminho absoluto correspondente a partir do diretório de execução.
         * O parâmetro `unify` permite controlar se barras duplicadas serão removidas na normalização do caminho.
         *
         * @param path  (Opcional) Caminho relativo a ser convertido em absoluto a partir do diretório de execução. Se vazio, retorna apenas o diretório absoluto de execução.
         * @param unify (Opcional) Se true (padrão), remove barras '/' duplicadas no caminho final. Se false, mantém múltiplas barras.
         * @return      Caminho absoluto normalizado.
         */
        std::string current_realpath( std::string path, bool unify ) {

            std::filesystem::path current_dir = std::filesystem::current_path();

            std::filesystem::path merge_path = current_dir / path;

            merge_path = merge_path.lexically_normal();

            std::string final_path = forcaFiles::utils::normalizePath( merge_path.string(), unify );

            return final_path;

        }

        /**
         * Verifica se um arquivo existe no caminho especificado.
         *
         * O caminho é normalizado antes da verificação para evitar problemas de formatação.
         * A função tenta abrir o arquivo em modo de leitura; se conseguir, considera que o arquivo existe.
         *
         * @param   std::string filepath   Caminho do arquivo a ser verificado.
         * @return  bool                   true se o arquivo existe, false caso contrário.
         */
        bool fileExist( std::string filepath ) {

            filepath = forcaFiles::utils::normalizePath(filepath);

            std::ifstream file(filepath);

            bool exist = file.is_open();

            file.close();

            return exist;

        }

        /**
         * Verifica se um arquivo pode ser lido no caminho especificado.
         *
         * O caminho é normalizado antes da verificação. A função tenta abrir o arquivo em modo de leitura
         * e verifica se o arquivo está acessível e em bom estado para leitura.
         *
         * @param   std::string filepath   Caminho do arquivo a ser verificado.
         * @return  bool                   true se o arquivo pode ser lido, false caso contrário.
         */
        bool canRead( std::string filepath ) {

            filepath = forcaFiles::utils::normalizePath(filepath);

            std::ifstream file(filepath);

            if( ! file.is_open() ) return false;

            if( ! file.good() ) return false;

            file.close();

            return true;

        }

        /**
         * Verifica se um arquivo pode ser escrito no caminho especificado.
         *
         * O caminho é normalizado antes da verificação. A função tenta abrir o arquivo em modo de escrita (append)
         * e realiza testes para garantir que é possível escrever no arquivo, verificando permissões e possíveis erros.
         *
         * @param   std::string filepath   Caminho do arquivo a ser verificado.
         * @return  bool                   true se o arquivo pode ser escrito, false caso contrário.
         */
        bool canWrite( std::string filepath ) {

            filepath = forcaFiles::utils::normalizePath(filepath);

            std::ofstream file(filepath, std::ios::app);

            if( ! file.is_open() ) return false;

            // Primeiro verifica com base na posição do ponteiro de escrita
            if( file.tellp() == -1 ) return false;

            // Agora tenta escreve um conteudo vazio mesmo
            file<<"";

            // Verifica se foi possivel escrever
            if( file.fail() ) return false;

            file.close();

            return true;

        }

        /**
         * Retorna o caminho de um arquivo a partir de uma chave e do tipo (DEFAULT, CUSTOM ou ANY).
         *
         * A função busca a chave primeiramente em forcaFileKeys, que representa arquivos presentes em ambas as pastas.
         * Se não encontrar, busca em forcaDefaultFiles (para DEFAULT) ou forcaCustomFiles (para CUSTOM).
         * Para o tipo ANY, busca em forcaAnyFiles, que pode conter qualquer arquivo do sistema.
         * 
         * Prioridade de busca:
         *   1. forcaFileKeys: prioridade máxima, pois representa arquivos compartilhados entre default e custom.
         *   2. forcaDefaultFiles ou forcaCustomFiles: caso não encontre na anterior, busca nos arquivos exclusivos de cada pasta.
         *   3. forcaAnyFiles: apenas se o tipo for ANY, busca em todos os arquivos conhecidos do sistema.
         *
         * @param key  Chave do arquivo a ser buscado.
         * @param type Tipo do caminho desejado: "DEFAULT", "CUSTOM" ou "ANY".
         * @return std::string Caminho do arquivo, já normalizado.
         * @throws std::invalid_argument Se o tipo for inválido ou a chave não existir no(s) mapa(s) correspondente(s).
         */
        std::string getPath( std::string key, std::string type ) {

            type = forcaStrings::normalizeWord(type);

            if( type != "DEFAULT" && type != "CUSTOM" && type != "ANY"){
                std::invalid_argument("O parametro type passado na funcao esta incorrete! Os valores esperados sao DEFAULT, CUSTOM OU ANY");
            }

            key = forcaStrings::removeSpaces(key);

            if(type != "ANY"){

                std::map<std::string, std::string>::const_iterator path = forcaFiles::forcaFileKeys.find(key);

                if(path == forcaFiles::forcaFileKeys.end()){
                    
                    if(type == "DEFAULT"){

                        path = forcaFiles::forcaDefaultFiles.find(key);

                        if(path == forcaFiles::forcaDefaultFiles.end()){
                            std::invalid_argument("O primeiro parametro (key) passado na funcao nao existe!");
                        }

                    }
                    else{

                        path = forcaFiles::forcaCustomFiles.find(key);

                        if(path == forcaFiles::forcaDefaultFiles.end()){
                            std::invalid_argument("O primeiro parametro (key) passado na funcao nao existe!");
                        }

                    }

                }

                std::string response;

                if(type == "DEFAULT"){
                    response = forcaFiles::utils::normalizePath(forcaFiles::forcaDefaultPath + path->second);
                }
                else{
                    response = forcaFiles::utils::normalizePath(forcaFiles::forcaCustomPath + path->second);
                }

                return response;

            }
            else{
             
                std::map<std::string, std::string>::const_iterator path = forcaFiles::forcaAnyFiles.find(key);

                if(path == forcaFiles::forcaAnyFiles.end()){
                    std::invalid_argument("O primeiro parametro (key) passado na funcao nao existe!");
                }

                std::string response = forcaFiles::utils::normalizePath(path->second);

                return response;

            }

        }

    }

    namespace create {

        /*
        |=====================================
        |   FUNÇÕES DE CRIAÇÃO DE ARQUIVOS
        |=====================================
        */

        /**
         * Cria um arquivo no caminho especificado, com conteúdo opcional.
         *
         * O caminho é normalizado antes da criação. Se o arquivo já existir, verifica permissões de leitura e escrita.
         * Caso não seja possível ler ou escrever, exibe mensagens de erro e retorna false.
         * Se o arquivo não existir ou for possível sobrescrever, cria o arquivo e grava o conteúdo informado.
         *
         * @param   std::string filepath   Caminho do arquivo a ser criado.
         * @param   std::string content    Conteúdo a ser gravado no arquivo (opcional, padrão é string vazia).
         * @return  bool                   true se o arquivo foi criado e escrito com sucesso, false caso contrário.
         */
        bool createFile( std::string filepath, std::string content ) {

            bool exist = forcaFiles::utils::fileExist(filepath);

            if( exist && ! forcaFiles::utils::canRead(filepath) ){

                forcaUtils::clear_screen();
                
                throw std::runtime_error("O arquivo " + filepath + " nao tem permissao de leitura, ou ocorreu um erro ao tentar abrir o arquivo, verifique e tente novamente!");
                
            }

            if( ! forcaFiles::utils::canWrite(filepath) ){

                forcaUtils::clear_screen();
                
                throw std::runtime_error("O arquivo " + filepath + " nao tem permissao de escrita!");

            }

            filepath = forcaFiles::utils::normalizePath(filepath);

            std::ofstream newFile;
            
            newFile.open(filepath, std::ios::binary);

            if( ! newFile.is_open() ) {

                throw std::runtime_error("Nao foi possivel abrir o arquivo " + filepath + " para criação!");

            }

            newFile<<content;

            if( newFile.fail() ){

                forcaUtils::clear_screen();
                
                newFile.close();
                
                throw std::runtime_error("Erro ao gravar o conteudo no arquivo " + filepath);

            }

            newFile.close();
            
            return true;

        }

    }

    namespace read {

        /*
        |=================================================
        |   FUNÇÕES DE EXTRAÇÃO DE CONTEÚDO DE ARQUIVO
        |=================================================
        */

        /**
         * Extrai e retorna o conteúdo de um arquivo no caminho especificado.
         *
         * O caminho é normalizado antes da leitura. A função verifica se o arquivo existe e se pode ser lido,
         * exibindo mensagens de erro e retornando uma string vazia em caso de falha. Se possível, lê todo o conteúdo
         * do arquivo e retorna como uma string.
         *
         * @param   std::string filepath   Caminho do arquivo a ser lido.
         * @return  std::string            Conteúdo do arquivo como string, ou string vazia em caso de erro.
         */
        std::string getContent( std::string filepath ) {

            bool exist = forcaFiles::utils::fileExist(filepath);

            if( ! exist ){

                forcaUtils::clear_screen();
                
                throw std::runtime_error("O arquivo " + filepath + " nao existe!");

            }

            if( ! forcaFiles::utils::canRead(filepath) ){

                forcaUtils::clear_screen();
                
                throw std::runtime_error("O arquivo " + filepath + " nao tem permissao de leitura!");

            }

            filepath = forcaFiles::utils::normalizePath(filepath);

            std::ifstream file(filepath, std::ios::binary | std::ios::ate);

            if( ! file.is_open() ) {

                throw std::runtime_error("Não foi possivel abrir o arquivo " + filepath + "!");

            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg); 

            if (size == -1) {

                forcaUtils::clear_screen();

                file.close();

                throw std::runtime_error("Erro ao obter tamanho do arquivo " + filepath);

            }

            std::string content;
            content.resize(static_cast<size_t>(size));

            file.read(&content[0], size);

            if( file.fail() ){

                forcaUtils::clear_screen();

                file.close();
                
                throw std::runtime_error("Erro ao extrair conteudo do arquivo " + filepath);

            }

            file.close();

            return content;

        }

    }

    /* 
        Porque outro namespace utils? Pois as funções desse precisam da função read ou create,
        e a função read e create precisam das funções dentro do namespace utils acima.
        Isso não altera em nada a declaração do namespace acima, só adiciona mais coisas.
    */
    namespace utils {

        /**
         * Verifica se um arquivo está vazio, ignorando espaços em branco.
         * 
         * Esta função abre o arquivo especificado e analisa seu conteúdo para determinar
         * se está vazio. Um arquivo é considerado vazio se:
         * - Não contém nenhum caractere, ou
         * - Contém apenas caracteres de espaço em branco (espaços, tabs, quebras de linha)
         * 
         * @param   std::string filename   Caminho do arquivo a ser verificado
         * @return  bool                   true se o arquivo está vazio, false caso contrário
         * @throws  std::runtime_error     Se o arquivo não existe ou não pode ser lido
         */
        bool isEmpty( std::string filename ) {

            if( ! forcaFiles::utils::fileExist(filename) ){
                throw std::runtime_error("O arquivo " + forcaFiles::utils::normalizePath(filename) + " nao existe!");
            }

            if( ! forcaFiles::utils::canRead(filename) ){
                throw std::runtime_error("O arquivo " + forcaFiles::utils::normalizePath(filename) + " nao permite leitura!");
            }

            std::string content = forcaFiles::read::getContent(filename);

            /* 
                Porque usei a função de remover espaços? Pois nesse caso, uma string não vazia (string.length() > 0),
                mas que só apresenta espaços é considerada vazia, portanto, se eu remover todos os espaços a string
                fica realmente vazia (string.length() == 0) e podemos usar a função string.empty()
            */
            content = forcaStrings::removeSpaces(content);

            if(content.empty()) return true;

            return false;

        }

    }

    namespace validate {

        /*
        |===========================
        |   FUNÇÕES DE VALIDAÇÃO
        |===========================
        */

        /**
         * Valida a existência e integridade de todos os arquivos essenciais do sistema.
         * 
         * Verifica cada arquivo listado em forcaPaths, garantindo que:
         * - Existe uma versão padrão (default) do arquivo
         * - A versão padrão pode ser lida e não está vazia
         * - Existe uma versão customizável do arquivo (ou cria uma cópia da versão padrão)
         * - A versão customizável pode ser lida e escrita
         * - A versão customizável não está vazia (se estiver, restaura conteúdo da versão padrão)
         * 
         * @return  bool                   true se todos os arquivos são válidos
         * @throws  std::runtime_error     Se algum arquivo padrão não existe, não pode ser lido ou está vazio
         *                                 Se algum arquivo customizável não pode ser escrito
         */
        bool validateEssentialFiles() {
           
            for( const auto& name : forcaFiles::forcaFileKeys ) {

                std::string filename = forcaFiles::utils::normalizePath( forcaFiles::forcaDefaultPath + name.second );

                if( ! forcaFiles::utils::fileExist(filename) ){
                    throw std::runtime_error("O arquivo " + filename + " nao existe!");
                }

                if( ! forcaFiles::utils::canRead(filename) ){
                    throw std::runtime_error("O arquivo " + filename + " nao permite leitura!");
                }

                if( forcaFiles::utils::isEmpty(filename) ){
                    throw std::runtime_error("O arquivo " + filename + " nao pode estar vazio!");
                }

                filename = forcaFiles::utils::normalizePath( forcaFiles::forcaCustomPath + name.second );

                std::string content = "";

                if( ! forcaFiles::utils::fileExist(filename) ){

                    // TODO: EVOLUIR ISSO PARA UM BLOCO TRY CATCH FUTURAMENTE

                    content = forcaFiles::read::getContent( forcaFiles::utils::normalizePath( forcaFiles::forcaDefaultPath + name.second ) );

                    forcaFiles::create::createFile(filename, content);

                }

                if( ! forcaFiles::utils::canWrite(filename) ){

                    throw std::runtime_error("O arquivo " + filename + " nao pode ser escrito!");

                }

                // Se o arquivo não permite leitura, sobreescreve ele para permitir
                if( ! forcaFiles::utils::canRead(filename) ){

                    // TODO: EVOLUIR ISSO PARA UM BLOCO TRY CATCH FUTURAMENTE

                    if(content == ""){
                        content = forcaFiles::read::getContent( forcaFiles::utils::normalizePath( forcaFiles::forcaDefaultPath + name.second ) );
                    }

                    forcaFiles::create::createFile(filename, content);

                }

                if( forcaFiles::utils::isEmpty(filename) ){

                    // TODO: EVOLUIR ISSO PARA UM BLOCO TRY CATCH FUTURAMENTE

                    if(content == ""){
                        content = forcaFiles::read::getContent( forcaFiles::utils::normalizePath( forcaFiles::forcaDefaultPath + name.second ) );
                    }

                    forcaFiles::create::createFile(filename, content);

                }

            }

            for( const auto& name : forcaFiles::forcaDefaultFiles ) {

                std::string filename = forcaFiles::utils::normalizePath( forcaFiles::forcaDefaultPath + name.second );

                if( ! forcaFiles::utils::fileExist(filename) ){
                    throw std::runtime_error("O arquivo " + filename + " nao existe!");
                }

                if( ! forcaFiles::utils::canRead(filename) ){
                    throw std::runtime_error("O arquivo " + filename + " nao permite leitura!");
                }

                if( forcaFiles::utils::isEmpty(filename) ){
                    throw std::runtime_error("O arquivo " + filename + " nao pode estar vazio!");
                }

            }

            return true;

        }

    }

}