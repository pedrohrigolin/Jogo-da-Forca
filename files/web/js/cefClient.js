// Guarda o objeto window.ForcaApp em uma constante, uma vez que ele é bastante usado.
const ForcaApp = window.ForcaApp;

/**
 * @namespace callUserFunc
 * @description
 * Objeto utilitário responsável por realizar chamadas de funções do backend C++ a partir do JavaScript,
 * tanto de forma síncrona quanto assíncrona, utilizando a ponte CEF (Chromium Embedded Framework).
 * 
 * O nome do objeto faz referência à função call_user_func() do PHP, mas sua implementação é específica para integração JS/C++.
 * 
 * Fornece métodos para chamadas síncronas (sync), assíncronas (async), chamadas sem retorno (sendToCpp) e gerenciamento de Promises (cefResolve).
 * Utiliza o objeto global ForcaApp, exposto pelo CEF, para executar funções nativas do backend.
 * 
 */
const callUserFunc = {

    /**
     * @function sendToCpp
     * @memberof callUserFunc
     * @description
     * Chama uma função do backend C++ de forma síncrona, sem esperar retorno de valor.
     * Utiliza o objeto global ForcaApp, que é exposto pelo CEF, para executar funções nativas do C++.
     * Lança erro se a função não existir ou se ForcaApp não estiver disponível.
     *
     * @param {string} functionName - Nome da função C++ a ser chamada.
     * @param {...any} args - Argumentos a serem passados para a função C++.
     * @returns {boolean} true se a chamada foi realizada com sucesso.
     * @throws {Error} Se functionName não for string ou se a função não existir no backend.
     */
    sendToCpp: function( functionName, ...args ){

        if( typeof functionName !== "string" ){

            throw new Error("O parâmetro name passado na função deve ser do tipo String!");

        }

        // TODO: ISSO AQUI PODE SER MELHORADO, TALVEZ JÁ RETORNAR ERRO DA APLICAÇÃO LOGO QUANDO FOR DETECTADO 
        // TODO: QUE O FORCAAPP NÃO FOI INICIADO NO WINDOWS.
        if (ForcaApp && ForcaApp.salvarUsuario) {

            ForcaApp[functionName](...args);
        
            return true;

        }
        else{

            throw new Error( "Erro: a API C++ (ForcaApp." + functionName + ") não foi encontrada!" );
            
        }

    },

    /**
     * @function async
     * @memberof callUserFunc
     * @description
     * Chama uma função do backend C++ de forma assíncrona, retornando uma Promise.
     * Utiliza o objeto global ForcaApp para executar funções nativas do C++ e gerencia a resolução/rejeição da Promise.
     * Lança erro se a função não existir ou se ForcaApp não estiver disponível.
     *
     * @param {string} functionName - Nome da função C++ a ser chamada.
     * @param {...any} args - Argumentos a serem passados para a função C++.
     * @returns {Promise<any>} Promise resolvida com o resultado da função ou rejeitada em caso de erro.
     * @throws {Error} Se functionName não for string.
     */
    async: function( functionName, ...args ){

        if( typeof functionName !== "string" ){

            throw new Error("O parâmetro name passado na função deve ser do tipo String!");

        }

        if ( ! ForcaApp || typeof ForcaApp[functionName] !== 'function' ) {
            return Promise.reject(new Error(`API C++ (ForcaApp.${functionName}) não foi encontrada!`));
        }

        return new Promise((resolve, reject) => {

            const promiseId = `promise-${callUserFunc.promiseCounter++}`;

            callUserFunc.pendingPromises[promiseId] = { resolve, reject };

            try {

                // Chama a função C++, passando os argumentos originais e o ID da promise por último.
                ForcaApp[functionName](promiseId, ...args);

            } catch (e) {

                // Em caso de erro na própria chamada da ponte, rejeita a promise.
                callUserFunc.cefResolve(promiseId, false, e.message);

            }

        });

    },

    /**
     * @function sync
     * @memberof callUserFunc
     * @description
     * Chama uma função do backend C++ de forma síncrona e retorna o valor retornado pela função C++.
     * Utiliza o objeto global ForcaApp para executar funções nativas do C++.
     * Lança erro se a função não existir ou se ForcaApp não estiver disponível.
     *
     * @param {string} functionName - Nome da função C++ a ser chamada.
     * @param {...any} args - Argumentos a serem passados para a função C++.
     * @returns {any} Valor retornado pela função C++.
     * @throws {Error} Se functionName não for string ou se a função não existir no backend.
     */
    sync: function (functionName, ...args) {
        
        if( typeof functionName !== "string" ){

            throw new Error("O parâmetro name passado na função deve ser do tipo String!");

        }

        if ( ! ForcaApp || typeof ForcaApp[functionName] !== 'function' ) {
            throw new Error(`API C++ (ForcaApp.${functionName}) não foi encontrada!`);
        }
        
        return ForcaApp[functionName](...args);

    },

    promiseCounter: 0,

    pendingPromises: {},

    /**
     * @function cefResolve
     * @memberof callUserFunc
     * @description
     * Resolve ou rejeita uma Promise pendente criada por uma chamada assíncrona ao backend C++.
     * Utilizada internamente para comunicação entre o backend C++ e o JavaScript, garantindo que o resultado
     * da operação assíncrona seja entregue corretamente à Promise correspondente.
     *
     * @param {string} promiseId - Identificador único da Promise.
     * @param {boolean} success - true para resolver a Promise, false para rejeitar.
     * @param {any} resultOrError - Valor de resolução ou mensagem de erro.
     * @throws {Error} Se a Promise com o ID informado não for encontrada.
     */
    cefResolve: function( promiseId, success, resultOrError ){

        if ( ! callUserFunc.pendingPromises[promiseId] ) {
            
            throw new Error(`Promise com ID ${promiseId} não encontrada.`);

        }

        const promise = callUserFunc.pendingPromises[promiseId];

        if (success) {
            promise.resolve(resultOrError);
        } else {
            promise.reject(new Error(resultOrError));
        }

        // Limpa a referência para evitar vazamento de memória
        delete callUserFunc.pendingPromises[promiseId];

    },

};

// --- Funções (imutáveis) ---
Object.defineProperty(callUserFunc, 'sync', {
    configurable: false,
    enumerable: false,
    writable: false
});

Object.defineProperty(callUserFunc, 'sendToCpp', {
    configurable: false,
    enumerable: false,
    writable: false
});

Object.defineProperty(callUserFunc, 'async', {
    configurable: false,
    enumerable: false,
    writable: false
});

Object.defineProperty(callUserFunc, 'cefResolve', {
    configurable: false,
    enumerable: false,
    writable: false
});

// --- Propriedades mutáveis (mas não deletáveis) ---
Object.defineProperty(callUserFunc, 'promiseCounter', {
    configurable: false,
    enumerable: false,
    writable: true
});

Object.defineProperty(callUserFunc, 'pendingPromises', {
    configurable: false,
    enumerable: false,
    writable: true
});