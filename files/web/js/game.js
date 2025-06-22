const initializeObject = Object.freeze({

    elements: {

        body: document.getElementById("initializeBody"),
        spinner: document.getElementById("initializeSpinner"),
        staticText: document.getElementById("initializeStaticText"),
        statusText: document.getElementById("initializeStatusText")

    },

    changeStatus: function(status){

        if( typeof status !== 'string' ) status = String.toString(status);

        initializeObject.statusElement.textContent = status;

    },

    init: function(){


    }

});