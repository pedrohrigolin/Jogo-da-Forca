#ifndef FORCA_FILES_H
#define FORCA_FILES_H

#include <string>
#include <cstring>
#include <map>

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

        std::string normalizePath( std::string path );

        bool fileExist( std::string filepath );

        bool canRead( std::string filepath );

        bool canWrite( std::string filepath );

    }

    namespace create {

        bool createFile( std::string filepath, std::string content );

    }

    namespace read {

        std::string getContent( std::string filepath );

    }

    namespace utils {

        bool isEmpty( std::string filename );

    }

    namespace validate {

        bool validateFiles();

    }

}

#endif