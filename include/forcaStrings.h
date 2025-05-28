#ifndef FORCA_STRINGS_H
#define FORCA_STRINGS_H

namespace forcaStrings {

    std::string removeSpaces( std::string text );

    std::string removeAcentos( std::string text );

    std::string to_uppercase( std::string text );

    std::string normalizeWord( std::string text );

    bool checkAlphaCharacters( std::string word );

}

#endif