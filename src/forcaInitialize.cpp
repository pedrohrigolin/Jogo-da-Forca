#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include "forcaStrings.h"
#include "forcaUtils.h"
#include "forcaFiles.h"
#include "forcaInitialize.h"
#include "forcaCore.h"

namespace forcaInitialize {

    /**
     * @brief Lê os arquivos de palavras por nível e preenche a estrutura words do jogo.
     *
     * Esta função busca os arquivos de palavras para cada nível de dificuldade (easy, normal, hard),
     * lê o conteúdo de cada arquivo, separa as palavras por linha e armazena nos respectivos vetores
     * da estrutura forcaCore::words.levels.
     *
     * Utiliza a função explode para dividir o conteúdo dos arquivos em linhas.
     *
     * @return true se todas as listas de palavras foram carregadas com sucesso.
     */
    bool createWordsStructLevels(){

        std::string path = forcaFiles::utils::getPath("WORDS_LEVELS_EASY", "CUSTOM");

        std::string content = forcaFiles::read::getContent(path);

        std::vector<std::string> explode;

        forcaUtils::explode(content, "\n", &explode);

        std::size_t i, size = explode.size();

        for(i=0; i<size; i++){

            forcaCore::words.levels.easy.push_back(explode[i]);

        }

        explode.clear();

        path = forcaFiles::utils::getPath("WORDS_LEVELS_NORMAL", "CUSTOM");

        content = forcaFiles::read::getContent(path);

        forcaUtils::explode(content, "\n", &explode);

        size = explode.size();

        for(i=0; i<size; i++){

            forcaCore::words.levels.normal.push_back(explode[i]);

        }

        explode.clear();

        path = forcaFiles::utils::getPath("WORDS_LEVELS_HARD", "CUSTOM");

        content = forcaFiles::read::getContent(path);

        forcaUtils::explode(content, "\n", &explode);

        size = explode.size();

        for(i=0; i<size; i++){

            forcaCore::words.levels.hard.push_back(explode[i]);

        }

        explode.clear();

        return true;

    }

    bool createWordsStruct(){


        return true;

    }

    bool init() {

        forcaUtils::clear_screen();

        std::cout<<std::endl<<std::endl<<"Iniciando o jogo da forca..."<<std::endl;

        std::cout<<std::endl<<std::endl<<"Verificando a integridade dos arquivos..."<<std::endl;

        return true;

    }

}