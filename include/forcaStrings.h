#ifndef FORCA_STRINGS_H
#define FORCA_STRINGS_H

#include <string>
#include <vector>

namespace forcaStrings {

    std::string removeSpaces( std::string text );

    std::string removeAcentos( std::string text );

    std::string to_uppercase( std::string text );

    std::string to_lowercase( std::string text );

    std::string trim( const std::string& string );

    std::string ltrim( const std::string& string );

    std::string rtrim( const std::string& string );

    std::string normalizeWord( std::string text );

    bool checkAlphaCharacters( std::string word );

    void explode( const std::string& string, std::string separator, std::vector<std::string>* reference );

    void explode( char* string, std::string separator, std::vector<std::string>* reference );

    void explode( const char* string, std::string separator, std::vector<std::string>* reference );

    std::string implode( const std::vector<std::string>& array, std::string delimeter );

    std::string implode( char* array[], int size, std::string delimiter );

    std::string implode( const char* array[], int size, std::string delimiter );

}

#endif