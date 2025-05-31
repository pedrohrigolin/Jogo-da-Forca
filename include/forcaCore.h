#ifndef FORCA_CORE_H
#define FORCA_CORE_H

#include <vector>
#include <string>
#include <cstring>

namespace forcaCore {

    struct words_levels {
        std::vector<std::string> easy;
        std::vector<std::string> normal;
        std::vector<std::string> hard;
    };

    /*
        Porque eu usei a estrutura words que simplesmente chama words_levels ao invés de criar
        só uma estrutura? Pois isso possibilita futuras implementações sem quebrar o código, por exemplo,
        eu poderia criar categorias, daí eu crio uma struct word_categorias e referencio ela aqui em words_game.
    */
    struct words_game {
        forcaCore::words_levels levels;
    };

    extern words_game words;

    // ? PARA PALAVRAS DIVIDIDAS EM CATEGORIAS, PROVAVELMENTE TERIA QUE USAR ALGUM MAPA

}

#endif