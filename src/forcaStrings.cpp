#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include "forcaStrings.h"

namespace forcaStrings {

    /* 
    |=========================================
    |   FUNÇÕES DE NORMALIZAÇÃO DE STRING
    |========================================= 
    */

    /**
     * Remove os espaçamentos da string obtida. Isso é util para normalizar a string
     * e evitar erros no funcionamento do código, além de corrigir possíveis espaçamentos
     * não intencionais feitos pelo usuário. Remove espaços em branco, tabs, quebras de linha
     * e outros caracteres de espaçamento.
     *
     * @param   std::string text    String que terá seus espaços removidos
     * @return  std::string         String normalizada sem espaços
     */
    std::string removeSpaces( std::string text ) {

        std::string::size_type spacePos = text.find_first_of(" \t\n\r\v\f");

        while(spacePos != std::string::npos){

            // Remove o espaço encontrado.
            text.erase(spacePos, 1);
            
            /*          
            |    Busca o proximo espaço a partir da posição do ultimo espaço encontrado.
            |    Isso otimiza a busca pois o find não vai ficar percorrendo a string desde o
            |    começo toda vez que for usado. 
            */
            spacePos = text.find_first_of(" \t\n\r\v\f", spacePos);

        }

        return text;

    }

    /**
     * Remove acentos e caracteres especiais (ç) de uma string, substituindo-os 
     * por seus equivalentes sem acentuação. Por exemplo, 'á' é substituído por 'a',
     * 'ç' por 'c', etc.
     *
     * @param   std::string text    String que terá seus acentos removidos
     * @return  std::string         String normalizada sem acentos
     */
    std::string removeAcentos( std::string text ) {

        int i;
        std::string::size_type pos;
        std::string::size_type stringSize;

        /**
         * Palavras acentuadas e ç. Serão usadas para comparação e depois
         * serão substituidas pela letra normal na função de normalização
         * de strings.
         * 
         * @var	string _REMOVE_ACENTOS_ACENTUADAS_
         */
        static const std::string _REMOVE_ACENTOS_ACENTUADAS_[] = {
            "á", "à", "â", "ã", "ä", "Á", "À", "Â", "Ã", "Ä",
            "é", "è", "ê", "ë", "É", "È", "Ê", "Ë",
            "í", "ì", "î", "ï", "Í", "Ì", "Î", "Ï",
            "ó", "ò", "ô", "õ", "ö", "Ó", "Ò", "Ô", "Õ", "Ö",
            "ú", "ù", "û", "ü", "Ú", "Ù", "Û", "Ü",
            "ç", "Ç"
        };

        /**
         * Palavras normais, sem acentuação e ç. Serão usadas para substituição,
         * na função de normalização de strings.
         * 
         * @var string _REMOVE_ACENTOS_NORMALIZADAS_
         */
        static const std::string _REMOVE_ACENTOS_NORMALIZADAS_[] = {
            "a", "a", "a", "a", "a", "A", "A", "A", "A", "A",
            "e", "e", "e", "e", "E", "E", "E", "E",
            "i", "i", "i", "i", "I", "I", "I", "I",
            "o", "o", "o", "o", "o", "O", "O", "O", "O", "O",
            "u", "u", "u", "u", "U", "U", "U", "U",
            "c", "C"
        };

        /**
         * Cache do tamanho da string constante ACENTUADOS. Evita ficar recalculando
         * desnecessáriamente o tamanho a acada função.
         * 
         * @var	int	_REMOVE_ACENTOS_MAP_SIZE_
         */
        static const int _REMOVE_ACENTOS_MAP_SIZE_ =
            sizeof(_REMOVE_ACENTOS_ACENTUADAS_) / sizeof(_REMOVE_ACENTOS_ACENTUADAS_[0]);

        for(i = 0; i < _REMOVE_ACENTOS_MAP_SIZE_; i++) {

            stringSize = _REMOVE_ACENTOS_ACENTUADAS_[i].size();
            pos = text.find(_REMOVE_ACENTOS_ACENTUADAS_[i]);

            while(pos != std::string::npos) {

                text.replace(pos, stringSize, _REMOVE_ACENTOS_NORMALIZADAS_[i]);
                pos = text.find(_REMOVE_ACENTOS_ACENTUADAS_[i], pos);
            
            }

        }

        return text;

    }

