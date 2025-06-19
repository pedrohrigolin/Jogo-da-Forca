// Guarda o objeto window.ForcaApp em uma constante, uma vez que ele é bastante usado.
const ForcaApp = window.ForcaApp;

/**
 * Objecto para fazer as chamadas de funções para o C++, tanto de forma 
 * síncrona quanto assíncrona.
 * 
 * O nome do objeto vem da função call_user_func() do PHP, porém, a implementação é
 * completamente diferente.
 * 
 * @author Pedro Rigolin
*/
const callUserFunc = {

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