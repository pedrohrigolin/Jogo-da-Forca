/**
 * @namespace ForcaFiles
 * @description
 * Objeto utilitário para manipulação e leitura de arquivos no Jogo da Forca.
 * 
 * Fornece métodos para obter o conteúdo de arquivos em diferentes formatos (string, JSON, base64, binário),
 * realizando validações de tipo e tratamento de erros padronizado.
 * 
 * Todos os métodos são síncronos e utilizam a ponte JS <-> C++ para acessar o sistema de arquivos.
 */
const ForcaFiles = Object.freeze({

    /**
     * @function getStringContent
     * @memberof ForcaFiles
     * @description
     * Retorna o conteúdo de um arquivo como string.
     * Permite opcionalmente escapar o conteúdo retornado.
     *
     * @param {string} filepath - Caminho do arquivo a ser lido.
     * @param {boolean} [escape=false] - Se true, aplica escape ao conteúdo retornado.
     * @returns {string} Conteúdo do arquivo como string.
     * @throws {TypeError} Se filepath não for string ou escape não for boolean.
     * @throws {Error} Se ocorrer erro ao obter o conteúdo do arquivo.
     */
    getStringContent: function(filepath, escape = false) {

        if( typeof filepath !== "string" ) throw new TypeError("O parâmetro filepath deve ser do tipo string!");

        if( typeof escape !== "boolean" ) {
            
            /* 
                Se o parâmetro escape não for do tipo booleano, tenta aplicar o filtro de 
                validação booleana no modo estrito. Se resultar em erro, o valor não é um
                boolean, nem uma string booleana como "TRUE" e "FALSE", e nem inteiro
                booleano como 1 (true) e 0 (false).
            */

            try {
                escape = ForcaUtils.filterValidateBoolean(escape, true);
            }
            catch(e) {
                throw new TypeError("O parâmetro escape deve ser do tipo boolean");
            }

        }

        const content = callUserFunc.sync("getStringContent", filepath, escape);

        if( typeof content !== "string" ) throw new Error(`Erro desconhecido ao obter o conteúdo do arquivo ${filepath}`);

        return content;

    },

    /**
     * @function getJSONContent
     * @memberof ForcaFiles
     * @description
     * Retorna o conteúdo de um arquivo JSON como string.
     * O conteúdo deve ser posteriormente convertido para objeto usando JSON.parse.
     *
     * @param {string} filepath - Caminho do arquivo JSON a ser lido.
     * @returns {string} Conteúdo do arquivo JSON como string.
     * @throws {TypeError} Se filepath não for string.
     * @throws {Error} Se ocorrer erro ao obter o conteúdo do arquivo.
     */
    getJSONContent: function(filepath) {

        if( typeof filepath !== "string" ) throw new TypeError("O parâmetro filepath deve ser do tipo string!");

        const content = callUserFunc.sync("getJSONContent", filepath);

        if( typeof content !== "string" ) throw new Error(`Erro desconhecido ao obter o conteúdo do arquivo ${filepath}`);

        return content;

    },

    /**
     * @function getBase64Content
     * @memberof ForcaFiles
     * @description
     * Retorna o conteúdo de um arquivo codificado em base64.
     *
     * @param {string} filepath - Caminho do arquivo a ser lido.
     * @returns {string} Conteúdo do arquivo em base64.
     * @throws {TypeError} Se filepath não for string.
     * @throws {Error} Se ocorrer erro ao obter o conteúdo do arquivo.
     */    
    getBase64Content: function(filepath) {

        if( typeof filepath !== "string" ) throw new TypeError("O parâmetro filepath deve ser do tipo string!");

        const content = callUserFunc.sync("getBase64Content", filepath);

        if( typeof content !== "string" ) throw new Error(`Erro desconhecido ao obter o conteúdo do arquivo ${filepath}`);

        return content;

    },

    /**
     * @function getBinaryContent
     * @memberof ForcaFiles
     * @description
     * Retorna o conteúdo de um arquivo como ArrayBuffer (binário).
     *
     * @param {string} filepath - Caminho do arquivo a ser lido.
     * @returns {ArrayBuffer} Conteúdo binário do arquivo.
     * @throws {TypeError} Se filepath não for string.
     * @throws {Error} Se ocorrer erro ao obter o conteúdo do arquivo.
     */    
    getBinaryContent: function(filepath) {

        if( typeof filepath !== "string" ) throw new TypeError("O parâmetro filepath deve ser do tipo string!");

        const content = callUserFunc.sync("getBinaryContent", filepath);

        if( ! (content instanceof ArrayBuffer) ) throw new Error(`Erro desconhecido ao obter o conteúdo do arquivo ${filepath}`);

        return content;

    },

    /**
     * @function createFile
     * @memberof ForcaFiles
     * @description
     * Cria um novo arquivo no caminho especificado, com o conteúdo informado.
     * O conteúdo pode ser uma string ou um ArrayBuffer (binário).
     * Utiliza o backend C++ para garantir escrita segura e compatibilidade com diferentes formatos.
     *
     * @param {string} filepath - Caminho do arquivo a ser criado.
     * @param {string|ArrayBuffer} [content=""] - Conteúdo a ser gravado no arquivo.
     * @returns {boolean} true se o arquivo foi criado com sucesso.
     * @throws {TypeError} Se filepath não for string ou content não for string nem ArrayBuffer.
     * @throws {Error} Se ocorrer erro ao criar o arquivo.
     */
    createFile: function(filepath, content = "") {

        if( typeof filepath !== "string" ) throw new TypeError("O parâmetro filepath deve ser do tipo string!");

        if( typeof content !== "string" && ! (content instanceof ArrayBuffer) ){
            throw new TypeError("O parâmetro content deve ser do tipo String, ArrayBuffer!");
        }

        // Tem que colocar a resposta nessa const pq se retornar a chamda da função, o array buffer é apagado no meio 
        // do processo, corrompendo os dados.
        const response = callUserFunc.sync("createFile", filepath, content);

        return response;

    }

})