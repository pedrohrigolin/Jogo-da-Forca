#ifndef FORCA_STRINGS_H
#define FORCA_STRINGS_H

#include <string>
#include <vector>
#include <limits>

namespace forcaStrings {

    std::string removeSpaces( const std::string& string );

    std::string removeAcentos( const std::string& string );

    std::string to_uppercase( const std::string& string );

    std::string to_lowercase( const std::string& string );

    std::string trim( const std::string& string );

    std::string ltrim( const std::string& string );

    std::string rtrim( const std::string& string );

    std::string normalizeWord( const std::string& string );

    std::string normalizeLineBreaks( const std::string& text );

    std::string removeExtraLineBreaks( const std::string& text, bool normalize = true );

    bool checkAlphaCharacters( const std::string& word );

    std::vector<std::string> explodeGraphemes( const std::string& string );

    std::string::size_type Length( const std::string& string );

    std::string charAt( const std::string& string, std::string::size_type index = 0 );

    std::string repeat( const std::string& string, std::string::size_type count = 0 );

    std::string substring( const std::string& string, std::size_t pos = std::numeric_limits<size_t>::max(), std::size_t len = std::numeric_limits<size_t>::max() );

    std::string::size_type firstIndexOf( const std::string& string, const std::string& search );

    std::string::size_type lastIndexOf( const std::string& string, const std::string& search );

    void explode( const std::string& string, const std::string& separator, std::vector<std::string>* reference, std::size_t limit = std::numeric_limits<size_t>::max() );

    void explode( char* string, const std::string& separator, std::vector<std::string>* reference, std::size_t limit = std::numeric_limits<size_t>::max() );

    void explode( const char* string, const std::string& separator, std::vector<std::string>* reference, std::size_t limit = std::numeric_limits<size_t>::max() );

    std::string implode( const std::vector<std::string>& array, const std::string& delimeter = "" );

    std::string implode( char* array[], int size, const std::string& delimiter = "" );

    std::string implode( const char* array[], int size, const std::string& delimiter = "" );

}

#endif