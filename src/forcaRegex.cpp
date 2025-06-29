// Lib para regex. PCRE2 É O MESMO MOTOR REGEX USADO NO PHP.
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <cstring>
#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <memory> // Para gerenciamento de memória com unique_ptr
#include "forcaRegex.h"

/**
 * Namespace que implementa funcionalidades de expressões regulares inspiradas no PHP.
 * Oferece uma interface familiar para usar regex no C++, seguindo o padrão do PHP.
 * * As funções deste namespace utilizam a biblioteca PCRE2 (mesmo motor usado pelo PHP)
 * para realizar operações de busca e substituição em strings, mantendo compatibilidade
 * com a sintaxe e comportamento do PHP.
 * * Características:
 * - Usa a mesma sintaxe de delimitadores do PHP (/ ou #)
 * - Suporta as mesmas flags de modificadores (i, m, s, u, x, U)
 * - Mantém compatibilidade com padrões regex do PHP
 * - Otimizado para operações em strings grandes
 * - Usa PCRE2 como motor regex (mesmo do PHP)
 * * @namespace   forcaRegex
 * @see         https://www.php.net/manual/pt_BR/book.pcre.php
 */
namespace forcaRegex {

    /**
     * Cria e compila um padrão de expressão regular no estilo PHP.
     * Aceita delimitadores '/' ou '#' e suporta as flags: i, m, s, u, x, U.
     * * Exemplo de padrão: "/regex/im" ou "#regex#u"
     * * Flags suportadas:
     * i - PCRE2_CASELESS  - Case insensitive
     * m - PCRE2_MULTILINE - Multiline mode
     * s - PCRE2_DOTALL    - Dot matches all
     * u - PCRE2_UTF       - UTF-8 mode
     * x - PCRE2_EXTENDED  - Extended mode
     * U - PCRE2_UNGREEDY  - Ungreedy mode
     * * @param   const std::string& pattern    String contendo o padrão regex no formato /pattern/flags
     * @return  RegexPattern                  Estrutura contendo o padrão compilado e suas configurações
     * @throws  std::invalid_argument         Se o padrão regex estiver malformado
     * @throws  std::runtime_error           Se houver erro na compilação do padrão
     */
    std::unique_ptr<forcaRegex::RegexPattern> createPattern( const std::string& pattern ) {

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

        /*         
            Verifica se o regex pattern tem delimitador de fechamento.
            A comparação para ver se o lastDelimeter é menor que 2, garante que o delimitador encontrado
            não é o de abertura e ainda garante novamente que a string tem ao menos 3 caracteres.
         */
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

        std::unique_ptr<forcaRegex::RegexPattern> finalPattern = std::make_unique<forcaRegex::RegexPattern>();

        finalPattern->expression = expression;
        finalPattern->pattern = reinterpret_cast<PCRE2_SPTR>( finalPattern->expression.c_str() );
        finalPattern->length = static_cast<PCRE2_SIZE>( expressionLength );
        finalPattern->options = options;

        finalPattern->compiled.code = pcre2_compile( finalPattern->pattern, finalPattern->length, finalPattern->options, 
                                                    &finalPattern->compiled.error_code, &finalPattern->compiled.error_offset, 
                                                    finalPattern->compiled.context );

        
        if( finalPattern->compiled.code == nullptr ){

            PCRE2_UCHAR buffer[256];

            int result = pcre2_get_error_message(finalPattern->compiled.error_code, buffer, sizeof(buffer));

            std::string message;

            if (result < 0) {
                message = "Erro desconhecido ao compilar regex. Código: " + std::to_string(finalPattern->compiled.error_code);
            } else {
                message = "Erro ao compilar regex: ";
                message += reinterpret_cast<char*>(buffer);
                message += " (na posição ";
                message += std::to_string(finalPattern->compiled.error_offset);
                message += ")";
            }

            throw std::runtime_error(message);

        }

        return finalPattern;

    }

