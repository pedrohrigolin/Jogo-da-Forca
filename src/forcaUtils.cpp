#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm> // Para usar o std::find
#include <stdexcept>
#include "forcaUtils.h"
#include "forcaStrings.h"

namespace forcaUtils {

    /**
     * Limpa a tela do terminal de forma robusta para diferentes sistemas operacionais.
     *
     * Esta função força a saída de qualquer conteúdo pendente no buffer de std::cout antes e depois do comando de limpeza.
     * Utiliza o comando "cls" para sistemas Windows e "clear" para sistemas Unix-like, escolhendo em tempo de compilação.
     * Após o comando do sistema, utiliza também a sequência ANSI "\033[2J\033[1;1H" para garantir a limpeza em terminais modernos,
     * aumentando a compatibilidade e robustez da função.
     *
     * Observação: Em alguns terminais integrados (como o terminal de debug do VS Code), nenhum método pode funcionar.
     */
    void clear_screen(){
        
        // Força a saída de possíveis buffers acumulados no std::cout
        std::cout<<std::flush;

        #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
            std::system("cls");
        #else
            std::system("clear");
        #endif

        // Depois tenta ANSI (caso o terminal suporte), isso melhora a robustes da função
        std::cout << "\033[2J\033[1;1H" << std::flush;

        // Força a saída de possíveis buffers acumulados no std::cout
        std::cout<<std::flush;
        
    }

    /**
     * Funções filter_validate_bool
     *
     * Estas funções implementam o comportamento semelhante ao filter_var() do PHP com a flag FILTER_VALIDATE_BOOL.
     * O objetivo é validar e converter diferentes tipos de valores (std::string, bool, int) para booleanos em C++.
     * 
     * Para isso, utilizamos a sobrecarga de funções: declaramos três funções com o mesmo nome, mas com tipos diferentes no primeiro parâmetro.
     * Assim, ao chamar filter_validate_bool() com diferentes tipos de variáveis, o compilador seleciona automaticamente a versão correta da função.
     * 
     * Parâmetros:
     *   - value: valor a ser validado (std::string, bool ou int)
     *   - strict: se true, apenas valores explicitamente booleanos são aceitos; caso contrário, aceita valores equivalentes.
     * 
     * Retorno:
     *   - true ou false, conforme a validação do valor.
     *   - Lança std::invalid_argument se o valor não puder ser interpretado como booleano em modo estrito.
     */

    /**
     * Valida e converte uma string para booleano.
     * 
     * Aceita valores como "TRUE", "1", "ON", "YES", "SIM" para true e "FALSE", "0", "OFF", "NO", "NAO" para false.
     * Se strict for true, lança std::invalid_argument para valores não reconhecidos.
     * 
     * OBS: VALORES SÃO CASE INSENSITIVE E PASSAM POR NORMALIZAÇÃO DE STRINGS, ENTÃO É PERMITIDO VALORES COMO
     * NÃO, sim, yes, NO, NAO, TRUE e etc, SEM PRECISAR SE PREOCUPAR COM NORMALIZAR A STRING ANTES.
     * 
     * @param value Valor a ser validado (std::string).
     * @param strict Se true, apenas valores explicitamente booleanos são aceitos; caso contrário, aceita valores equivalentes. Padrão: false.
     * @return true se o valor for considerado verdadeiro, false caso contrário.
     * @throws std::invalid_argument Se strict for true e o valor não puder ser interpretado como booleano.
     */
    bool filter_validate_bool( std::string value, bool strict ){

        static const std::vector<std::string> TRUE_VALUES = { "TRUE", "1", "ON", "YES", "SIM" };

        static const std::vector<std::string> FALSE_VALUES = { "FALSE", "0", "OFF", "NO", "NAO" };

        value = forcaStrings::normalizeWord(value);

        std::vector<std::string>::const_iterator pos = std::find(TRUE_VALUES.begin(), TRUE_VALUES.end(), value);

        if( pos != TRUE_VALUES.end() ) return true;

        if(strict){

            pos = std::find(FALSE_VALUES.begin(), FALSE_VALUES.end(), value);

            if( pos != FALSE_VALUES.end() ) return false;

            // Se chegou aqui, valor não é reconhecido como booleano válido
            throw std::invalid_argument("Valor booleano invalido: " + value);

        }

        return false;

    }

    /**
     * Retorna o próprio valor booleano.
     * 
     * Esta função existe para manter a interface consistente com as outras sobrecargas.
     * 
     * @param value Valor booleano.
     * @param strict Parâmetro ignorado, apenas para manter a interface. Padrão: false.
     * @return O próprio valor booleano.
     */
    bool filter_validate_bool( bool value, bool strict ){ return value; }

    /**
     * Valida e converte um inteiro para booleano.
     * 
     * Aceita 1 como true e 0 como false.
     * Se strict for true, lança std::invalid_argument para outros valores.
     * 
     * @param value Valor inteiro a ser validado.
     * @param strict Se true, apenas 1 e 0 são aceitos; caso contrário, aceita valores equivalentes. Padrão: false.
     * @return true se o valor for 1, false se for 0 ou outro valor (quando strict for false).
     * @throws std::invalid_argument Se strict for true e o valor não for 1 nem 0.
     */
    bool filter_validate_bool( int value, bool strict ){

        if(value == 1) return true;

        if(strict){

            if(value == 0) return false;

            // Se chegou aqui, valor não é reconhecido como booleano válido
            throw std::invalid_argument("Valor booleano invalido: " + value);

        }

        return false;

    }

    void debug( const std::string& string ) {

        std::cout<<std::endl<<std::endl<<string<<std::endl<<std::endl;

    }

}