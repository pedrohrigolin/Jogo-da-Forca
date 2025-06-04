// Lib para regex. PCRE2 É O MESMO MOTOR REGEX USADO NO PHP.
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <cstring>
#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include "forcaRegex.h"

#include <iostream> // !! TIRAR DEPOIS DE CONCLUIR TODOS OS TESTES

/**
 * Namespace que implementa funcionalidades de expressões regulares inspiradas no PHP.
 * Oferece uma interface familiar para usar regex no C++, seguindo o padrão do PHP.
 * 
 * As funções deste namespace utilizam a biblioteca PCRE2 (mesmo motor usado pelo PHP)
 * para realizar operações de busca e substituição em strings, mantendo compatibilidade
 * com a sintaxe e comportamento do PHP.
 * 
 * Características:
 *   - Usa a mesma sintaxe de delimitadores do PHP (/ ou #)
 *   - Suporta as mesmas flags de modificadores (i, m, s, u, x, U)
 *   - Mantém compatibilidade com padrões regex do PHP
 *   - Otimizado para operações em strings grandes
 *   - Usa PCRE2 como motor regex (mesmo do PHP)
 * 
 * @namespace   forcaRegex
 * @see         https://www.php.net/manual/pt_BR/book.pcre.php
 */
namespace forcaRegex {

    /**
     * Cria e compila um padrão de expressão regular no estilo PHP.
     * Aceita delimitadores '/' ou '#' e suporta as flags: i, m, s, u, x, U.
     * 
     * Exemplo de padrão: "/regex/im" ou "#regex#u"
     * 
     * Flags suportadas:
     *   i - PCRE2_CASELESS  - Case insensitive
     *   m - PCRE2_MULTILINE - Multiline mode
     *   s - PCRE2_DOTALL    - Dot matches all
     *   u - PCRE2_UTF       - UTF-8 mode
     *   x - PCRE2_EXTENDED  - Extended mode
     *   U - PCRE2_UNGREEDY  - Ungreedy mode
     * 
     * @param   const std::string& pattern    String contendo o padrão regex no formato /pattern/flags
     * @return  RegexPattern                  Estrutura contendo o padrão compilado e suas configurações
     * @throws  std::invalid_argument         Se o padrão regex estiver malformado
     * @throws  std::runtime_error           Se houver erro na compilação do padrão
     */
    forcaRegex::RegexPattern createPattern( const std::string& pattern ) {

        std::string cpypattern = pattern;

        // Removendo espaços a esquerda.
        while( isspace(cpypattern[0]) && cpypattern.length() != 0 ){
        
            cpypattern.erase(0, 1);

        }

        // Removendo espaços a direita.
        while( isspace( cpypattern[ cpypattern.length() - 1 ] ) && cpypattern.length() > 0 ){
        
            cpypattern.erase( (cpypattern.length() - 1), 1 );

        }

        std::string::size_type length = cpypattern.length();

        // Verifica se o regex pattern tem ao menos 3 caracteres.
        if( length < 3 ){

            throw std::invalid_argument(
                "Regex pattern malformado. O padrão deve conter no mínimo 3 caracteres: "
                "um delimitador de abertura ('/' ou '#'), "
                "uma regra regex, e "
                "um delimitador de fechamento (mesmo caractere usado na abertura)."
            );

        }

        // Verifica se o primeiro caracter do regex pattern é um delimitador aceito.
        if( cpypattern[0] != '/' && cpypattern[0] != '#' ){

            throw std::invalid_argument(
                "Regex pattern malformado. O padrão deve conter no mínimo 3 caracteres: "
                "um delimitador de abertura ('/' ou '#'), "
                "uma regra regex, e "
                "um delimitador de fechamento (mesmo caractere usado na abertura)."
            );            

        }

        std::string::size_type lastDelimeter = cpypattern.find_last_of(cpypattern[0]);

        // Verifica se o regex pattern tem delimitador de fechamento.
        // A comparação para ver se o lastDelimeter é menor que 2, garante que o delimitador encontrado
        // não é o de abertura e ainda garante novamente que a string tem ao menos 3 caracteres.
        if( lastDelimeter == std::string::npos || lastDelimeter < 2 ){

            throw std::invalid_argument(
                "Regex pattern malformado. O padrão deve conter no mínimo 3 caracteres: "
                "um delimitador de abertura ('/' ou '#'), "
                "uma regra regex, e "
                "um delimitador de fechamento (mesmo caractere usado na abertura)."
            );

        }

        // Extrai a expressão regular.
        std::string expression = cpypattern.substr(1, lastDelimeter - 1);

        // Calcula o tamanho da expressão regular.
        std::string::size_type expressionLength = lastDelimeter - 1;

        // Extrai as flags. Se não houver, recebe uma string vazia.
        std::string flags = cpypattern.substr( lastDelimeter + 1, length );

        static const std::map<char, uint32_t> PHP_PCRE2_FLAG_MAP = {
            { 'i', PCRE2_CASELESS },
            { 'm', PCRE2_MULTILINE },
            { 's', PCRE2_DOTALL },
            { 'u', PCRE2_UTF },
            { 'x', PCRE2_EXTENDED },
            { 'U', PCRE2_UNGREEDY }
        };

        uint32_t options = 0;

        // Determina as flags que serão usadas.
        if( flags.length() != 0 ){

            std::string::size_type i, length = flags.length();

            for(i=0; i<length; i++){

                std::map<char, uint32_t>::const_iterator flagKey = PHP_PCRE2_FLAG_MAP.find( flags[i] );

                if( flagKey != PHP_PCRE2_FLAG_MAP.end() ){

                    options |= flagKey->second;

                }

            }

        }

        forcaRegex::RegexPattern finalPattern;

        finalPattern.expression = expression;

        finalPattern.pattern = reinterpret_cast<PCRE2_SPTR>( finalPattern.expression.data() );

        finalPattern.length = static_cast<PCRE2_SIZE>( expressionLength );

        finalPattern.options = options;

        finalPattern.compiled.code = pcre2_compile( finalPattern.pattern, finalPattern.length, finalPattern.options, 
                                                    &finalPattern.compiled.error_code, &finalPattern.compiled.error_offset, 
                                                    finalPattern.compiled.context );

        
        if( finalPattern.compiled.code == nullptr ){

            PCRE2_UCHAR buffer[256];

            int result = pcre2_get_error_message(finalPattern.compiled.error_code, buffer, sizeof(buffer));

            std::string message;

            if (result < 0) {
                message = "Erro desconhecido ao compilar regex. Código: " + std::to_string(finalPattern.compiled.error_code);
            } else {
                message = "Erro ao compilar regex: ";
                message += reinterpret_cast<char*>(buffer);
                message += " (na posição ";
                message += std::to_string(finalPattern.compiled.error_offset);
                message += ")";
            }

            throw std::runtime_error(message);

        }

        return finalPattern;

    }

    bool preg_match( std::string pattern, std::string subject ) {

        RegexPattern finalPattern = createPattern(pattern);

    }

}