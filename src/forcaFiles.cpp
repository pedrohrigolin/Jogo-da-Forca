#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <stdexcept>
#include "forcaStrings.h"
#include "forcaUtils.h"
#include "forcaFiles.h"

namespace forcaFiles {

    /**
     * Caminho relativo base para os arquivos padrão do sistema.
     * 
     * Esta constante define o diretório que contém os arquivos essenciais e imutáveis
     * do programa, como recursos base, configurações padrão e arquivos do sistema
     * que não devem ser modificados durante a execução.
     */
    std::string const forcaDefaultPath = "../files/default/";

    /**
     * Caminho relativo base para os arquivos customizáveis.
     * 
     * Define o diretório que armazena arquivos que podem ser modificados pelo usuário,
     * como configurações personalizadas, dados salvos e recursos adicionais
     * que podem ser alterados sem afetar o funcionamento base do programa.
     */
    std::string const forcaCustomPath = "../files/custom/";

    /**
     * Mapeamento de identificadores para caminhos de arquivos do sistema.
     * 
     * Estrutura que associa identificadores legíveis (keys) aos caminhos relativos
     * dos arquivos do programa. Facilita o acesso e manutenção dos caminhos,
     * centralizando as referências e permitindo validações consistentes.
     */
    std::map<std::string, std::string> const forcaPaths = {

        {"WORDS_LEVELS_EASY", "words/levels/easy.txt"},
        {"WORDS_LEVELS_NORMAL", "words/levels/normal.txt"},
        {"WORDS_LEVELS_HARD", "words/levels/hard.txt"}

    };

    namespace utils {

        /*
        |=========================================================
        |   FUNÇÕES UTILITÁRIAS PARA A MANIPULAÇÃO DE ARQUIVOS
        |=========================================================
        */

        /**
         * Normaliza um caminho de arquivo, removendo espaços, convertendo todas as barras invertidas (\)
         * para barras normais (/) e eliminando barras duplicadas. 
         * 
         * Essa função é útil para padronizar caminhos de arquivos, evitando problemas de compatibilidade
         * entre sistemas operacionais e inconsistências causadas por múltiplas barras ou espaços.
         *
         * @param   std::string path    Caminho do arquivo a ser normalizado.
         * @return  std::string         Caminho normalizado, sem espaços, com barras '/' e sem duplicidade.
         */
        std::string normalizePath( std::string path ) {

            path = forcaStrings::removeSpaces(path);

            std::string::size_type pos = path.find("\\");

            // Remove / invertidas, comium no windows mas que pode gerar problemas no codigo.
            while( pos != std::string::npos ){

                path.replace(pos, 1, "/");

                pos = path.find("\\", pos);

            }

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
        bool createFile( std::string filepath, std::string content = "" ) {

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
            
            newFile.open(filepath);

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

        bool isEmpty( std::string filename ) {

            if( ! forcaFiles::utils::fileExist(filename) ){
                throw std::runtime_error("O arquivo" + forcaFiles::utils::normalizePath(filename) + " nao existe!");
            }

            if( ! forcaFiles::utils::canRead(filename) ){
                throw std::runtime_error("O arquivo" + forcaFiles::utils::normalizePath(filename) + " nao permite leitura!");
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



    }

}