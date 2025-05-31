#include <iostream>
#include <map>
#include <stdexcept>
#include "forcaStrings.h"
#include "forcaUtils.h"
#include "forcaFiles.h"
#include "forcaInitialize.h"
#include "forcaCore.h"

namespace forcaInitialize {

    // Adiciona as palavras por nivel a struct words.
    bool createWordsStructLevels(){

        std::string path = forcaFiles::utils::getPath("WORDS_LEVELS_EASY");

        std::string content = forcaFiles::read::getContent(path);

        std::cout<<path<<std::endl;

        return true;

    }

    bool createWordsStruct(){


        return true;

    }

    bool init() {

        forcaUtils::clear_screen();

        std::cout<<std::endl<<std::endl<<"Iniciando o jogo da forca..."<<std::endl;

        return true;

    }

}