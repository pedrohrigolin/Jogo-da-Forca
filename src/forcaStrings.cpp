#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unicode/unistr.h>
#include <unicode/brkiter.h>
#include <unicode/ustream.h>
#include <unicode/uchar.h>
#include <unicode/locid.h>
#include "forcaStrings.h"
#include "forcaRegex.h"
#include "forcaUtils.h"

namespace forcaStrings {

    /* 
    |=========================================
    |   FUNÇÕES DE NORMALIZAÇÃO DE STRING
    |========================================= 
    */

    /**
     * Remove os espaçamentos da string obtida, considerendo caracteres unicode.
     *
     * @param   std::string text    String que terá seus espaços removidos
     * @return  std::string         String normalizada sem espaços
     */
    std::string removeSpaces( const std::string& string ) {

        std::string text = string;

        icu::UnicodeString input(text.c_str(), "UTF-8");

        int32_t i = 0;

        while (i < input.length()) {

            UChar32 c = input.char32At(i);

            if (u_isUWhiteSpace(c)) {

                input.remove(i, U16_LENGTH(c));

            } 
            else {
                i += U16_LENGTH(c);
            }

        }

        std::string response;

        input.toUTF8String(response);

        return response;

    }

    /**
     * Remove acentos e caracteres especiais (ç) de uma string, substituindo-os 
     * por seus equivalentes sem acentuação. Por exemplo, 'á' é substituído por 'a',
     * 'ç' por 'c', etc. Focado em palavras pt-br.
     *
     * @param   std::string text    String que terá seus acentos removidos
     * @return  std::string         String normalizada sem acentos
     */
    std::string removeAcentos( const std::string& string ) {

        std::string text = string;

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
     * corretamente com caracteres Unicode.
     *
     * @param   std::string text    String que será convertida para maiúsculo
     * @return  std::string         String com todos os caracteres em maiúsculo
     */
    std::string to_uppercase( const std::string& string ) {
        
        std::string text = string;

        icu::UnicodeString input(text.c_str(), "UTF-8");

        input.toUpper(icu::Locale::getDefault());

        std::string response;

        input.toUTF8String(response);

        return response;

    }

    /**
     * Converte todos os caracteres de uma string para minúsculos. A função lida 
     * corretamente com caracteres Unicode.
     *
     * @param   std::string text    String que será convertida para minúsculo
     * @return  std::string         String com todos os caracteres em minúsculo
     */
    std::string to_lowercase( const std::string& string ) {

        std::string text = string;

        icu::UnicodeString input(text.c_str(), "UTF-8");

        input.toLower(icu::Locale::getDefault());

        std::string response;

        input.toUTF8String(response);

        return response;

    }

    /**
     * Remove espaços em branco do início e do fim de uma string.
     * Combina as funcionalidades de ltrim() e rtrim() em uma única chamada.
     * 
     * Inspirado na função PHP trim().
     * 
     * @param   const std::string& string    String que terá os espaços removidos
     * @return  std::string                  String sem espaços no início e fim
     */
    std::string trim( const std::string& string ) {

        std::string text = string;

        icu::UnicodeString input(text.c_str(), "UTF-8");

        UChar32 c = input.char32At(0);

        while( u_isUWhiteSpace(c) && input.length() != 0){

            input.remove(0, U16_LENGTH(c));   
            
            c = input.char32At(0);

        }

        c = input.char32At(input.length() - 1);

        while( u_isUWhiteSpace(c) && input.length() != 0){

            input.remove( (input.length() - 1), U16_LENGTH(c) );

            c = input.char32At(input.length() - 1);

        }

        std::string response;

        input.toUTF8String(response);

        return response;

    }

    /**
     * Remove espaços em branco do início de uma string (left trim).
     * Remove espaços, tabs, quebras de linha e outros caracteres de espaçamento
     * do início da string.
     * 
     * Inspirado na função PHP ltrim().
     * 
     * @param   const std::string& string    String que terá os espaços removidos do início
     * @return  std::string                  String sem espaços no início
     */
    std::string ltrim( const std::string& string ) {

        std::string text = string;

        icu::UnicodeString input(text.c_str(), "UTF-8");

        UChar32 c = input.char32At(0);

        while( u_isUWhiteSpace(c) && input.length() != 0){

            input.remove(0, U16_LENGTH(c));   
            
            c = input.char32At(0);

        }

        std::string response;

        input.toUTF8String(response);

        return response;

    }

    /**
     * Remove espaços em branco do fim de uma string (right trim).
     * Remove espaços, tabs, quebras de linha e outros caracteres de espaçamento
     * do final da string.
     * 
     * Inspirado na função PHP rtrim().
     * 
     * @param   const std::string& string    String que terá os espaços removidos do fim
     * @return  std::string                  String sem espaços no fim
     */
    std::string rtrim( const std::string& string ) {

        std::string text = string;

        icu::UnicodeString input(text.c_str(), "UTF-8");

        UChar32 c = input.char32At( (input.length() - 1) );

        while( u_isUWhiteSpace(c) && input.length() != 0){

            input.remove( (input.length() - 1), U16_LENGTH(c) );

            c = input.char32At(input.length() - 1);

        }

        std::string response;

        input.toUTF8String(response);

        return response;

    }

    /**
     * Normaliza uma string removendo espaços, acentos e convertendo para maiúsculo.
     * Esta função combina as operações de removeSpaces(), removeAcentos() e 
     * to_uppercase() em uma única chamada.
     *
     * @param   std::string text    String que será normalizada
     * @return  std::string         String normalizada sem espaços, acentos e em maiúsculo
     */
    std::string normalizeWord( const std::string& string ) {

        std::string text = string;

        text = forcaStrings::removeSpaces(text);

        text = forcaStrings::removeAcentos(text);

        text = forcaStrings::to_uppercase(text);

        return text;

    }

    /**
     * Normaliza quebras de linha em uma string, convertendo diferentes tipos de quebra
     * para o formato Unix (\n).
     * 
     * Converte as seguintes sequências para \n:
     * - \r\n (Windows)
     * - \v (Vertical tab)
     * - \f (Form feed)
     * - \n (Unix)
     *
     * @param   const std::string& text    String que terá as quebras de linha normalizadas
     * @return  std::string               String com quebras de linha normalizadas para \n
     */
    std::string normalizeLineBreaks( const std::string& text ) {

        return forcaRegex::preg_replace(R"(/(?:\r\n|\r|\n|\v|\f|\x85|\x0b|\p{Zl}|\p{Zp})/u)", text, "\n");

    }

    /**
     * Remove quebras de linha múltiplas consecutivas, substituindo-as por uma única quebra.
     * 
     * Por exemplo:
     * "texto\n\n\n\ntexto" se torna "texto\ntexto"
     *
     * @param   const std::string& text    String que terá as quebras de linha extras removidas
     * @return  std::string               String com apenas quebras de linha simples
     */
    std::string removeExtraLineBreaks( const std::string& text, bool normalize ) {

        if(normalize){
            return forcaRegex::preg_replace(R"(/(?:\r\n|\r|\n|\v|\f|\x85|\x0b|\p{Zl}|\p{Zp})+/u)", text, "\n");
        }
        else{
            return forcaRegex::preg_replace(R"(/((?:\r\n|\r|\n|\v|\f|\x85|\x0b|\p{Zl}|\p{Zp}))+/u)", text, "$1");
        }

    }

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
    bool checkAlphaCharacters( const std::string& word ) {

        std::string::size_type length = word.length();

        std::string::size_type i;

        for (i = 0; i<length; i++) {

            if ( ! std::isalpha( static_cast<unsigned char>( word[i] ) ) ) return false;

        }

        return true;

    }

    /**
     * Quebra uma string em um vetor de caracteres/graphemes considerando code points (Unicode).
     *
     * Esta função utiliza ICU para dividir a string em graphemes, ou seja, em unidades de texto visíveis,
     * respeitando os code points Unicode. Isso é útil para manipulação correta de strings Unicode,
     * onde um "caractere" pode ser composto por múltiplos bytes ou combinações de símbolos.
     *
     * @param string String de entrada a ser quebrada em graphemes.
     * @return std::vector<std::string> Vetor contendo cada grapheme (caractere visual) da string original.
     * @throws std::runtime_error Em caso de erro na biblioteca ICU.
     */
    std::vector<std::string> explodeGraphemes( const std::string& string ) {

        std::string input = string;

        icu::UnicodeString inputUnicode(input.c_str(), "UTF-8");

        UErrorCode status = U_ZERO_ERROR;

        std::unique_ptr<icu::BreakIterator> it(
            icu::BreakIterator::createCharacterInstance(icu::Locale::getDefault(), status)
        );

        if ( U_FAILURE(status) ) {

            std::string error = u_errorName(status);

            throw std::runtime_error("ICU Erro: " + error);

        }

        std::vector<std::string> explode;

        it->setText(inputUnicode);

        int32_t start = it->first();
        int32_t end = it->next();

        while( end != icu::BreakIterator::DONE ){

            icu::UnicodeString grapheme = inputUnicode.tempSubStringBetween(start, end);
            
            std::string utf8;

            grapheme.toUTF8String(utf8);

            explode.push_back(utf8);

            start = end;

            end = it->next();

        }

        return explode;

    }

    /**
     * Calcula o tamanho de uma string com base nos caracteres visíveis (graphemes/code points Unicode).
     *
     * Esta função retorna a quantidade de caracteres visuais presentes na string, considerando corretamente
     * caracteres Unicode que podem ocupar múltiplos bytes. Diferente de std::string::size(), que retorna o
     * número de bytes, esta função utiliza ICU para contar os graphemes/code points, fornecendo o tamanho real
     * percebido pelo usuário.
     *
     * @param string String de entrada a ser analisada.
     * @return std::string::size_type Quantidade de caracteres visíveis (graphemes) na string.
     * @throws std::runtime_error Em caso de erro na biblioteca ICU.
     */
    std::string::size_type Length( const std::string& string ) {

        std::string input = string;

        icu::UnicodeString inputUnicode(input.c_str(), "UTF-8");

        UErrorCode status = U_ZERO_ERROR;

        std::unique_ptr<icu::BreakIterator> it(
            icu::BreakIterator::createCharacterInstance(icu::Locale::getDefault(), status)
        );

        if ( U_FAILURE(status) ) {

            std::string error = u_errorName(status);

            throw std::runtime_error("ICU Erro: " + error);

        }

        it->setText(inputUnicode);

        int32_t start = it->first();
        int32_t end = it->next();

        std::string::size_type length = 0;

        while( end != icu::BreakIterator::DONE ){

            length++;

            start = end;

            end = it->next();

        }

        return length;

    }

    /**
     * Retorna o caractere visível (grapheme/code point Unicode) na posição informada da string.
     *
     * Esta função retorna o caractere visual (grapheme) na posição especificada, levando em consideração
     * os code points Unicode, e não apenas bytes individuais. Assim, a posição é baseada no que o usuário
     * percebe como "caracteres", mesmo que ocupem múltiplos bytes.
     *
     * Inspirada na função charAt do JavaScript, mas adaptada para suportar Unicode corretamente em C++.
     *
     * @param string String de entrada.
     * @param index  Índice do caractere desejado (baseado em graphemes/code points Unicode).
     * @return std::string Caractere visual na posição informada, ou string vazia se a string for vazia.
     * @throws std::out_of_range Se o índice for maior ou igual ao número de caracteres visíveis.
     */
    std::string charAt( const std::string& string, std::string::size_type index ) {

        if(string.empty()) return "";

        std::string::size_type length = forcaStrings::Length(string);

        if(index >= length) throw std::out_of_range("forcaStrings::charAt: index (which is " + std::to_string(index) + ") >= this->length() (which is " + std::to_string(length) + ")");

        std::vector<std::string> explode = forcaStrings::explodeGraphemes(string);

        std::string charString = explode.at(index);

        return charString;

    }

    /**
     * Extrai uma substring com base nos caracteres visíveis (graphemes/code points Unicode).
     *
     * Esta função retorna uma substring da string original, considerando corretamente os caracteres visuais
     * (graphemes/code points Unicode), e não apenas o número de bytes. Assim, a extração é feita de acordo
     * com o que o usuário percebe como "caracteres", mesmo que eles ocupem múltiplos bytes.
     *
     * @param string String de entrada.
     * @param pos    Posição inicial (baseada em graphemes/code points Unicode).
     * @param len    Comprimento da substring (em graphemes/code points Unicode).
     * @return std::string Substring extraída, respeitando os caracteres visíveis.
     * @throws std::out_of_range Se a posição inicial for maior que o tamanho da string.
     */
    std::string substring( const std::string& string, std::size_t pos, std::size_t len ) {

        std::string copy = string;

        std::string::size_type length = forcaStrings::Length(copy);

        if(pos > length) throw std::out_of_range("forcaStrings::substring: pos (which is " + std::to_string(pos) + ") > this->size() (which is " + std::to_string(length) + ")");

        if(pos == length) return "";

        if( len > (length - pos) ) len = length - pos;

        std::vector<std::string> explode = forcaStrings::explodeGraphemes(copy);

        std::vector<std::string> substr;

        substr.assign( (explode.begin() + pos), ( explode.begin() + (pos + len) ) );

        std::string substring = forcaStrings::implode(substr);

        return substring;

    }

    /**
     * Retorna o índice da primeira ocorrência de um caractere visível (grapheme/code point Unicode) em uma string.
     *
     * Esta função busca a primeira ocorrência exata do caractere (ou grapheme) informado, levando em consideração
     * a divisão correta de caracteres Unicode (code points), e não apenas bytes. Assim, a busca é feita de acordo
     * com o que o usuário percebe como "caracteres", mesmo que ocupem múltiplos bytes.
     *
     * @param string String onde será feita a busca.
     * @param search Caractere/grapheme a ser procurado (pode ser um code point Unicode).
     * @return std::string::size_type Índice da primeira ocorrência do caractere/grapheme, ou std::string::npos se não encontrado.
     */
    std::string::size_type firstIndexOf( const std::string& string, const std::string& search ) {

        if(search.empty()) return std::string::npos;

        std::vector<std::string> explode = forcaStrings::explodeGraphemes(string);

        std::vector<std::string>::iterator index = std::find( explode.begin(), explode.end(), search);

        if(index == explode.end()) return std::string::npos;

        std::string::size_type pos = std::distance(explode.begin(), index);

        return pos;

    }

    /**
     * Retorna o índice da última ocorrência de um caractere visível (grapheme/code point Unicode) em uma string.
     *
     * Esta função busca a última ocorrência exata do caractere (ou grapheme) informado, levando em consideração
     * a divisão correta de caracteres Unicode (code points), e não apenas bytes. Assim, a busca é feita de acordo
     * com o que o usuário percebe como "caracteres", mesmo que ocupem múltiplos bytes.
     *
     * @param string String onde será feita a busca.
     * @param search Caractere/grapheme a ser procurado (pode ser um code point Unicode).
     * @return std::string::size_type Índice da última ocorrência do caractere/grapheme, ou std::string::npos se não encontrado.
     */
    std::string::size_type lastIndexOf( const std::string& string, const std::string& search ) {

        if(search.empty()) return std::string::npos;

        std::vector<std::string> explode = forcaStrings::explodeGraphemes(string);

        auto index = std::find( explode.rbegin(), explode.rend(), search);

        if(index == explode.rend()) return std::string::npos;

        std::string::size_type pos = std::distance( explode.begin(), (index.base() - 1) );

        return pos;

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
     * @param   const std::string& separator        String usada como separador
     * @param   std::vector<std::string>* reference    Ponteiro para o vetor que armazenará os resultados
     * @return  void
     */
    void explode( const std::string& string, const std::string& separator, std::vector<std::string>* reference, std::size_t limit ) {

        if(limit == 0 || reference->size() >= limit) return;

        /*         
            Cria uma cópia da string de entrada.
            Isso é preciso pois se uma string const for passada, não será possível
            modificar, e também evita alterar strings originais, já que são passadas por referência. 
        */
        std::string copy = string;

        std::string::size_type separatorLength = separator.length();

        if(separatorLength == 0){

            /* 
                Quando o separador é uma string vazia (""), o objetivo é quebrar a string em um vetor de caracteres visuais (graphemes),
                levando em consideração code points (Unicode). Isso é importante porque um "caractere" visual pode ser composto por múltiplos bytes,
                e a divisão correta deve respeitar a representação visual e não apenas os bytes individuais. Para isso, utilizamos a função
                explodeGraphemes, que trata corretamente os code points Unicode. Após obter os graphemes, o ajuste do limite é feito manualmente.
            */

            std::vector<std::string> explode = forcaStrings::explodeGraphemes(copy);

            while( ( reference->size() + 1 ) < limit && !explode.empty() ){

                reference->push_back(explode[0]);

                explode.erase( explode.begin() );

            }

            if( !explode.empty() && limit != 0 ){

                reference->push_back( forcaStrings::implode(explode) );

            }

            return;

        }
        
        /*
            Quando o separador possui tamanho maior que zero, a busca e quebra da string é feita normalmente usando find e substr,
            pois a posição do separador na string não depende da quantidade de bytes de cada caractere, mas sim da posição lógica
            onde o separador ocorre. Além disso, o código já considera corretamente o tamanho do separador (separatorLength), permitindo
            separadores com múltiplos bytes ou múltiplos caracteres. Isso garante que a divisão será feita exatamente após o separador,
            independentemente de seu tamanho ou conteúdo.
        */

        std::string::size_type pos = copy.find(separator);

        if( separatorLength > 0 && pos != std::string::npos && ( reference->size() + 1 ) < limit ) {

            std::string substr = copy.substr(0, pos);

            reference->push_back(substr);

            copy = copy.erase( 0, pos + separatorLength );

            explode(copy, separator, reference, limit);

        }
        else{
            reference->push_back(copy);
        }

    }

    /**
     * Divide uma string em um vetor de strings usando um separador.
     * Esta versão trabalha com strings mutáveis (char*).
     * 
     * @note Por preferência pessoal, a implementação ideal seria retornar um std::vector<std::string> e utilizar um loop while,
     *       mas optei por esta abordagem recursiva com ponteiro para demonstrar conhecimento em manipulação de ponteiros e recursividade.      
     *
     * @param   char* string                 String a ser dividida
     * @param   const std::string& separator        String usada como separador
     * @param   std::vector<std::string>* reference    Ponteiro para o vetor que armazenará os resultados
     * @return  void
     */
    void explode( char* string, const std::string& separator, std::vector<std::string>* reference, std::size_t limit ) {

        explode(string, separator, reference, limit);

    }

    /**
     * Divide uma string em um vetor de strings usando um separador.
     * Esta versão trabalha com strings constantes (const char*).
     * 
     * @note Por preferência pessoal, a implementação ideal seria retornar um std::vector<std::string> e utilizar um loop while,
     *       mas optei por esta abordagem recursiva com ponteiro para demonstrar conhecimento em manipulação de ponteiros e recursividade.     
     * 
     * @param   const char* string           String a ser dividida
     * @param   const std::string& separator        String usada como separador
     * @param   std::vector<std::string>* reference    Ponteiro para o vetor que armazenará os resultados
     * @return  void
     */
    void explode( const char* string, const std::string& separator, std::vector<std::string>* reference, std::size_t limit ) {

        explode(string, separator, reference, limit);

    }

    /**
     * Concatena elementos de um vetor de strings usando um delimitador.
     * Esta versão trabalha com std::vector<std::string>.
     * 
     * Essa função foi inspirada na função implode do PHP.
     * 
     * @param   const std::vector<std::string>& array    Vetor de strings a ser concatenado
     * @param   const std::string& delimiter                    String usada como delimitador
     * @return  std::string                             String resultante da concatenação
     */
    std::string implode( const std::vector<std::string>& array, const std::string& delimiter ) {

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
     * @param   const std::string& delimiter       String usada como delimitador
     * @return  std::string                String resultante da concatenação
     */
    std::string implode(char* array[], int size, const std::string& delimiter) {

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
     * @param   const std::string& delimiter       String usada como delimitador
     * @return  std::string                String resultante da concatenação
     */
    std::string implode(const char* array[], int size, const std::string& delimiter) {

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