    /**
     * Executa uma busca por um padrão em uma string (primeira ocorrência).
     * Similar à função preg_match() do PHP.
     * * @param   const std::string& pattern    Padrão regex no formato /pattern/flags
     * @param   const std::string& subject    String onde será feita a busca
     * @param   PCRE2_SIZE offset            Posição onde iniciar a busca (default: 0)
     * @return  RegexResult                   Estrutura contendo os resultados da busca
     * - match: true se encontrou match, false caso contrário
     * - Grupos numéricos e nomeados acessíveis via get()
     * @throws  std::invalid_argument         Se o padrão regex estiver malformado
     * @throws  std::runtime_error           Se houver erro na compilação do padrão
     */
    forcaRegex::RegexResult preg_match( const std::string& pattern, const std::string& subject, PCRE2_SIZE offset ) {

        forcaRegex::RegexResult finalResult;

        finalResult.match = false;

        if( offset >= subject.length() || subject.empty() ) return finalResult;

        std::unique_ptr<forcaRegex::RegexPattern> finalPattern_ptr = createPattern(pattern);

        RegexMatchData regex_data;

        regex_data.match_data = pcre2_match_data_create_from_pattern( finalPattern_ptr->compiled.code, regex_data.gcontext );

        PCRE2_SPTR subject_string = reinterpret_cast<PCRE2_SPTR>( subject.data() );

        PCRE2_SIZE subject_length = static_cast<PCRE2_SIZE>( subject.length() );

        pcre2_match_context *mcontext = nullptr;

        int regex_result = pcre2_match( finalPattern_ptr->compiled.code, subject_string, subject_length, offset,
        0, regex_data.match_data, mcontext );

        // Se não houve match, limpa e retorna
        if (regex_result <= 0) {

            finalResult.match = false;

            return finalResult;

        }

        // Armazena os grupos numéricos
        PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(regex_data.match_data);

        for (int i = 0; i < regex_result; ++i) {

            PCRE2_SIZE start = ovector[2 * i];
            PCRE2_SIZE end = ovector[2 * i + 1];

            std::string match = subject.substr(start, end - start);

            finalResult.push(start, end, match, i);

        }

        // Armazena os grupos nomeados
        PCRE2_SPTR name_table;
        uint32_t namecount;
        uint32_t name_entry_size;

        pcre2_pattern_info(finalPattern_ptr->compiled.code, PCRE2_INFO_NAMECOUNT, &namecount);

        if (namecount > 0) {

            pcre2_pattern_info(finalPattern_ptr->compiled.code, PCRE2_INFO_NAMETABLE, &name_table);
            pcre2_pattern_info(finalPattern_ptr->compiled.code, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);

            PCRE2_SPTR entry = name_table;

            for (uint32_t i = 0; i < namecount; ++i) {

                uint16_t group_number = (entry[0] << 8) | entry[1];
                std::string group_name(reinterpret_cast<const char*>(entry + 2));

                PCRE2_UCHAR* substr;
                PCRE2_SIZE substr_length;

                int res = pcre2_substring_get_bynumber(regex_data.match_data, group_number, &substr, &substr_length);

                if (res == 0) {

                    std::string match(reinterpret_cast<char*>(substr), substr_length);

                    PCRE2_SIZE start = ovector[2 * group_number];
                    PCRE2_SIZE end = ovector[2 * group_number + 1];

                    finalResult.push(start, end, match, group_name);

                    pcre2_substring_free(substr);

                }

                entry += name_entry_size;
            }

        }

        if( mcontext != nullptr ) pcre2_match_context_free(mcontext);

        finalResult.match = true;

        return finalResult;

    }

