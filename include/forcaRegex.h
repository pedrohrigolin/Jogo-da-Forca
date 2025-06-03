#ifndef FORCA_REGEX_H
#define FORCA_REGEX_H

#include <pcre2.h>
#include <string>
#include <vector>

namespace forcaRegex {

    struct RegexPattern {
        
        std::string expression;
        PCRE2_SPTR pattern;
        PCRE2_SIZE length;
        uint32_t options;
        
        struct RegexCompile{

            int error_code = 0;
            PCRE2_SIZE error_offset = 0;
            pcre2_compile_context *context = nullptr;
            pcre2_code *code;

            ~RegexCompile(){

                if( code != nullptr ) pcre2_code_free(code);
                
                if( context != nullptr ) pcre2_compile_context_free(context);

            }

        } compiled;

    };

    forcaRegex::RegexPattern createPattern( const std::string& pattern );

}

#endif