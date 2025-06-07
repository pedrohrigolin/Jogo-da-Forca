#ifndef FORCA_REGEX_H
#define FORCA_REGEX_H

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm> // Para usar o std::find

namespace forcaRegex {

    /**
     * Estrutura que mantém o padrão regex compilado e suas configurações.
     * Armazena a expressão original, flags e o código compilado pelo PCRE2.
     * 
     * @struct  RegexPattern
     * @member  expression   String contendo o padrão regex sem delimitadores/flags
     * @member  pattern      Ponteiro PCRE2 para o padrão
     * @member  length      Tamanho do padrão em bytes
     * @member  options     Flags de compilação (PCRE2_CASELESS, PCRE2_MULTILINE, etc)
     * @member  compiled    Sub-estrutura que mantém o código compilado e informações de erro
     */
    struct RegexPattern {
        
        std::string expression;
        PCRE2_SPTR pattern;
        PCRE2_SIZE length;
        uint32_t options;
        
        struct RegexCompile {

            int error_code = 0;
            PCRE2_SIZE error_offset = 0;
            pcre2_compile_context *context = nullptr;
            pcre2_code *code;

            // Destrutor para garantir a liberação da memória.
            ~RegexCompile() {

                if( code != nullptr ) pcre2_code_free(code);
                
                if( context != nullptr ) pcre2_compile_context_free(context);

            }

        } compiled;

    };

    /**
     * Estrutura que gerencia os dados de match do PCRE2.
     * Responsável por alocar e liberar a memória dos matches.
     * 
     * @struct  RegexMatchData
     * @member  gcontext     Contexto geral do PCRE2 para alocação
     * @member  match_data   Dados dos matches encontrados
     */
    struct RegexMatchData {

        pcre2_general_context *gcontext = nullptr;
        
        pcre2_match_data *match_data = nullptr;

        // Destrutor para garantir a liberação da memória.
        ~RegexMatchData() {

            if( match_data != nullptr ) pcre2_match_data_free(match_data);

            if( gcontext != nullptr ) pcre2_general_context_free(gcontext);

        }

    };
    
    /**
     * Estrutura que armazena informações sobre um match específico.
     * Mantém a posição inicial, final e o texto encontrado.
     * 
     * @struct  RegexStructData
     * @member  start   Posição inicial do match na string original
     * @member  end     Posição final do match na string original
     * @member  match   String contendo o texto encontrado
     */
    struct RegexStructData {

        PCRE2_SIZE start;
        PCRE2_SIZE end;
        std::string match;

    };

    /**
     * Estrutura principal que armazena os resultados de uma busca regex.
     * Mantém grupos numéricos e nomeados, permitindo acesso via índice ou nome.
     * 
     * Membros públicos:
     * - match: indica se houve match
     * - int_keys: lista de grupos numéricos disponíveis
     * - string_keys: lista de grupos nomeados disponíveis
     * 
     * Métodos principais:
     * - key_exist(): verifica existência de um grupo
     * - push(): adiciona um novo match
     * - get(): obtém matches de um grupo
     * 
     * @struct  RegexResult
     * @method  key_exist   Verifica se um grupo existe (por nome ou número)
     * @method  push        Adiciona um novo match a um grupo
     * @method  get         Obtém os matches de um grupo específico
     * @member  match       Flag indicando se houve match
     * @member  int_keys    Vetor com índices dos grupos numéricos
     * @member  string_keys Vetor com nomes dos grupos nomeados
     */
    struct RegexResult {

        private:

            std::vector<std::vector<forcaRegex::RegexStructData>> numberGroups;

            std::map<std::string, std::vector<forcaRegex::RegexStructData>> namedGroups;

            std::string trim( const std::string& string ){

                std::string newString = string;

                // Removendo espaços a esquerda.
                while( isspace(newString[0]) && newString.length() != 0 ){
                
                    newString.erase(0, 1);

                }

                // Removendo espaços a direita.
                while( isspace( newString[ newString.length() - 1 ] ) && newString.length() > 0 ){
                
                    newString.erase( (newString.length() - 1), 1 );

                }

                return newString;

            }

        public:

            std::vector<int> int_keys;

            std::vector<std::string> string_keys;

            bool match = false;

            bool key_exist( const std::string& key ){

                std::string newKey = forcaRegex::RegexResult::trim(key);

                std::map<std::string, std::vector<forcaRegex::RegexStructData>>::iterator pos = forcaRegex::RegexResult::namedGroups.find(newKey);

                return pos != forcaRegex::RegexResult::namedGroups.end();

            };

            bool key_exist( int key ){

                return key >= 0 && key < static_cast<int>(numberGroups.size());

            };

            void push( PCRE2_SIZE start, PCRE2_SIZE end, std::string match, int key ){

                forcaRegex::RegexStructData data = {start, end, match};

                if( key_exist(key) ){

                    forcaRegex::RegexResult::numberGroups[key].push_back(data);

                }
                else{

                    std::vector<forcaRegex::RegexStructData> newPush;

                    newPush.push_back(data);

                    forcaRegex::RegexResult::numberGroups.push_back(newPush);

                    forcaRegex::RegexResult::int_keys.push_back( numberGroups.size() - 1 );

                }

            };

            void push( PCRE2_SIZE start, PCRE2_SIZE end, std::string match, const std::string& key ){

                std::string newKey = forcaRegex::RegexResult::trim(key);

                forcaRegex::RegexStructData data = {start, end, match};

                if( key_exist(key) ){

                    forcaRegex::RegexResult::namedGroups[key].push_back(data);

                }
                else{

                    std::vector<forcaRegex::RegexStructData> newPush;

                    newPush.push_back(data);

                    forcaRegex::RegexResult::namedGroups[key] = newPush;

                    forcaRegex::RegexResult::string_keys.push_back(key);

                }

            };

            std::vector<RegexStructData>* get(int key) {
                return forcaRegex::RegexResult::key_exist(key) ? &forcaRegex::RegexResult::numberGroups[key] : nullptr;
            };

            std::vector<RegexStructData>* get(const std::string& key) {
                std::string newKey = forcaRegex::RegexResult::trim(key);
                std::map<std::string, std::vector<forcaRegex::RegexStructData>>::iterator item = forcaRegex::RegexResult::namedGroups.find(newKey);
                return item != forcaRegex::RegexResult::namedGroups.end() ? &item->second : nullptr;
            };

    };
    
    forcaRegex::RegexPattern createPattern( const std::string& pattern );

    forcaRegex::RegexResult preg_match( const std::string& pattern, const std::string& subject, PCRE2_SIZE offset = 0 );

    forcaRegex::RegexResult preg_match_all( const std::string& pattern, const std::string& subject, PCRE2_SIZE offset = 0 );

    std::string preg_replace( const std::string& pattern, const std::string& subject, const std::string& replacement, PCRE2_SIZE offset = 0 );

}

#endif