    /**
     * Executa uma busca por um padrão em uma string (todas ocorrências).
     * Similar à função preg_match_all() do PHP.
     *
     * @param pattern  Padrão regex no formato /pattern/flags.
     * @param subject  String onde será feita a busca.
     * @param offset   (Opcional) Posição onde iniciar a busca (default: 0).
     * @param limit    (Opcional) Limite máximo de matches a serem encontrados (default: ilimitado).
     * @return RegexResult Estrutura contendo os resultados da busca:
     *         - match: true se encontrou matches, false caso contrário.
     *         - Grupos numéricos e nomeados acessíveis via get().
     * @throws std::invalid_argument Se o padrão regex estiver malformado.
     * @throws std::runtime_error Se houver erro na compilação do padrão.
     */
    forcaRegex::RegexResult preg_match_all( const std::string& pattern, const std::string& subject, PCRE2_SIZE offset, std::size_t limit ) {

        forcaRegex::RegexResult finalResult;

        finalResult.match = false;

        if ( offset >= subject.length() || limit == 0 || subject.empty() ) return finalResult;

        std::unique_ptr<forcaRegex::RegexPattern> finalPattern_ptr = createPattern(pattern);

        RegexMatchData regex_data;

        regex_data.match_data = pcre2_match_data_create_from_pattern(finalPattern_ptr->compiled.code, regex_data.gcontext);

        PCRE2_SPTR subject_string = reinterpret_cast<PCRE2_SPTR>(subject.data());
        PCRE2_SIZE subject_length = static_cast<PCRE2_SIZE>(subject.length());

        pcre2_match_context *mcontext = nullptr;

        bool foundAny = false;

        std::size_t count = 0;

        while (offset < subject_length) {
            
            if (limit == 0 || count >= limit) {
                break;
            }

            int regex_result = pcre2_match(
                finalPattern_ptr->compiled.code,
                subject_string,
                subject_length,
                offset,
                0,
                regex_data.match_data,
                mcontext
            );

            if (regex_result <= 0) break;

            foundAny = true;

            // Grupos numéricos
            PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(regex_data.match_data);

            for (int i = 0; i < regex_result; ++i) {

                PCRE2_SIZE start = ovector[2 * i];
                PCRE2_SIZE end = ovector[2 * i + 1];

                std::string match = subject.substr(start, end - start);

                finalResult.push(start, end, match, i);

            }

            // Grupos nomeados
            PCRE2_SPTR name_table;
            uint32_t namecount;
            uint32_t name_entry_size;

            pcre2_pattern_info(finalPattern_ptr->compiled.code, PCRE2_INFO_NAMECOUNT, &namecount);

            if (namecount > 0) {

                pcre2_pattern_info(finalPattern_ptr->compiled.code, PCRE2_INFO_NAMETABLE, &name_table);
                pcre2_pattern_info(finalPattern_ptr->compiled.code, PCRE2_INFO_NAMEENTRYSIZE, &name_entry_size);

                PCRE2_SPTR entry = name_table;

                for (uint32_t i = 0; i < namecount; ++i) {

                    uint16_t group_number = (entry[0] << 8) | entry[1];

                    std::string group_name(reinterpret_cast<const char*>(entry + 2));

                    PCRE2_UCHAR* substr;
                    PCRE2_SIZE substr_length;

                    int res = pcre2_substring_get_bynumber(regex_data.match_data, group_number, &substr, &substr_length);
                    
                    if (res == 0) {

                        std::string match(reinterpret_cast<char*>(substr), substr_length);

                        PCRE2_SIZE start = ovector[2 * group_number];
                        PCRE2_SIZE end = ovector[2 * group_number + 1];

                        finalResult.push(start, end, match, group_name);

                        pcre2_substring_free(substr);

                    }

                    entry += name_entry_size;

                }

            }

            // Atualiza offset
            PCRE2_SIZE new_offset = ovector[1];
            
            if (new_offset <= offset) offset++; // evita loop infinito com zero-width matches
            else offset = new_offset;

            count++;

        }

        if( mcontext != nullptr ) pcre2_match_context_free(mcontext);

        finalResult.match = foundAny;

        return finalResult;

    }

