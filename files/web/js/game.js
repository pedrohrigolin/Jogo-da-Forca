const resourcesObject = {

    words: {}

};

const interfaceObject = {};

const initializeObject = Object.freeze({

    elements: {

        body: () => document.getElementById("initializeBody"),
        spinner: () => document.getElementById("initializeSpinner"),
        staticText: () => document.getElementById("initializeStaticText"),
        statusText: () => document.getElementById("initializeStatusText")

    },

    changeStatus: function(status){

        if( typeof status !== 'string' ) status = String.toString(status);

        initializeObject.elements.statusText().textContent = status;

    },

    init: function(){

        initializeObject.changeStatus("Carregando recursos...");

        initializeObject.changeStatus("Carregando palavras...");

        try {
            
            let tempObj = JSON.parse( ForcaFiles.getJSONContent("../files/words.json") );

            if( ! tempObj.hasOwnProperty("easy") ) 
                throw new Error("O arquivo JSON de palavras do Jogo da Forca não contém as palavras da dificuldade fácil.");
            if( ! tempObj.hasOwnProperty("normal") ) 
                throw new Error("O arquivo JSON de palavras do Jogo da Forca não contém as palavras da dificuldade normal.");
            if( ! tempObj.hasOwnProperty("hard") ) 
                throw new Error("O arquivo JSON de palavras do Jogo da Forca não contém as palavras da dificuldade difícil.");

            resourcesObject.words = tempObj;

        } catch (error) {
            
            initializeObject.changeStatus("Erro ao carregar palavras, tente reiniciar o Jogo da Forca!");
            
            throw new Error("\n\n\t" + error + "\n");

        }

        Object.freeze(resourcesObject);

        initializeObject.changeStatus("Criando a interface gráfica...");

    }

});