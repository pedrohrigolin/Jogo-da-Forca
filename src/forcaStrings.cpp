#include <cstring>
#include <string>
#include <vector>
#include <regex>
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
     * @brief Divide uma string em substrings com base em um separador e armazena o resultado em um vetor.
     *
     * Esta função simula o comportamento da função explode do PHP, separando a string de entrada em partes,
     * utilizando o separador informado, e armazenando cada parte no vetor passado por ponteiro.
     *
     * @param string     String a ser dividida.
     * @param separator  Separador utilizado para dividir a string.
     * @param reference  Ponteiro para o vetor onde as substrings resultantes serão armazenadas.
     *
     * @note Por preferência pessoal, a implementação ideal seria retornar um std::vector<std::string> e utilizar um loop while,
     *       mas optei por esta abordagem recursiva com ponteiro para demonstrar conhecimento em manipulação de ponteiros e recursividade.
     */
    void explode( std::string string, std::string separator, std::vector<std::string>* reference ) {

        std::string::size_type pos = string.find(separator);

        if( pos != std::string::npos ){

            std::string substr = string.substr(0, pos);

            reference->push_back(substr);

            string = string.erase( 0, pos + separator.length() );

            explode(string, separator, reference);

        }
        else{
            reference->push_back(string);
        }

    }

}