    /**
     * Realiza substituições em uma string baseadas em um padrão regex.
     * Similar à função preg_replace() do PHP.
     *
     * Suporta referências para grupos capturados no texto de substituição:
     * - $0: Match completo
     * - $1, $2, etc: Grupos numéricos
     * - $name: Grupos nomeados
     *
     * @param pattern      Padrão regex no formato /pattern/flags.
     * @param subject      String onde serão feitas as substituições.
     * @param replacement  String de substituição (pode conter $0, $1, $name etc).
     * @param offset       (Opcional) Posição onde iniciar as substituições (default: 0).
     * @param limit        (Opcional) Limite máximo de substituições a serem realizadas (default: ilimitado).
     * @return std::string Nova string com as substituições realizadas.
     * @throws std::invalid_argument Se o padrão regex estiver malformado.
     * @throws std::runtime_error Se houver erro na compilação do padrão.
     */
    std::string preg_replace( const std::string& pattern, const std::string& subject, const std::string& replacement, PCRE2_SIZE offset, std::size_t limit ) {

        if(subject.empty() || limit == 0) return subject;

        RegexResult result;

        // Otimização para quando limit for igual a 1, usando o preg_match que é mais rápido.
        if(limit == 1){
            result = preg_match(pattern, subject, offset);
        }
        else{
            result = preg_match_all(pattern, subject, offset, limit);
        }

        if (!result.match) return subject;

        std::string output;
        std::string::size_type last_pos = 0;

        std::vector<forcaRegex::RegexStructData>* full_matches = result.get(0);

        if ( full_matches == nullptr ) return subject;

        std::size_t count = 0;

        for (const auto& match_data : *full_matches) {

            if (limit == 0 || count >= limit) {
                break;
            }

            // Adiciona parte do subject entre o último match e o início do atual
            output += subject.substr(last_pos, match_data.start - last_pos);

            // Constrói substituição
            std::string replaced = replacement;

            // Substituição de grupos posicionais: $0, $1, ...
            for (int group_id : result.int_keys) {

                auto group = result.get(group_id);

                if (!group || group->empty()) continue;

                std::string tag = "$" + std::to_string(group_id);

                std::string::size_type pos = 0;

                while ((pos = replaced.find(tag, pos)) != std::string::npos) {

                    replaced.replace(pos, tag.size(), (*group)[&match_data - &(*full_matches)[0]].match);
                    pos += (*group)[&match_data - &(*full_matches)[0]].match.size();

                }

            }

            // Substituição de grupos nomeados: $name, $email, etc.
            for (const std::string& name : result.string_keys) {

                auto group = result.get(name);

                if (!group || group->empty()) continue;

                std::string tag = "$" + name;

                std::string::size_type pos = 0;

                while ((pos = replaced.find(tag, pos)) != std::string::npos) {

                    replaced.replace(pos, tag.size(), (*group)[&match_data - &(*full_matches)[0]].match);
                    pos += (*group)[&match_data - &(*full_matches)[0]].match.size();

                }

            }

            output += replaced;
            last_pos = match_data.end;
            count++;

        }

        // Adiciona o restante do subject após o último match
        output += subject.substr(last_pos);
        return output;

    }

    /**
     * Divide uma string em substrings usando um padrão regex como delimitador.
     * Similar à função preg_split() do PHP.
     *
     * @param pattern  Padrão regex usado como delimitador.
     * @param subject  String a ser dividida.
     * @param limit    (Opcional) Limite máximo de divisões (default: ilimitado).
     * @return std::vector<std::string> Vetor contendo as substrings resultantes.
     */
    std::vector<std::string> preg_split( const std::string& pattern, const std::string& subject, std::size_t limit ) {

        if(subject.empty() || limit == 0) return std::vector<std::string>{subject};

        RegexResult result;

        // Otimização para quando limit for igual a 1, usando o preg_match que é mais rápido.
        if(limit == 1){
            result = preg_match(pattern, subject, 0);
        }
        else{
            result = preg_match_all(pattern, subject, 0, limit);
        }

        if (!result.match) return std::vector<std::string>{subject};

        std::vector<RegexStructData>* complete_matches = result.get(0);

        if( complete_matches == nullptr ) return std::vector<std::string>{subject};

        std::size_t i, matchesLength = complete_matches->size();

        std::vector<std::string> explode;

        std::string copy = subject;

        std::string::size_type offset = 0;

        for(i=0; i<matchesLength; i++){

            if(i >= limit || offset >= copy.length()) break;

            if(i + 1 < limit){

                std::string::size_type start = complete_matches->at(i).start;
    
                std::string::size_type end = complete_matches->at(i).end;

                std::string substring = copy.substr( offset, (start - offset) );

                explode.push_back(substring);

                offset = end;

            }
            else{
                explode.push_back( copy.substr(offset) );
            }

        }

        if(explode.empty()) explode.push_back(copy);

        return explode;

    }

}