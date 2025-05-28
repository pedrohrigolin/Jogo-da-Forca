#include <cstdlib>
#include <string>
#include "forcaUtils.h"

namespace forcaUtils {

    /**
     * Limpa a tela do terminal de acordo com o sistema operacional.
     *
     * Utiliza o comando "cls" para sistemas Windows e "clear" para sistemas Unix-like.
     * A escolha do comando é feita em tempo de compilação usando diretivas de pré-processador.
     * É um código simples e bem conhecido.
     */
    void clear_screen(){
        
        #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            std::system("cls");
        #else
            std::system("clear");
        #endif
        
    }

}