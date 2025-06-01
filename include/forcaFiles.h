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
     * Mapeamento de identificadores para caminhos de arquivos presentes em ambas as pastas default e custom.
     *
     * Este mapa serve para identificar arquivos que possuem uma versão tanto na pasta padrão (default) quanto na customizada (custom).
     * Use este mapa para arquivos que existem nas duas pastas, permitindo alternar facilmente entre as versões conforme necessário.
     * Para arquivos exclusivos de uma das pastas, utilize as constantes forcaDefaultFiles ou forcaCustomFiles.
     *
     * Estrutura que associa identificadores legíveis (keys) aos caminhos relativos dos arquivos compartilhados.
     */
    std::map<std::string, std::string> const forcaFileKeys = {

        {"WORDS_LEVELS_EASY", "words/levels/easy.txt"},
        {"WORDS_LEVELS_NORMAL", "words/levels/normal.txt"},
        {"WORDS_LEVELS_HARD", "words/levels/hard.txt"}

    };

    /**
     * Mapeamento de identificadores para arquivos exclusivos da pasta padrão (default).
     *
     * Use este mapa para arquivos que existem apenas na pasta default e não possuem equivalente customizado.
     * Facilita o acesso e manutenção dos arquivos essenciais e imutáveis do sistema.
     */
    std::map<std::string, std::string> const forcaDefaultFiles = {};

    /**
     * Mapeamento de identificadores para arquivos exclusivos da pasta customizada (custom).
     *
     * Use este mapa para arquivos que existem apenas na pasta custom e não possuem equivalente na pasta padrão.
     * Ideal para recursos, configurações ou dados criados ou modificados pelo usuário.
     */
    std::map<std::string, std::string> const forcaCustomFiles = {};

    /**
     * Mapeamento de identificadores para qualquer arquivo do sistema, independentemente da pasta.
     *
     * Este mapa pode conter arquivos presentes em qualquer local relevante para o sistema,
     * seja na pasta default, custom ou outros diretórios. Útil para buscas e operações genéricas.
     */
    std::map<std::string, std::string> const forcaAnyFiles = {};

    namespace utils {

        std::string normalizePath( std::string path );

        bool fileExist( std::string filepath );

        bool canRead( std::string filepath );

        bool canWrite( std::string filepath );

        std::string getPath( std::string key, std::string type = "DEFAULT" );

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

        bool validateEssentialFiles();

    }

}

#endif