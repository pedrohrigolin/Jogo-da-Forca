#ifndef FORCA_UTILS_H
#define FORCA_UTILS_H

#include <string>

namespace forcaUtils {

    /**
     * Define uma constante de string indicando o sistema operacional em uso.
     * 
     * Se o código estiver sendo compilado em um sistema Windows (_WIN32, _WIN64 ou __CYGWIN__),
     * define forcaUtils::OS como "WINDOWS". Caso contrário, define como "UNIX".
     * 
     * Isso permite que o restante do código adapte comportamentos específicos
     * de acordo com o sistema operacional detectado em tempo de compilação.
     */
    #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
        std::string const OS = "WINDOWS";
    #else
        std::string const OS = "UNIX";
    #endif

    void clear_screen();

    bool filter_validate_bool( std::string value, bool strict = false );

    bool filter_validate_bool( bool value, bool strict = false );

    bool filter_validate_bool( int value, bool strict = false );

}

#endif