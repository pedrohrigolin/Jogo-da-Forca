/**
 * @namespace ForcaUtils
 * @description
 * 
 * Objeto utilitário com funções auxiliares para validação e manipulação de valores no Jogo da Forca.
 * 
 * Fornece métodos para validação de booleanos e outras operações utilitárias.
 * 
 * Todos os métodos são síncronos e utilizam a ponte JS <-> C++ quando necessário.
 */
const ForcaUtils = Object.freeze({

    /**
     * @function filterValidateBoolean
     * 
     * @memberof ForcaUtils
     * 
     * @description
     * Valida e converte um valor para booleano, de forma semelhante ao filter_var() do PHP com FILTER_VALIDATE_BOOL.
     * 
     * Aceita valores do tipo string, number ou boolean.
     *
     * Valores aceitos como booleanos:
     *   - TRUE:
     *     - STRINGS: "TRUE", "1", "ON", "YES", "SIM"
     *     - INT: 1
     *     - BOOL: true
     *   - FALSE:
     *     - STRINGS: "FALSE", "0", "OFF", "NO", "NAO"
     *     - INT: 0
     *     - BOOL: false
     *
     * Internamente, as strings são normalizadas: convertidas para uppercase, sem acentuação e sem espaços,
     * permitindo aceitar variações como "sim", "nÃo", "Yes", etc.
     *
     * O parâmetro strict define o comportamento para valores não reconhecidos:
     *   - Se strict for true (padrão), apenas os valores explicitamente listados acima são aceitos; qualquer outro valor lança erro.
     *   - Se strict for false, valores reconhecidos como true retornam true, valores reconhecidos como false retornam false,
     *     e qualquer outro valor retorna false (sem lançar erro).
     *
     * Exemplo:
     *   - strict = true: "TRUE" → true, "FALSE" → false, "qualquer coisa" → erro
     *   - strict = false: "TRUE" → true, "FALSE" → false, "qualquer coisa" → false
     *
     * @param {string|number|boolean} value - Valor a ser validado.
     * @param {boolean} [strict=true] - Se true, apenas valores explicitamente booleanos são aceitos; caso contrário, valores não reconhecidos retornam false.
     * @returns {boolean} true ou false conforme a validação do valor.
     * @throws {TypeError} Se value não for string, number ou boolean, ou se strict não for boolean.
     * @throws {Error} Se strict for true e o valor não puder ser interpretado como booleano.
     */
    filterValidateBoolean: function(value, strict = true){

        if( typeof value !== 'string' && typeof value !== 'number' && typeof value !== 'boolean' ){
            throw new TypeError("O parâmetro value dever do tipo string, number, ou boolean");
        }

        if( typeof strict !== 'boolean' ) throw new TypeError("O parâmetro strict deve ser do tipo boolean!");

        const response = callUserFunc.sync("filterValidateBoolean", value, strict);

        if( typeof response !== 'boolean' ) throw new Error(`Error ao validar o valor ${value}`);

        return response;

    }

});