    /**
     * Converte todos os caracteres de uma string para maiúsculos. A função lida 
     * corretamente com caracteres ASCII estendido (>127) através de um cast para 
     * unsigned char.
     *
     * @param   std::string text    String que será convertida para maiúsculo
     * @return  std::string         String com todos os caracteres em maiúsculo
     */
    std::string to_uppercase( std::string text ) {

        std::string::size_type i, length;

        length = text.size();

        for (i = 0; i < length; i++) {

            // Cast para unsigned char para evitar comportamento indefinido em caracteres >127
            text[i] = static_cast<char>( std::toupper(static_cast<unsigned char>( text[i] ) ) );

        }

        return text;

    }

    /**
     * Converte todos os caracteres de uma string para minúsculos. A função lida 
     * corretamente com caracteres ASCII estendido (>127) através de um cast para 
     * unsigned char.
     *
     * @param   std::string text    String que será convertida para minúsculo
     * @return  std::string         String com todos os caracteres em minúsculo
     */
    std::string to_lowercase( std::string text ) {

        std::string::size_type i, length;

        length = text.size();

        for (i = 0; i < length; i++) {

            // Cast para unsigned char para evitar comportamento indefinido em caracteres >127
            text[i] = static_cast<char>( std::tolower(static_cast<unsigned char>( text[i] ) ) );

        }

        return text;

    }

    /**
     * Remove espaços em branco do início e do fim de uma string.
     * Combina as funcionalidades de ltrim() e rtrim() em uma única chamada.
     * 
     * @param   const std::string& string    String que terá os espaços removidos
     * @return  std::string                  String sem espaços no início e fim
     */
    std::string trim( const std::string& string ) {

        std::string text = string;

        while( isspace(text[0]) && text.length() != 0 ){
        
            text.erase(0, 1);

        }

        while( isspace( text[ text.length() - 1 ] ) && text.length() > 0 ){
        
            text.erase( (text.length() - 1), 1 );

        }

        return text;

    }

    /**
     * Remove espaços em branco do início de uma string (left trim).
     * Remove espaços, tabs, quebras de linha e outros caracteres de espaçamento
     * do início da string.
     * 
     * @param   const std::string& string    String que terá os espaços removidos do início
     * @return  std::string                  String sem espaços no início
     */
    std::string ltrim( const std::string& string ) {

        std::string text = string;

        while( isspace(text[0]) && text.length() != 0 ){
        
            text.erase(0, 1);

        }

        return text;

    }

    /**
     * Remove espaços em branco do fim de uma string (right trim).
     * Remove espaços, tabs, quebras de linha e outros caracteres de espaçamento
     * do final da string.
     * 
     * @param   const std::string& string    String que terá os espaços removidos do fim
     * @return  std::string                  String sem espaços no fim
     */
    std::string rtrim( const std::string& string ) {

        std::string text = string;

        while( isspace( text[ text.length() - 1 ] ) && text.length() > 0 ){
        
            text.erase( (text.length() - 1), 1 );

        }

        return text;

    }

    /**
     * Normaliza uma string removendo espaços, acentos e convertendo para maiúsculo.
     * Esta função combina as operações de removeSpaces(), removeAcentos() e 
     * to_uppercase() em uma única chamada.
     *
     * @param   std::string text    String que será normalizada
     * @return  std::string         String normalizada sem espaços, acentos e em maiúsculo
     */
    std::string normalizeWord( std::string text ) {

        text = forcaStrings::removeSpaces(text);

        text = forcaStrings::removeAcentos(text);

        text = forcaStrings::to_uppercase(text);

        return text;

    }

    // std::string normalizeBreakLines( std::string text ){


    // }

    /*
    |=============================
    |   FUNÇÕES DE VERIFICAÇÃO
    |=============================
    */

    /**
     * Verifica se uma string contém apenas caracteres alfabéticos (A-Z, a-z).
     *
     * Percorre cada caractere da string e retorna false caso encontre algum caractere
     * que não seja uma letra. Caso todos os caracteres sejam letras, retorna true.
     *
     * @param   std::string word    String a ser verificada.
     * @return  bool                true se todos os caracteres são letras, false caso contrário.
     */
    bool checkAlphaCharacters( std::string word ) {

        std::string::size_type length = word.length();

        std::string::size_type i;

        for (i = 0; i<length; i++) {

            if ( ! std::isalpha( static_cast<unsigned char>( word[i] ) ) ) return false;

        }

        return true;

    }

