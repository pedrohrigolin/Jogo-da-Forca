#include <iostream>
#include <map> // ? TALVEZ DE PARA REMOVER DEPOIS
#include <stdexcept>
#include "forcaStrings.h"
#include "forcaUtils.h"
#include "forcaFiles.h"
#include "forcaInitialize.h"
#include "forcaCore.h"

namespace forcaInitialize {

    // Adiciona as palavras por nivel a struct words.
    bool createWordsStructLeves(){

        // std::string contentEasy = forcaFiles::read::getContent( forcaFiles::forcaCustomPath + forcaFiles::forcaPaths["WORDS_LEVELS_EASY"] );

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