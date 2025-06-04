#ifndef FORCA_STRINGS_H
#define FORCA_STRINGS_H

#include <string>
#include <vector>

namespace forcaStrings {

    std::string removeSpaces( const std::string& string );

    std::string removeAcentos( const std::string& string );

    std::string to_uppercase( const std::string& string );

    std::string to_lowercase( const std::string& string );

    std::string trim( const std::string& string );

    std::string ltrim( const std::string& string );

    std::string rtrim( const std::string& string );

    std::string normalizeWord( const std::string& string );

    bool checkAlphaCharacters( const std::string& word );

    void explode( const std::string& string, std::string separator, std::vector<std::string>* reference );

    void explode( char* string, std::string separator, std::vector<std::string>* reference );

    void explode( const char* string, std::string separator, std::vector<std::string>* reference );

    std::string implode( const std::vector<std::string>& array, std::string delimeter );

    std::string implode( char* array[], int size, std::string delimiter );

    std::string implode( const char* array[], int size, std::string delimiter );

}

#endif