    /**
     * Divide uma string em um vetor de strings usando um separador.
     * Esta versão trabalha com strings passadas por referência constante.
     * 
     * Essa função foi inspirada pela função explode do PHP.
     * 
     * @note Por preferência pessoal, a implementação ideal seria retornar um std::vector<std::string> e utilizar um loop while,
     *       mas optei por esta abordagem recursiva com ponteiro para demonstrar conhecimento em manipulação de ponteiros e recursividade.
     * 
     * @param   const std::string& string    String a ser dividida
     * @param   std::string separator        String usada como separador
     * @param   std::vector<std::string>* reference    Ponteiro para o vetor que armazenará os resultados
     * @return  void
     */
    void explode( const std::string& string, std::string separator, std::vector<std::string>* reference ) {

        /*         
            Cria uma cópia da string de entrada.
            Isso é preciso pois se uma string const for passada, não será possível
            modificar, e também evita alterar strings originais, já que são passadas por referência. 
        */
        std::string copy = string;

        std::string::size_type pos = copy.find(separator);

        if( pos != std::string::npos ){

            std::string substr = copy.substr(0, pos);

            reference->push_back(substr);

            copy = copy.erase( 0, pos + separator.length() );

            explode(copy, separator, reference);

        }
        else{
            reference->push_back(copy);
        }

    }

    /**
     * Divide uma string em um vetor de strings usando um separador.
     * Esta versão trabalha com strings mutáveis (char*).
     * 
     * @param   char* string                 String a ser dividida
     * @param   std::string separator        String usada como separador
     * @param   std::vector<std::string>* reference    Ponteiro para o vetor que armazenará os resultados
     * @return  void
     */
    void explode( char* string, std::string separator, std::vector<std::string>* reference ) {

        std::string copy = string;

        explode(copy, separator, reference);

    }

    /**
     * Divide uma string em um vetor de strings usando um separador.
     * Esta versão trabalha com strings constantes (const char*).
     * 
     * @param   const char* string           String a ser dividida
     * @param   std::string separator        String usada como separador
     * @param   std::vector<std::string>* reference    Ponteiro para o vetor que armazenará os resultados
     * @return  void
     */
    void explode( const char* string, std::string separator, std::vector<std::string>* reference ) {

        std::string copy = string;

        explode(copy, separator, reference);

    }

    /**
     * Concatena elementos de um vetor de strings usando um delimitador.
     * Esta versão trabalha com std::vector<std::string>.
     * 
     * Essa função foi inspirada na função implode do PHP.
     * 
     * @param   const std::vector<std::string>& array    Vetor de strings a ser concatenado
     * @param   std::string delimiter                    String usada como delimitador
     * @return  std::string                             String resultante da concatenação
     */
    std::string implode( const std::vector<std::string>& array, std::string delimiter ) {

        if(array.empty()) return "";

        std::size_t i, length = array.size();

        std::string response = array[0];

        for(i=1; i<length; i++){

            response += delimiter;
            response += array[i]; 

        }

        return response;

    }

    /**
     * Concatena elementos de um array de strings usando um delimitador.
     * Esta versão trabalha com arrays de strings mutáveis (char*).
     * 
     * @param   char* array[]               Array de strings a ser concatenado
     * @param   int size                    Tamanho do array
     * @param   std::string delimiter       String usada como delimitador
     * @return  std::string                String resultante da concatenação
     */
    std::string implode(char* array[], int size, std::string delimiter) {

        if (size <= 0 || array == nullptr) return "";
        
        std::string response = array[0];
        
        for(int i = 1; i < size; i++) {

            if (array[i] == nullptr) break;

            response += delimiter;
            response += array[i];

        }
        
        return response;
        
    }

    /**
     * Concatena elementos de um array de strings usando um delimitador.
     * Esta versão trabalha com arrays de strings constantes (const char*).
     * 
     * @param   const char* array[]         Array de strings a ser concatenado
     * @param   int size                    Tamanho do array
     * @param   std::string delimiter       String usada como delimitador
     * @return  std::string                String resultante da concatenação
     */
    std::string implode(const char* array[], int size, std::string delimiter) {

        if (size <= 0 || array == nullptr) return "";
        
        std::string response = array[0];
        
        for(int i = 1; i < size; i++) {

            if (array[i] == nullptr) break;

            response += delimiter;
            response += array[i];

        }
        
        return response;
        
    }

}