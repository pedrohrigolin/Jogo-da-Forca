const resourcesObject = {

    words: {
        default: {},
        custom: {}
    }

};

const interfaceObject = {

    handlers: {},

    timeouts: {}

};

const gameObject = {

    word: "",

    letrasCorretas: [],
    
    letrasErradas: [],

    erros: 0,

    initialized: false,

    mostrasTracos: function() {

        if( ! gameObject.initialized ) return;

        const wordLength = gameObject.word.length;

        const textContent = "_".repeat(wordLength);

        interfaceObject.menu.forca.tracosContainer.textContent = textContent;

    },

    mostrarErro: function() {

        if( ! gameObject.initialized ) return;

        const partesLength = interfaceObject.menu.forca.partes.length;

        if( gameObject.erros <= partesLength ){

            const parte = interfaceObject.menu.forca.partes[gameObject.erros - 1];

            if( parte ) parte.style.display = "block";

        }

    },

    verificarFimDeJogo: function() {

        if( ! gameObject.initialized ) return;

        if( ! interfaceObject.menu.forca.tracosContainer.textContent.includes("_") ){

            interfaceObject.menu.forca.mensagemFinal.textContent = "Parabéns! Você venceu! A palavra era: " + gameObject.word;

            interfaceObject.menu.forca.input.value = "";

            interfaceObject.menu.forca.input.blur();

            interfaceObject.menu.forca.input.disabled = true;

            const partesLength = interfaceObject.menu.forca.partes.length;

            for(let i = 0; i < partesLength; i++){

                const parte = interfaceObject.menu.forca.partes[i];

                if( parte ) parte.style.display = "none";

            }

            gameObject.word = "";

            gameObject.letrasCorretas = [];

            gameObject.letrasErradas = [];

            gameObject.erros = 0;

            gameObject.initialized = false;

            interfaceObject.menu.forca.letrasErradas.textContent = "";

            interfaceObject.menu.forca.tracosContainer.textContent = "";

        }
        else if( gameObject.erros >= 6 ){

            interfaceObject.menu.forca.mensagemFinal.textContent = "Game over! A palavra era: " + gameObject.word;

            interfaceObject.menu.forca.input.value = "";

            interfaceObject.menu.forca.input.blur();

            interfaceObject.menu.forca.input.disabled = true;

            gameObject.word = "";

            gameObject.letrasCorretas = [];

            gameObject.letrasErradas = [];

            gameObject.erros = 0;

            gameObject.initialized = false;

            interfaceObject.menu.forca.letrasErradas.textContent = "";

            interfaceObject.menu.forca.tracosContainer.textContent = "";

        }

    },

    verificarLetras: function(letter) {

        if( ! gameObject.initialized ) return;

        if( typeof letter !== "string" || letter.length !== 1 ){

            alert("Digite uma letra válida!");

            interfaceObject.menu.forca.input.value = "";

            interfaceObject.menu.forca.input.focus();

            return;

        }

        letter = letter.normalizeWord();

        if( ! letter.isAlpha() ){

            alert("Digite uma letra válida!");

            interfaceObject.menu.forca.input.value = "";

            interfaceObject.menu.forca.input.focus();

            return;

        }

        if( gameObject.letrasCorretas.includes(letter) || gameObject.letrasErradas.includes(letter) ){

            alert("Você já tentou essa letra!");

            interfaceObject.menu.forca.input.value = "";

            interfaceObject.menu.forca.input.focus();

            return;

        }

        if( gameObject.word.includes(letter) ){

            gameObject.letrasCorretas.push(letter);

            const index = gameObject.word.searchAll(letter);

            const indexLength = index.length;

            let textContent = interfaceObject.menu.forca.tracosContainer.textContent;

            for(let i = 0; i < indexLength; i++){

                const letterIndex = index[i];

                let firstSubstring;

                if( letterIndex === 0 ){

                    firstSubstring = "";

                }
                else {

                    firstSubstring = textContent.substr(0, letterIndex);

                }

                textContent = firstSubstring + letter + textContent.substr(letterIndex + 1, textContent.length);

            }

            interfaceObject.menu.forca.tracosContainer.textContent = textContent;

            interfaceObject.menu.forca.input.value = "";

            interfaceObject.menu.forca.input.focus();

            gameObject.verificarFimDeJogo();

        }
        else {

            gameObject.letrasErradas.push(letter);

            gameObject.erros++;

            interfaceObject.menu.forca.letrasErradas.textContent = gameObject.letrasErradas.implode(", ");

            gameObject.mostrarErro();

            interfaceObject.menu.forca.input.value = "";

            interfaceObject.menu.forca.input.focus();

            gameObject.verificarFimDeJogo();

        }

    },

    resetDisable: function() {

        gameObject.letrasCorretas = [];

        gameObject.letrasErradas = [];

        gameObject.erros = 0;

        interfaceObject.menu.forca.input.disabled = true;

        gameObject.initialized = false;

        interfaceObject.menu.forca.input.value = "";

        interfaceObject.menu.forca.input.blur();

        interfaceObject.menu.forca.letrasErradas.textContent = "";

        interfaceObject.menu.forca.mensagemFinal.textContent = "";

        interfaceObject.menu.forca.tracosContainer.textContent = "";

        const partesLength = interfaceObject.menu.forca.partes.length;

        for(let i = 0; i < partesLength; i++){

            const parte = interfaceObject.menu.forca.partes[i];

            if( parte ) parte.style.display = "none";

        }

    },

    init: function() {

        gameObject.letrasCorretas = [];

        gameObject.letrasErradas = [];

        gameObject.erros = 0;

        interfaceObject.menu.forca.input.disabled = false;

        gameObject.initialized = true;

        interfaceObject.menu.forca.input.value = "";

        interfaceObject.menu.forca.input.focus();

        interfaceObject.menu.forca.letrasErradas.textContent = "";

        interfaceObject.menu.forca.mensagemFinal.textContent = "";

        interfaceObject.menu.forca.tracosContainer.textContent = "";

        const partesLength = interfaceObject.menu.forca.partes.length;

        for(let i = 0; i < partesLength; i++){

            const parte = interfaceObject.menu.forca.partes[i];

            if( parte ) parte.style.display = "none";

        }

        gameObject.mostrasTracos();

    },

};

const utilsObject = Object.freeze({

    filterWordsArray: function(array){

        if(typeof array !== "object" || ! Array.isArray(array))
            throw new TypeError("O parâmetro array deve ser do tipo array!");

        if(array.empty()) return [];

        const copyArray = array;

        const arrayLength = copyArray.length;

        for(let i = (arrayLength - 1); i >= 0; i--){

            if(typeof copyArray[i] !== 'string') copyArray.splice(i, 1);

            copyArray[i] = copyArray[i].preg_replace(/[^a-záéíóúàèìòùâêîôûãõç]/iug, "");

            copyArray[i] = copyArray[i].normalizeWord();

            if(copyArray[i].empty()) copyArray.splice(i, 1);

        }

        return copyArray;

    }

});

const initializeObject = Object.freeze({

    elements: {

        body: () => document.getElementById("initializeBody"),
        spinner: () => document.getElementById("initializeSpinner"),
        staticText: () => document.getElementById("initializeStaticText"),
        statusText: () => document.getElementById("initializeStatusText"),
        subStatusText: () => document.getElementById("initializeSubStatusText")

    },

    changeStatus: function(status){

        if( typeof status !== 'string' ) status = status.toString();

        initializeObject.elements.statusText().textContent = status;

    },

    changeSubStatus: function(status){

        if( typeof status !== 'string' ) status = status.toString();

        initializeObject.elements.subStatusText().textContent = status;

    }, 

    svg: {

        caveira: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "209");
            svg.setAttribute("height", "311");
            svg.setAttribute("viewBox", "0 0 209 311");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            const path1 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path1.setAttribute("d", "M178 254C168.4 258.8 161 257 158.5 255.5L46.5 211C40.1 208.2 38.8333 203.833 39 202C39.4 184.4 24.8333 184.333 17.5 186.5C3.9 194.1 9.83334 206 14.5 211C2.9 215 0.666671 224 1 228C2.6 246.8 17.6667 243.5 25 239.5C31.4 232.7 38.3333 234.333 41 236L142 278.5C152.4 280.5 153.667 286.333 153 289C152.2 305.8 164 310 170 310C190.8 308.4 188.333 292 184.5 284C204.5 278.4 201.833 264.333 198 258C195.333 254.667 187.6 249.2 178 254Z");
            path1.setAttribute("fill", "#EAEAEA");
            svg.appendChild(path1);

            const path2 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path2.setAttribute("d", "M31.6869 254.468C41.2869 259.268 48.6869 257.468 51.1869 255.968L163.187 211.468C169.587 208.668 170.854 204.302 170.687 202.468C170.287 184.868 184.854 184.802 192.187 186.968C205.787 194.568 199.854 206.468 195.187 211.468C206.787 215.468 209.02 224.468 208.687 228.468C207.087 247.268 192.02 243.968 184.687 239.968C178.287 233.168 171.354 234.802 168.687 236.468L67.6869 278.968C57.2869 280.968 56.0202 286.802 56.6869 289.468C57.4869 306.268 45.6869 310.468 39.6869 310.468C18.8869 308.868 21.3535 292.468 25.1869 284.468C5.18687 278.868 7.85355 264.802 11.6869 258.468C14.3535 255.135 22.0869 249.668 31.6869 254.468Z");
            path2.setAttribute("fill", "#EAEAEA");
            svg.appendChild(path2);

            const path3 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path3.setAttribute("d", "M72.5951 197.526C77.7951 208.726 103.762 209.86 116.095 209.026C134.595 208.026 156.095 200.026 156.595 186.526C157.095 173.026 156.595 172.026 171.595 164.026C186.595 156.026 184.595 146.526 182.095 135.026C179.595 123.526 182.095 124.526 182.095 120.526C182.095 116.526 192.595 100.026 178.595 60.0262C164.595 20.0262 123.095 4.0263 111.095 2.5263C99.0951 1.0263 69.0951 -7.9737 35.0951 20.5263C1.09511 49.0263 4.09511 90.0263 6.09511 108.526C8.09511 127.026 21.0951 141.026 22.5951 142.526C24.0951 144.026 24.0951 153.026 25.5951 157.526C27.0951 162.026 33.5951 170.526 48.0951 171.526C62.5951 172.526 66.0951 183.526 72.5951 197.526Z");
            path3.setAttribute("fill", "#EAEAEA");
            svg.appendChild(path3);

            const path4 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path4.setAttribute("d", "M101.5 153.5L112 134.5C116 129.3 120 132.333 121.5 134.5L128 149C132.8 155.8 130 160.167 128 161.5C125.2 163.1 121.5 160.833 120 159.5C116 156.7 113.667 158.333 113 159.5C109 163.1 105.333 162.333 104 161.5C100.8 160.3 101 155.667 101.5 153.5Z");
            path4.setAttribute("fill", "#1B1919");
            path4.setAttribute("stroke", "black");
            path4.setAttribute("stroke-width", "3");
            svg.appendChild(path4);

            const path5 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path5.setAttribute("d", "M64.5 90C39.7 107.2 49.5 127.833 57.5 136C61.5 139.333 72.1 144.5 82.5 138.5C95.5 131 104 117 96.5 98C90.5 82.8 72.6667 86.3334 64.5 90Z");
            path5.setAttribute("fill", "#1B1919");
            path5.setAttribute("stroke", "black");
            path5.setAttribute("stroke-width", "3");
            svg.appendChild(path5);

            const path6 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path6.setAttribute("d", "M150.5 138C127.7 135.6 127 110.333 129.5 97.9999C130.187 92.7999 136.786 89.1666 140 87.9999C162.4 83.9999 169.333 99.6666 170 108C171.6 133.6 157.667 138.667 150.5 138Z");
            path6.setAttribute("fill", "#1B1919");
            path6.setAttribute("stroke", "black");
            path6.setAttribute("stroke-width", "3");
            svg.appendChild(path6);

            const mask1 = document.createElementNS("http://www.w3.org/2000/svg", "mask");
            mask1.setAttribute("id", "mask0_7_237");
            mask1.setAttribute("style", "mask-type:alpha");
            mask1.setAttribute("maskUnits", "userSpaceOnUse");
            mask1.setAttribute("x", "93");
            mask1.setAttribute("y", "179");
            mask1.setAttribute("width", "12");
            mask1.setAttribute("height", "33");

            const mask1Path = document.createElementNS("http://www.w3.org/2000/svg", "path");
            mask1Path.setAttribute("d", "M96.5 180.5C93.7 185.3 93.6667 202.833 94 211L105 212C103.333 199.5 99.3 175.7 96.5 180.5Z");
            mask1Path.setAttribute("fill", "#D9D9D9");
            mask1.appendChild(mask1Path);
            svg.appendChild(mask1);

            const g1 = document.createElementNS("http://www.w3.org/2000/svg", "g");
            g1.setAttribute("mask", "url(#mask0_7_237)");

            const g1Path = document.createElementNS("http://www.w3.org/2000/svg", "path");
            g1Path.setAttribute("d", "M72.5951 197.526C77.7951 208.726 103.762 209.86 116.095 209.026C134.595 208.026 156.095 200.026 156.595 186.526C157.095 173.026 156.595 172.026 171.595 164.026C186.595 156.026 184.595 146.526 182.095 135.026C179.595 123.526 182.095 124.526 182.095 120.526C182.095 116.526 192.595 100.026 178.595 60.0262C164.595 20.0262 123.095 4.0263 111.095 2.5263C99.0951 1.0263 69.0951 -7.9737 35.0951 20.5263C1.09511 49.0263 4.09511 90.0263 6.09511 108.526C8.09511 127.026 21.0951 141.026 22.5951 142.526C24.0951 144.026 24.0951 153.026 25.5951 157.526C27.0951 162.026 33.5951 170.526 48.0951 171.526C62.5951 172.526 66.0951 183.526 72.5951 197.526Z");
            g1Path.setAttribute("fill", "#D9D9D9");
            g1.appendChild(g1Path);
            svg.appendChild(g1);

            const path7 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path7.setAttribute("d", "M74.5 172C79.3 191.2 86.8333 203.333 90 207C77.6 205 70.5 197.833 68.5 194.5C63.7 175.7 54.3333 171.667 51.5 172L47.5 171L43.5 170L39 169.333C25.8 167.733 23.3333 150.833 23.5 142.833C7.50001 126.033 0.682915 100 2 88C3 60.5 12.7828 37.426 29.5 22.5C43.5 10 62.6667 3.33333 74.5 1.5C31.7 19.9 21.6667 53.5 19.5 70.5C13.5 102.9 26.6667 119.667 34 124V136C34.1667 143.5 38.4 158.9 54 160.5C69.6 162.1 74.1667 168.833 74.5 172Z");
            path7.setAttribute("fill", "#D9D9D9");
            svg.appendChild(path7);

            const path8 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path8.setAttribute("d", "M51.5 286C52.7 303.2 41 308.5 35 309C51 310.2 56 302.833 56.5 299C55.3 287.4 59.6667 283.167 62 282.5C94.5 268.5 160.7 240 165.5 238C171.5 235.5 176.5 234.5 185.5 241.5C192.7 247.1 199.833 243.167 202.5 240.5C196 242.5 191.5 243.5 184 234.5C178 227.3 169.167 228.167 165.5 229.5C137.167 242.333 77.4 269.1 65 273.5C52.6 277.9 50.8333 283.667 51.5 286Z");
            path8.setAttribute("fill", "#D9D9D9");
            svg.appendChild(path8);

            const path9 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path9.setAttribute("d", "M120 262.5L144.5 250L140.5 247L105 263C112.667 267 131.9 275.3 141.5 278.5C152.3 282.1 152 288.5 152.5 292C152.5 307.2 164.833 310 170.5 310C157 305.5 155.5 293.5 157 286.5C158.2 280.9 152.5 276.833 149.5 275.5L120 262.5Z");
            path9.setAttribute("fill", "#D9D9D9");
            svg.appendChild(path9);

            const path10 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path10.setAttribute("d", "M43.5 229.5L81.5 245L70.5 250C65.6667 247.667 53.5 242 43.5 238C31 233 33 236.5 21 242C11.4 246.4 4.66667 239.167 2.5 235C5 238 13.5 243.5 23 233.5C30.6 225.5 39.8333 227.5 43.5 229.5Z");
            path10.setAttribute("fill", "#D9D9D9");
            svg.appendChild(path10);

            return svg;

        },

        forca: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "160");
            svg.setAttribute("height", "387");
            svg.setAttribute("viewBox", "0 0 160 387");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            const path1 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path1.setAttribute("d", "M21.2186 307.5C9.61855 323.5 6.71855 338.5 6.71855 344C1.11855 330.8 0.051885 316.833 0.218552 311.5C4.21855 304.3 17.2186 295.833 23.2186 292.5C21.6186 298.5 21.2186 305 21.2186 307.5Z");
            path1.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path1);

            const path2 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path2.setAttribute("d", "M28.2186 280.5C15.4186 281.7 5.55188 285.667 2.21855 287.5C2.21855 281.5 6.88522 273.333 9.21855 270C16.0186 264.8 37.3852 259.167 47.2186 257C34.4186 268.2 29.2186 277.333 28.2186 280.5Z");
            path2.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path2);

            const path3 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path3.setAttribute("d", "M64.2186 235.5C57.8186 243.5 32.8852 250.5 21.2186 253L51.2186 217.5L77.2186 214.5C75.5519 218.167 70.6186 227.5 64.2186 235.5Z");
            path3.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path3);

            const path4 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path4.setAttribute("d", "M93.2186 78.5L64.7186 107.5V111H93.2186V78.5Z");
            path4.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path4);

            const path5 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path5.setAttribute("d", "M93.2186 64L64.7186 92V64L93.2186 34V64Z");
            path5.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path5);

            const path6 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path6.setAttribute("d", "M91.7186 20.5L64.7186 42.5V17L78.2186 0H91.7186V20.5Z");
            path6.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path6);

            const path7 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path7.setAttribute("d", "M98.7186 203.5C90.7186 209.9 87.7186 221.833 87.2186 227C80.4186 216.6 79.3852 207 79.7186 203.5H98.7186Z");
            path7.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path7);

            const path8 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path8.setAttribute("d", "M41.7186 191.5C36.9186 180.7 46.3852 173.333 51.7186 171C48.8852 169.833 42.9186 165.3 41.7186 156.5C40.5186 147.7 48.5519 142.833 52.7186 141.5C50.2186 141.667 44.8186 139.2 43.2186 128C41.6186 116.8 51.2186 112.333 56.2186 111.5C62.5519 111.167 79.1186 110.7 94.7186 111.5C114.219 112.5 113.719 119 115.219 126C116.419 131.6 110.052 139 106.719 142C108.552 142.5 112.819 145.6 115.219 154C117.619 162.4 110.552 168.833 106.719 171C115.519 173.8 117.052 182.167 116.719 186C116.719 207 86.7186 203.5 79.2186 203C71.7186 202.5 47.7186 205 41.7186 191.5Z");
            path8.setAttribute("fill", "#D7AF6F");
            path8.setAttribute("stroke", "#B89257");
            path8.setAttribute("stroke-width", "10");
            svg.appendChild(path8);

            const path9 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path9.setAttribute("d", "M121.719 236.5L119.719 264.5C110.919 256.5 102.719 247.5 99.7186 244L104.719 214C111.119 224.8 118.719 233.5 121.719 236.5Z");
            path9.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path9);

            const path10 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path10.setAttribute("d", "M152.719 276.5C153.519 281.7 142.719 294.667 137.219 300.5C137.219 286.9 131.219 277.833 128.219 275L135.219 249C140.719 256 151.919 271.3 152.719 276.5Z");
            path10.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path10);

            const path11 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path11.setAttribute("d", "M36.7186 345.5V373C26.7186 366.6 20.2186 359.667 18.2186 357C17.0186 344.2 21.0519 326 23.2186 318.5C25.6186 333.7 33.2186 342.833 36.7186 345.5Z");
            path11.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path11);

            const path12 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path12.setAttribute("d", "M68.7186 363C57.5186 361.4 49.3852 356.333 46.7186 354C45.5186 368.4 52.8852 379.667 56.7186 383.5C74.3186 388.7 92.3852 385.667 99.2186 383.5C84.0186 380.7 72.5519 368.667 68.7186 363Z");
            path12.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path12);

            const path13 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path13.setAttribute("d", "M112.719 354C103.919 360 93.3852 362.5 89.2186 363C98.4186 373.4 112.052 376.667 117.719 377C134.919 369 145.552 352.667 148.719 345.5C137.919 352.7 120.219 354.167 112.719 354Z");
            path13.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path13);

            const path14 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path14.setAttribute("d", "M156.719 325C155.119 331.8 135.385 338.5 125.719 341C133.319 335.8 137.219 322.167 138.219 316C148.219 312.8 156.385 301 159.219 295.5C160.219 299.5 158.719 316.5 156.719 325Z");
            path14.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path14);

            return svg;

        },

        easy: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "213");
            svg.setAttribute("height", "210");
            svg.setAttribute("viewBox", "0 0 213 210");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            const ellipse1 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse1.setAttribute("cx", "106.5");
            ellipse1.setAttribute("cy", "105");
            ellipse1.setAttribute("rx", "106.5");
            ellipse1.setAttribute("ry", "105");
            ellipse1.setAttribute("fill", "#1CD428");
            svg.appendChild(ellipse1);

            const ellipse2 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse2.setAttribute("cx", "68.5");
            ellipse2.setAttribute("cy", "54.5");
            ellipse2.setAttribute("rx", "68.5");
            ellipse2.setAttribute("ry", "54.5");
            ellipse2.setAttribute("transform", "matrix(1 0 0 -1 37 167)");
            ellipse2.setAttribute("fill", "black");
            svg.appendChild(ellipse2);

            const ellipse3 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse3.setAttribute("cx", "68.5");
            ellipse3.setAttribute("cy", "58.5");
            ellipse3.setAttribute("rx", "68.5");
            ellipse3.setAttribute("ry", "58.5");
            ellipse3.setAttribute("transform", "matrix(1 0 0 -1 37 159)");
            ellipse3.setAttribute("fill", "#1CD428");
            svg.appendChild(ellipse3);

            const circle1 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle1.setAttribute("cx", "58");
            circle1.setAttribute("cy", "72");
            circle1.setAttribute("r", "21");
            circle1.setAttribute("fill", "black");
            svg.appendChild(circle1);

            const circle2 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle2.setAttribute("cx", "156");
            circle2.setAttribute("cy", "72");
            circle2.setAttribute("r", "21");
            circle2.setAttribute("fill", "black");
            svg.appendChild(circle2);

            const circle3 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle3.setAttribute("cx", "54");
            circle3.setAttribute("cy", "84");
            circle3.setAttribute("r", "21");
            circle3.setAttribute("fill", "#1CD428");
            svg.appendChild(circle3);

            const circle4 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle4.setAttribute("cx", "160");
            circle4.setAttribute("cy", "84");
            circle4.setAttribute("r", "21");
            circle4.setAttribute("fill", "#1CD428");
            svg.appendChild(circle4);

            return svg;

        },

        normal: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "213");
            svg.setAttribute("height", "210");
            svg.setAttribute("viewBox", "0 0 213 210");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            const ellipse1 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse1.setAttribute("cx", "106.5");
            ellipse1.setAttribute("cy", "105");
            ellipse1.setAttribute("rx", "106.5");
            ellipse1.setAttribute("ry", "105");
            ellipse1.setAttribute("fill", "#D4CE1C");
            svg.appendChild(ellipse1);

            const ellipse2 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse2.setAttribute("cx", "68.5");
            ellipse2.setAttribute("cy", "37.5");
            ellipse2.setAttribute("rx", "68.5");
            ellipse2.setAttribute("ry", "37.5");
            ellipse2.setAttribute("transform", "matrix(1 0 0 -1 37 167)");
            ellipse2.setAttribute("fill", "black");
            svg.appendChild(ellipse2);

            const ellipse3 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse3.setAttribute("cx", "80");
            ellipse3.setAttribute("cy", "38");
            ellipse3.setAttribute("rx", "80");
            ellipse3.setAttribute("ry", "38");
            ellipse3.setAttribute("transform", "matrix(1 0 0 -1 26 158)");
            ellipse3.setAttribute("fill", "#D4CE1C");
            svg.appendChild(ellipse3);

            const circle1 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle1.setAttribute("cx", "58");
            circle1.setAttribute("cy", "72");
            circle1.setAttribute("r", "21");
            circle1.setAttribute("fill", "black");
            svg.appendChild(circle1);

            const circle2 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle2.setAttribute("cx", "156");
            circle2.setAttribute("cy", "72");
            circle2.setAttribute("r", "21");
            circle2.setAttribute("fill", "black");
            svg.appendChild(circle2);

            return svg;

        },

        hard: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "213");
            svg.setAttribute("height", "210");
            svg.setAttribute("viewBox", "0 0 213 210");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            const ellipseBg = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipseBg.setAttribute("cx", "106.5");
            ellipseBg.setAttribute("cy", "105");
            ellipseBg.setAttribute("rx", "106.5");
            ellipseBg.setAttribute("ry", "105");
            ellipseBg.setAttribute("fill", "#D41C1C");
            svg.appendChild(ellipseBg);

            const pathMouth1 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            pathMouth1.setAttribute("d", "M176.5 166.5C176.5 196.6 145.832 182.5 108 182.5C70.1685 182.5 39.5 196.6 39.5 166.5C39.5 136.4 70.1685 112 108 112C145.832 112 176.5 136.4 176.5 166.5Z");
            pathMouth1.setAttribute("fill", "black");
            svg.appendChild(pathMouth1);

            const pathMouth2 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            pathMouth2.setAttribute("d", "M176 172.5C176 204.809 145.832 185 108 185C70.1685 185 39 204.809 39 172.5C39 140.191 70.1685 120 108 120C145.832 120 176 140.191 176 172.5Z");
            pathMouth2.setAttribute("fill", "#D41C1C");
            svg.appendChild(pathMouth2);

            const ellipseLeftEye = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipseLeftEye.setAttribute("cx", "68");
            ellipseLeftEye.setAttribute("cy", "74.5");
            ellipseLeftEye.setAttribute("rx", "25");
            ellipseLeftEye.setAttribute("ry", "24.5");
            ellipseLeftEye.setAttribute("fill", "black");
            svg.appendChild(ellipseLeftEye);

            const ellipseRightEye = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipseRightEye.setAttribute("cx", "151");
            ellipseRightEye.setAttribute("cy", "72");
            ellipseRightEye.setAttribute("rx", "26");
            ellipseRightEye.setAttribute("ry", "27");
            ellipseRightEye.setAttribute("fill", "black");
            svg.appendChild(ellipseRightEye);

            const pathLeftHorn = document.createElementNS("http://www.w3.org/2000/svg", "path");
            pathLeftHorn.setAttribute("d", "M98 35V96L40 49.0769L98 35Z");
            pathLeftHorn.setAttribute("fill", "#D41C1C");
            svg.appendChild(pathLeftHorn);

            const pathRightHorn = document.createElementNS("http://www.w3.org/2000/svg", "path");
            pathRightHorn.setAttribute("d", "M116 32V99L176 47.4615L116 32Z");
            pathRightHorn.setAttribute("fill", "#D41C1C");
            svg.appendChild(pathRightHorn);

            return svg;

        },

        solo: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "256");
            svg.setAttribute("height", "256");
            svg.setAttribute("viewBox", "0 0 256 256");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            const ellipse1 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse1.setAttribute("cx", "130.5");
            ellipse1.setAttribute("cy", "145");
            ellipse1.setAttribute("rx", "106.5");
            ellipse1.setAttribute("ry", "105");
            ellipse1.setAttribute("fill", "#6D4C41");
            svg.appendChild(ellipse1);

            const path1 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path1.setAttribute("d", "M70.0284 215.321C69.5738 201.713 74.3929 172.07 97.3061 162.361C120.219 152.652 148.679 158.315 160.045 162.361C170.804 165.119 192.051 179.573 190.96 215.321C189.596 260.006 72.7562 255.041 70.0284 215.321Z");
            path1.setAttribute("fill", "black");
            svg.appendChild(path1);

            const ellipse2 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse2.setAttribute("cx", "129.5");
            ellipse2.setAttribute("cy", "115");
            ellipse2.setAttribute("rx", "53.5");
            ellipse2.setAttribute("ry", "53");
            ellipse2.setAttribute("fill", "#6D4C41");
            svg.appendChild(ellipse2);

            const ellipse3 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse3.setAttribute("cx", "129.5");
            ellipse3.setAttribute("cy", "120");
            ellipse3.setAttribute("rx", "43.5");
            ellipse3.setAttribute("ry", "43");
            ellipse3.setAttribute("fill", "black");
            svg.appendChild(ellipse3);

            const path2 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path2.setAttribute("d", "M130.5 45C186.624 45 232 89.8382 232 145C232 200.162 186.624 245 130.5 245C74.376 245 29 200.162 29 145C29 89.8382 74.376 45 130.5 45Z");
            path2.setAttribute("stroke", "#6D4C41");
            path2.setAttribute("stroke-width", "10");
            svg.appendChild(path2);

            return svg;

        },

        duo: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "279");
            svg.setAttribute("height", "259");
            svg.setAttribute("viewBox", "0 0 279 259");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            const ellipse1 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse1.setAttribute("cx", "154.143");
            ellipse1.setAttribute("cy", "133.504");
            ellipse1.setAttribute("rx", "124.518");
            ellipse1.setAttribute("ry", "125.102");
            ellipse1.setAttribute("fill", "#6D4C41");
            svg.appendChild(ellipse1);

            const path1 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path1.setAttribute("d", "M64.7801 213.099C86.9988 221.501 100.853 250.203 125.906 250.203C156.457 250.203 164.302 241.574 164.302 213.099C164.302 165.663 139.826 148.441 114.772 148.441C89.7188 148.441 64.7801 165.663 64.7801 213.099Z");
            path1.setAttribute("fill", "black");
            svg.appendChild(path1);

            const path2 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path2.setAttribute("d", "M64.8049 210.525C64.3841 197.821 68.8715 160.644 90.0842 151.579C111.297 142.515 137.644 147.802 148.166 151.579C158.127 154.154 177.797 167.648 176.787 201.022C175.524 242.74 67.3303 247.608 64.8049 210.525Z");
            path2.setAttribute("fill", "black");
            svg.appendChild(path2);

            const ellipse2 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse2.setAttribute("cx", "119.889");
            ellipse2.setAttribute("cy", "107.363");
            ellipse2.setAttribute("rx", "49.5293");
            ellipse2.setAttribute("ry", "49.4805");
            ellipse2.setAttribute("fill", "#6D4C41");
            svg.appendChild(ellipse2);

            const path3 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path3.setAttribute("d", "M225.891 232.465C203.672 240.867 194.471 251.137 169.418 251.137C138.867 251.137 126.369 260.939 126.369 232.465C126.369 185.029 144.365 149.842 169.418 149.842C194.471 149.842 225.891 185.029 225.891 232.465Z");
            path3.setAttribute("fill", "white");
            svg.appendChild(path3);

            const path4 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path4.setAttribute("d", "M135.19 201.022C134.77 188.318 139.231 160.644 160.444 151.579C181.656 142.515 208.004 147.802 218.526 151.579C228.487 154.154 248.156 167.648 247.146 201.022C245.883 242.74 137.716 238.104 135.19 201.022Z");
            path4.setAttribute("fill", "white");
            svg.appendChild(path4);

            const ellipse3 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse3.setAttribute("cx", "190.711");
            ellipse3.setAttribute("cy", "133.037");
            ellipse3.setAttribute("rx", "32.4023");
            ellipse3.setAttribute("ry", "24.7402");
            ellipse3.setAttribute("fill", "#6D4C41");
            svg.appendChild(ellipse3);

            const ellipse4 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse4.setAttribute("cx", "190.248");
            ellipse4.setAttribute("cy", "112.031");
            ellipse4.setAttribute("rx", "40.2715");
            ellipse4.setAttribute("ry", "40.1445");
            ellipse4.setAttribute("fill", "white");
            svg.appendChild(ellipse4);

            const ellipse5 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse5.setAttribute("cx", "119.889");
            ellipse5.setAttribute("cy", "112.031");
            ellipse5.setAttribute("rx", "40.2715");
            ellipse5.setAttribute("ry", "40.1445");
            ellipse5.setAttribute("fill", "black");
            svg.appendChild(ellipse5);

            const path5 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path5.setAttribute("d", "M154.143 15.9023C218.737 15.9023 271.16 68.5212 271.16 133.504C271.16 198.487 218.737 251.105 154.143 251.105C89.5486 251.105 37.125 198.487 37.125 133.504C37.125 68.5212 89.5486 15.9023 154.143 15.9023Z");
            path5.setAttribute("stroke", "#6D4C41");
            path5.setAttribute("stroke-width", "15");
            svg.appendChild(path5);

            return svg;

        },

        settings: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "256px");
            svg.setAttribute("height", "256px");
            svg.setAttribute("viewBox", "0 0 24 24");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");
            svg.setAttribute("fill", "#000000");

            const path = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path.setAttribute("fill-rule", "evenodd");
            path.setAttribute("clip-rule", "evenodd");
            path.setAttribute("d", `M19.43,12.98c0.04-0.32,0.07-0.64,0.07-0.98s-0.03-0.66-0.07-0.98l2.11-1.65c0.19-0.15,0.24-0.42,0.12-0.64l-2-3.46
                c-0.12-0.22-0.39-0.3-0.61-0.22l-2.49,1c-0.52-0.4-1.08-0.73-1.69-0.98l-0.38-2.65C14.46,2.18,14.25,2,14,2h-4
                c-0.25,0-0.46,0.18-0.49,0.42l-0.38,2.65c-0.61,0.25-1.17,0.59-1.69,0.98l-2.49-1c-0.22-0.08-0.49,0-0.61,0.22l-2,3.46
                c-0.12,0.22-0.07,0.49,0.12,0.64l2.11,1.65c-0.04,0.32-0.07,0.65-0.07,0.98s0.03,0.66,0.07,0.98l-2.11,1.65
                c-0.19,0.15-0.24,0.42-0.12,0.64l2,3.46c0.12,0.22,0.39,0.3,0.61,0.22l2.49-1c0.52,0.4,1.08,0.73,1.69,0.98l0.38,2.65
                c0.03,0.24,0.24,0.42,0.49,0.42h4c0.25,0,0.46-0.18,0.49-0.42l0.38-2.65c0.61-0.25,1.17-0.59,1.69-0.98l2.49,1
                c0.22,0.08,0.49,0,0.61-0.22l2-3.46c0.12-0.22,0.07-0.49-0.12-0.64L19.43,12.98z M12,15.5c-1.93,0-3.5-1.57-3.5-3.5
                s1.57-3.5,3.5-3.5s3.5,1.57,3.5,3.5S13.93,15.5,12,15.5z`);

            svg.appendChild(path);

            return svg;

        },

        exit: function() {

            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "256px");
            svg.setAttribute("height", "256px");
            svg.setAttribute("viewBox", "0 0 512 512");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");
            svg.setAttribute("fill", "#000000");

            const path = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path.setAttribute("d", `M377.9 105.9L500.7 228.7c7.2 7.2 11.3 17.1 11.3 27.3s-4.1 20.1-11.3 27.3L377.9 406.1c-6.4 6.4-15 9.9-24 9.9c-18.7 0-33.9-15.2-33.9-33.9l0-62.1-128 0c-17.7 0-32-14.3-32-32l0-64c0-17.7 14.3-32 32-32l128 0 0-62.1c0-18.7 15.2-33.9 33.9-33.9c9 0 17.6 3.6 24 9.9zM160 96L96 96c-17.7 0-32 14.3-32 32l0 256c0 17.7 14.3 32 32 32l64 0c17.7 0 32 14.3 32 32s-14.3 32-32 32l-64 0c-53 0-96-43-96-96L0 128C0 75 43 32 96 32l64 0c17.7 0 32 14.3 32 32s-14.3 32-32 32z`);

            svg.appendChild(path);

            return svg;

        }

    },

    init: function(){

        initializeObject.changeStatus("Carregando recursos...");

        initializeObject.changeStatus("Carregando palavras essenciais...");

        try {
            
            let tempObj = JSON.parse( ForcaFiles.getJSONContent("../files/words/default/words.json") );

            if( ! tempObj.hasOwnProperty("easy") ) 
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca não contém as palavras da dificuldade fácil.");
            if( ! tempObj.hasOwnProperty("normal") ) 
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca não contém as palavras da dificuldade normal.");
            if( ! tempObj.hasOwnProperty("hard") ) 
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca não contém as palavras da dificuldade difícil.");

            if( typeof tempObj.easy !== "object" || ! Array.isArray(tempObj.easy) || tempObj.easy.length < 1 )
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca está corrompido!")
            if( typeof tempObj.normal !== "object" || ! Array.isArray(tempObj.normal) || tempObj.normal.length < 1 )
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca está corrompido!")
            if( typeof tempObj.hard !== "object" || ! Array.isArray(tempObj.hard) || tempObj.hard.length < 1 )
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca está corrompido!")    
            
            tempObj.easy = utilsObject.filterWordsArray(tempObj.easy);

            if(tempObj.easy.empty())
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca está corrompido!")

            tempObj.normal = utilsObject.filterWordsArray(tempObj.normal);

            if(tempObj.normal.empty())
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca está corrompido!")

            tempObj.hard = utilsObject.filterWordsArray(tempObj.hard);

            if(tempObj.hard.empty())
                throw new Error("O arquivo JSON de palavras essenciais do Jogo da Forca está corrompido!")

            resourcesObject.words.default = tempObj;

        } catch (error) {
            
            initializeObject.changeStatus("Erro ao carregar palavras essenciais, tente reiniciar o Jogo da Forca!");
            
            throw new Error("\n\n\t" + error + "\n");

        }

        initializeObject.changeStatus("Carregando palavras customizadas...");

        try {
            
            let tempObj = JSON.parse( ForcaFiles.getJSONContent("../files/words/custom/words.json") );

            if( ! tempObj.hasOwnProperty("easy") ) 
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca não contém as palavras da dificuldade fácil.");
            if( ! tempObj.hasOwnProperty("normal") ) 
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca não contém as palavras da dificuldade normal.");
            if( ! tempObj.hasOwnProperty("hard") ) 
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca não contém as palavras da dificuldade difícil.");

            if( typeof tempObj.easy !== "object" || ! Array.isArray(tempObj.easy) || tempObj.easy.length < 1 )
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca está corrompido!")
            if( typeof tempObj.normal !== "object" || ! Array.isArray(tempObj.normal) || tempObj.normal.length < 1 )
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca está corrompido!")
            if( typeof tempObj.hard !== "object" || ! Array.isArray(tempObj.hard) || tempObj.hard.length < 1 )
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca está corrompido!")    
            
            tempObj.easy = utilsObject.filterWordsArray(tempObj.easy);

            if(tempObj.easy.empty())
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca está corrompido!")

            tempObj.normal = utilsObject.filterWordsArray(tempObj.normal);

            if(tempObj.normal.empty())
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca está corrompido!")

            tempObj.hard = utilsObject.filterWordsArray(tempObj.hard);

            if(tempObj.hard.empty())
                throw new Error("O arquivo JSON de palavras customizadas do Jogo da Forca está corrompido!")

            resourcesObject.words.default = tempObj;

        } catch (error) {
            
            initializeObject.changeStatus("Erro ao carregar palavras customizadas, tentando recriar o arquivo...");
            
            resourcesObject.words.custom.easy = [];
            resourcesObject.words.custom.normal = [];
            resourcesObject.words.custom.hard = [];

            try {
                
                const jsonString = JSON.stringify(resourcesObject.words.custom);

                const createFile = ForcaFiles.createFile("../files/words/custom/words.json", jsonString);

                if( ! createFile )
                    throw new Error("Erro ao criar o arquivo JSON de palavras customizadas!");

            } catch (error) {
                
                console.error("Erro ao criar o arquivo JSON de palavras customizadas, operando no modo temporário.");

                initializeObject.changeStatus("Erro ao criar o arquivo JSON de palavras customizadas, operando no modo temporário.");

            }

        }        

        Object.freeze(resourcesObject);

        initializeObject.changeStatus("Criando a interface gráfica...");

        initializeObject.changeSubStatus("Criando o elemento body...");

        interfaceObject.body = document.createElement("body");

        initializeObject.changeSubStatus("Carregando as fontes...");

        interfaceObject.styleFonts = document.createElement("style")

        const styleFontsString = ForcaFiles.getStringContent("../files/web/css/fonts.css");

        interfaceObject.styleFonts.textContent = styleFontsString;

        interfaceObject.body.append(interfaceObject.styleFonts);

        initializeObject.changeSubStatus("Carregando a folha de estilos...");

        interfaceObject.styleSheet = document.createElement("style");

        const styleString = ForcaFiles.getStringContent("../files/web/css/game.css");

        interfaceObject.styleSheet.textContent = styleString;

        interfaceObject.body.append(interfaceObject.styleSheet);

        initializeObject.changeSubStatus("Carregando SVGs...");

        interfaceObject.svg = {};

        interfaceObject.svg.caveira = initializeObject.svg.caveira();

        interfaceObject.svg.forca = initializeObject.svg.forca();

        interfaceObject.svg.easy = initializeObject.svg.easy();

        interfaceObject.svg.normal = initializeObject.svg.normal();

        interfaceObject.svg.hard = initializeObject.svg.hard();

        interfaceObject.svg.solo = initializeObject.svg.solo();

        interfaceObject.svg.duo = initializeObject.svg.duo();

        interfaceObject.svg.settings = initializeObject.svg.settings();

        interfaceObject.svg.exit = initializeObject.svg.exit();

        initializeObject.changeSubStatus("Criando o menu inicial...");

        // Criando o menu principal do jogo

        interfaceObject.menu = {};

        interfaceObject.menu.start = {};

        interfaceObject.menu.start.main = document.createElement("main");

        interfaceObject.menu.start.main.classList.add("menu-container");

        interfaceObject.menu.start.main.classList.add("startMenu");

        // Criando o container do título do jogo

        interfaceObject.menu.start.tituloContainer = document.createElement("div")

        interfaceObject.menu.start.tituloContainer.classList.add("titulo-container");

        interfaceObject.menu.start.svgCaveira = interfaceObject.svg.caveira.cloneNode(true);

        interfaceObject.menu.start.svgCaveira.classList.add("caveira-svg");

        interfaceObject.menu.start.svgCaveira.style.width = "47px";

        interfaceObject.menu.start.svgCaveira.style.height = "70px";

        interfaceObject.menu.start.tituloContainer.append(interfaceObject.menu.start.svgCaveira);

        interfaceObject.menu.start.titulo = document.createElement("h1");

        interfaceObject.menu.start.titulo.classList.add("titulo-jogo");

        interfaceObject.menu.start.titulo.textContent = "Jogo da Forca";

        interfaceObject.menu.start.tituloContainer.append(interfaceObject.menu.start.titulo);

        interfaceObject.menu.start.svgForca = interfaceObject.svg.forca.cloneNode(true);

        interfaceObject.menu.start.svgForca.classList.add("forca-svg");

        interfaceObject.menu.start.svgForca.style.width = "29px";

        interfaceObject.menu.start.svgForca.style.height = "70px";

        interfaceObject.menu.start.tituloContainer.append(interfaceObject.menu.start.svgForca);

        interfaceObject.menu.start.main.append(interfaceObject.menu.start.tituloContainer);

        // Criando as opções do menu principal

        interfaceObject.menu.start.optionsContainer = document.createElement("nav");

        interfaceObject.menu.start.optionsContainer.classList.add("menu-navegacao");

        // Criando o botão solo

        interfaceObject.menu.start.buttons = {};

        interfaceObject.menu.start.buttons.solo = {};

        interfaceObject.menu.start.buttons.solo.button = document.createElement("button");

        interfaceObject.menu.start.buttons.solo.button.classList.add("menu-botao");

        interfaceObject.menu.start.buttons.solo.button.setAttribute("id", "solo");

        interfaceObject.menu.start.buttons.solo.title = document.createElement("p");

        interfaceObject.menu.start.buttons.solo.title.classList.add("menu-botao-titulo");

        interfaceObject.menu.start.buttons.solo.title.textContent = "Solo";

        interfaceObject.menu.start.buttons.solo.button.append(interfaceObject.menu.start.buttons.solo.title);

        interfaceObject.menu.start.buttons.solo.svg = interfaceObject.svg.solo.cloneNode(true);

        interfaceObject.menu.start.buttons.solo.svg.classList.add("menu-botao-svg");

        interfaceObject.menu.start.buttons.solo.svg.classList.add("solo-svg");

        interfaceObject.menu.start.buttons.solo.svg.style.width = "120px";

        interfaceObject.menu.start.buttons.solo.svg.style.height = "120px";

        interfaceObject.menu.start.buttons.solo.button.append(interfaceObject.menu.start.buttons.solo.svg);

        interfaceObject.menu.start.buttons.solo.afterText = document.createElement("p");

        interfaceObject.menu.start.buttons.solo.afterText.classList.add("menu-botao-after");

        interfaceObject.menu.start.buttons.solo.afterText.textContent = "Enfrente o desafio e mostre que é capaz de vencer a forca! Escolha um nível de dificuldade, adivinhe as palavras e teste seus conhecimentos sem depender de ninguém.";

        interfaceObject.menu.start.buttons.solo.button.append(interfaceObject.menu.start.buttons.solo.afterText);

        interfaceObject.menu.start.optionsContainer.append(interfaceObject.menu.start.buttons.solo.button);

        // Criando o botão duo

        interfaceObject.menu.start.buttons.duo = {};

        interfaceObject.menu.start.buttons.duo.button = document.createElement("button");

        interfaceObject.menu.start.buttons.duo.button.classList.add("menu-botao");

        interfaceObject.menu.start.buttons.duo.button.setAttribute("id", "duo");

        interfaceObject.menu.start.buttons.duo.title = document.createElement("p");

        interfaceObject.menu.start.buttons.duo.title.classList.add("menu-botao-titulo");

        interfaceObject.menu.start.buttons.duo.title.textContent = "Dupla";

        interfaceObject.menu.start.buttons.duo.button.append(interfaceObject.menu.start.buttons.duo.title);

        interfaceObject.menu.start.buttons.duo.svg = interfaceObject.svg.duo.cloneNode(true);

        interfaceObject.menu.start.buttons.duo.svg.classList.add("menu-botao-svg");

        interfaceObject.menu.start.buttons.duo.svg.classList.add("duo-svg");

        interfaceObject.menu.start.buttons.duo.svg.style.width = "120px";

        interfaceObject.menu.start.buttons.duo.svg.style.height = "120px";

        interfaceObject.menu.start.buttons.duo.button.append(interfaceObject.menu.start.buttons.duo.svg);

        interfaceObject.menu.start.buttons.duo.afterText = document.createElement("p");

        interfaceObject.menu.start.buttons.duo.afterText.classList.add("menu-botao-after");

        interfaceObject.menu.start.buttons.duo.afterText.textContent = "Um jogador escolhe a palavra enquanto o outro tenta adivinhar. Vocês podem se revezar ou criar seus próprios desafios. Vejam quem consegue escapar da forca!";

        interfaceObject.menu.start.buttons.duo.button.append(interfaceObject.menu.start.buttons.duo.afterText);

        interfaceObject.menu.start.optionsContainer.append(interfaceObject.menu.start.buttons.duo.button);

        // Criando o botão opções

        interfaceObject.menu.start.buttons.options = {};

        interfaceObject.menu.start.buttons.options.button = document.createElement("button");

        interfaceObject.menu.start.buttons.options.button.classList.add("menu-botao");

        interfaceObject.menu.start.buttons.options.button.setAttribute("id", "options");

        interfaceObject.menu.start.buttons.options.title = document.createElement("p");

        interfaceObject.menu.start.buttons.options.title.classList.add("menu-botao-titulo");

        interfaceObject.menu.start.buttons.options.title.textContent = "Opções";

        interfaceObject.menu.start.buttons.options.button.append(interfaceObject.menu.start.buttons.options.title);

        interfaceObject.menu.start.buttons.options.svg = interfaceObject.svg.settings.cloneNode(true);

        interfaceObject.menu.start.buttons.options.svg.classList.add("menu-botao-svg");

        interfaceObject.menu.start.buttons.options.svg.classList.add("options-svg");

        interfaceObject.menu.start.buttons.options.svg.style.width = "120px";

        interfaceObject.menu.start.buttons.options.svg.style.height = "120px";

        interfaceObject.menu.start.buttons.options.button.append(interfaceObject.menu.start.buttons.options.svg);

        interfaceObject.menu.start.buttons.options.afterText = document.createElement("p");

        interfaceObject.menu.start.buttons.options.afterText.classList.add("menu-botao-after");

        interfaceObject.menu.start.buttons.options.afterText.textContent = "Personalize sua experiência! Ajuste preferências de som, tema, idioma e outras configurações do jogo para jogar do seu jeito.";

        interfaceObject.menu.start.buttons.options.button.append(interfaceObject.menu.start.buttons.options.afterText);

        interfaceObject.menu.start.optionsContainer.append(interfaceObject.menu.start.buttons.options.button);

        // Criando o botão sair

        interfaceObject.menu.start.buttons.exit = {};

        interfaceObject.menu.start.buttons.exit.button = document.createElement("button");

        interfaceObject.menu.start.buttons.exit.button.classList.add("menu-botao");

        interfaceObject.menu.start.buttons.exit.button.setAttribute("id", "exit");

        interfaceObject.menu.start.buttons.exit.title = document.createElement("p");

        interfaceObject.menu.start.buttons.exit.title.classList.add("menu-botao-titulo");

        interfaceObject.menu.start.buttons.exit.title.textContent = "Sair";

        interfaceObject.menu.start.buttons.exit.button.append(interfaceObject.menu.start.buttons.exit.title);

        interfaceObject.menu.start.buttons.exit.svg = interfaceObject.svg.exit.cloneNode(true);

        interfaceObject.menu.start.buttons.exit.svg.classList.add("menu-botao-svg");

        interfaceObject.menu.start.buttons.exit.svg.classList.add("exit-svg");

        interfaceObject.menu.start.buttons.exit.svg.style.width = "120px";

        interfaceObject.menu.start.buttons.exit.svg.style.height = "120px";

        interfaceObject.menu.start.buttons.exit.button.append(interfaceObject.menu.start.buttons.exit.svg);

        interfaceObject.menu.start.buttons.exit.afterText = document.createElement("p");

        interfaceObject.menu.start.buttons.exit.afterText.classList.add("menu-botao-after");

        interfaceObject.menu.start.buttons.exit.afterText.textContent = "Encerre sua sessão e volte para a tela inicial. Tem certeza de que deseja sair do Jogo da Forca?";

        interfaceObject.menu.start.buttons.exit.button.append(interfaceObject.menu.start.buttons.exit.afterText);

        interfaceObject.menu.start.optionsContainer.append(interfaceObject.menu.start.buttons.exit.button);

        interfaceObject.menu.start.main.append(interfaceObject.menu.start.optionsContainer);

        // Adicionando as nodes list vivas dos botões, para consulta rápida e cache inteligente

        interfaceObject.menu.start.buttons.all = interfaceObject.menu.start.main.getElementsByClassName("menu-botao");

        interfaceObject.menu.start.buttons.mouseenter = interfaceObject.menu.start.main.getElementsByClassName("menu-botao-hover");

        initializeObject.changeSubStatus("Criando o menu de jogo solo...");

        // Criando o menu de jogo solo

        interfaceObject.menu.solo = {};

        interfaceObject.menu.solo.main = document.createElement("main");

        interfaceObject.menu.solo.main.classList.add("menu-nivel");

        interfaceObject.menu.solo.main.classList.add("soloMenu");

        // Criando o container do título do jogo

        interfaceObject.menu.solo.tituloContainer = document.createElement("div")

        interfaceObject.menu.solo.tituloContainer.classList.add("titulo-container");

        interfaceObject.menu.solo.svgCaveira = interfaceObject.svg.caveira.cloneNode(true);

        interfaceObject.menu.solo.svgCaveira.classList.add("caveira-svg");

        interfaceObject.menu.solo.svgCaveira.style.width = "47px";

        interfaceObject.menu.solo.svgCaveira.style.height = "70px";

        interfaceObject.menu.solo.tituloContainer.append(interfaceObject.menu.solo.svgCaveira);

        interfaceObject.menu.solo.titulo = document.createElement("h1");

        interfaceObject.menu.solo.titulo.classList.add("titulo-jogo");

        interfaceObject.menu.solo.titulo.textContent = "Jogo da Forca";

        interfaceObject.menu.solo.tituloContainer.append(interfaceObject.menu.solo.titulo);

        interfaceObject.menu.solo.svgForca = interfaceObject.svg.forca.cloneNode(true);

        interfaceObject.menu.solo.svgForca.classList.add("forca-svg");

        interfaceObject.menu.solo.svgForca.style.width = "29px";

        interfaceObject.menu.solo.svgForca.style.height = "70px";

        interfaceObject.menu.solo.tituloContainer.append(interfaceObject.menu.solo.svgForca);

        interfaceObject.menu.solo.main.append(interfaceObject.menu.solo.tituloContainer);

        // Criando o subtitulo do menu de jogo solo

        interfaceObject.menu.solo.subtitulo = document.createElement("h3");

        interfaceObject.menu.solo.subtitulo.classList.add("titulo-nivel");

        interfaceObject.menu.solo.subtitulo.textContent = "ESCOLHA UM NÍVEL";

        interfaceObject.menu.solo.main.append(interfaceObject.menu.solo.subtitulo);

        // Criando o container dos botões de nível

        interfaceObject.menu.solo.niveisContainer = document.createElement("nav");

        interfaceObject.menu.solo.niveisContainer.classList.add("menu-navegacao");

        // Criando o subcontainer dos botões de nível

        interfaceObject.menu.solo.niveisSubContainer = document.createElement("div");

        interfaceObject.menu.solo.niveisSubContainer.classList.add("container-botoes");

        // Criando o botão fácil

        interfaceObject.menu.solo.buttons = {};

        interfaceObject.menu.solo.buttons.easy = {};

        interfaceObject.menu.solo.buttons.easy.button = document.createElement("button");

        interfaceObject.menu.solo.buttons.easy.button.classList.add("menu-botao-nivel");

        interfaceObject.menu.solo.buttons.easy.button.setAttribute("id", "easy");

        interfaceObject.menu.solo.buttons.easy.title = document.createElement("p");

        interfaceObject.menu.solo.buttons.easy.title.classList.add("texto-fixo");

        interfaceObject.menu.solo.buttons.easy.title.textContent = "FÁCIL";

        interfaceObject.menu.solo.buttons.easy.button.append(interfaceObject.menu.solo.buttons.easy.title);

        interfaceObject.menu.solo.buttons.easy.titleAfter = document.createElement("p");

        interfaceObject.menu.solo.buttons.easy.titleAfter.classList.add("texto-dificuldade");

        interfaceObject.menu.solo.buttons.easy.titleAfter.textContent = "Palavras curtas e simples, ideais para iniciantes ou partidas leves.";

        interfaceObject.menu.solo.buttons.easy.button.append(interfaceObject.menu.solo.buttons.easy.titleAfter);

        interfaceObject.menu.solo.buttons.easy.svg = interfaceObject.svg.easy.cloneNode(true);

        interfaceObject.menu.solo.buttons.easy.svg.classList.add("nivel-botao-svg");

        interfaceObject.menu.solo.buttons.easy.svg.classList.add("easy-svg");

        interfaceObject.menu.solo.buttons.easy.svg.style.width = "60px";

        interfaceObject.menu.solo.buttons.easy.svg.style.height = "60px";

        interfaceObject.menu.solo.buttons.easy.button.append(interfaceObject.menu.solo.buttons.easy.svg);

        interfaceObject.menu.solo.niveisSubContainer.append(interfaceObject.menu.solo.buttons.easy.button);

        // Criando o botão normal

        interfaceObject.menu.solo.buttons.normal = {};

        interfaceObject.menu.solo.buttons.normal.button = document.createElement("button");

        interfaceObject.menu.solo.buttons.normal.button.classList.add("menu-botao-nivel");

        interfaceObject.menu.solo.buttons.normal.button.setAttribute("id", "normal");

        interfaceObject.menu.solo.buttons.normal.title = document.createElement("p");

        interfaceObject.menu.solo.buttons.normal.title.classList.add("texto-fixo");

        interfaceObject.menu.solo.buttons.normal.title.textContent = "NORMAL";

        interfaceObject.menu.solo.buttons.normal.button.append(interfaceObject.menu.solo.buttons.normal.title);

        interfaceObject.menu.solo.buttons.normal.titleAfter = document.createElement("p");

        interfaceObject.menu.solo.buttons.normal.titleAfter.classList.add("texto-dificuldade");

        interfaceObject.menu.solo.buttons.normal.titleAfter.textContent = "Palavras médias que pedem mais atenção e estratégia.";

        interfaceObject.menu.solo.buttons.normal.button.append(interfaceObject.menu.solo.buttons.normal.titleAfter);

        interfaceObject.menu.solo.buttons.normal.svg = interfaceObject.svg.normal.cloneNode(true);

        interfaceObject.menu.solo.buttons.normal.svg.classList.add("nivel-botao-svg");

        interfaceObject.menu.solo.buttons.normal.svg.classList.add("normal-svg");

        interfaceObject.menu.solo.buttons.normal.svg.style.width = "60px";

        interfaceObject.menu.solo.buttons.normal.svg.style.height = "60px";

        interfaceObject.menu.solo.buttons.normal.button.append(interfaceObject.menu.solo.buttons.normal.svg);

        interfaceObject.menu.solo.niveisSubContainer.append(interfaceObject.menu.solo.buttons.normal.button);

        // Criando o botão difícil

        interfaceObject.menu.solo.buttons.hard = {};

        interfaceObject.menu.solo.buttons.hard.button = document.createElement("button");

        interfaceObject.menu.solo.buttons.hard.button.classList.add("menu-botao-nivel");

        interfaceObject.menu.solo.buttons.hard.button.setAttribute("id", "hard");

        interfaceObject.menu.solo.buttons.hard.title = document.createElement("p");

        interfaceObject.menu.solo.buttons.hard.title.classList.add("texto-fixo");

        interfaceObject.menu.solo.buttons.hard.title.textContent = "DIFÍCIL";

        interfaceObject.menu.solo.buttons.hard.button.append(interfaceObject.menu.solo.buttons.hard.title);

        interfaceObject.menu.solo.buttons.hard.titleAfter = document.createElement("p");

        interfaceObject.menu.solo.buttons.hard.titleAfter.classList.add("texto-dificuldade");

        interfaceObject.menu.solo.buttons.hard.titleAfter.textContent = "Desafio intenso com palavras longas e pouco comuns.";

        interfaceObject.menu.solo.buttons.hard.button.append(interfaceObject.menu.solo.buttons.hard.titleAfter);

        interfaceObject.menu.solo.buttons.hard.svg = interfaceObject.svg.hard.cloneNode(true);

        interfaceObject.menu.solo.buttons.hard.svg.classList.add("nivel-botao-svg");

        interfaceObject.menu.solo.buttons.hard.svg.classList.add("hard-svg");

        interfaceObject.menu.solo.buttons.hard.svg.style.width = "60px";

        interfaceObject.menu.solo.buttons.hard.svg.style.height = "60px";

        interfaceObject.menu.solo.buttons.hard.button.append(interfaceObject.menu.solo.buttons.hard.svg);

        interfaceObject.menu.solo.niveisSubContainer.append(interfaceObject.menu.solo.buttons.hard.button);

        interfaceObject.menu.solo.niveisContainer.append(interfaceObject.menu.solo.niveisSubContainer);

        // Criando o botão voltar

        interfaceObject.menu.solo.buttons.back = {};

        interfaceObject.menu.solo.buttons.back.button = document.createElement("button");

        interfaceObject.menu.solo.buttons.back.button.classList.add("menu-botao");

        interfaceObject.menu.solo.buttons.back.button.setAttribute("id", "back");

        interfaceObject.menu.solo.buttons.back.title = document.createElement("p");

        interfaceObject.menu.solo.buttons.back.title.classList.add("texto-fixo");

        interfaceObject.menu.solo.buttons.back.title.textContent = "VOLTAR";

        interfaceObject.menu.solo.buttons.back.button.append(interfaceObject.menu.solo.buttons.back.title);

        interfaceObject.menu.solo.niveisContainer.append(interfaceObject.menu.solo.buttons.back.button);

        interfaceObject.menu.solo.main.append(interfaceObject.menu.solo.niveisContainer);

        // Adicionando as nodes list vivas dos botões, para consulta rápida e cache inteligente

        interfaceObject.menu.solo.buttons.all = interfaceObject.menu.solo.main.getElementsByClassName("menu-botao-nivel");

        initializeObject.changeSubStatus("Criando o menu de jogo dupla...");

        // Criando o menu de jogo duo

        interfaceObject.menu.duo = {};

        interfaceObject.menu.duo.main = document.createElement("main");

        interfaceObject.menu.duo.main.classList.add("menu-nivel");

        interfaceObject.menu.duo.main.classList.add("duoMenu");

        // Criando o container do título do jogo

        interfaceObject.menu.duo.tituloContainer = document.createElement("div")

        interfaceObject.menu.duo.tituloContainer.classList.add("titulo-container");

        interfaceObject.menu.duo.svgCaveira = interfaceObject.svg.caveira.cloneNode(true);

        interfaceObject.menu.duo.svgCaveira.classList.add("caveira-svg");

        interfaceObject.menu.duo.svgCaveira.style.width = "47px";

        interfaceObject.menu.duo.svgCaveira.style.height = "70px";

        interfaceObject.menu.duo.tituloContainer.append(interfaceObject.menu.duo.svgCaveira);

        interfaceObject.menu.duo.titulo = document.createElement("h1");

        interfaceObject.menu.duo.titulo.classList.add("titulo-jogo");

        interfaceObject.menu.duo.titulo.textContent = "Jogo da Forca";

        interfaceObject.menu.duo.tituloContainer.append(interfaceObject.menu.duo.titulo);

        interfaceObject.menu.duo.svgForca = interfaceObject.svg.forca.cloneNode(true);

        interfaceObject.menu.duo.svgForca.classList.add("forca-svg");

        interfaceObject.menu.duo.svgForca.style.width = "29px";

        interfaceObject.menu.duo.svgForca.style.height = "70px";

        interfaceObject.menu.duo.tituloContainer.append(interfaceObject.menu.duo.svgForca);

        interfaceObject.menu.duo.main.append(interfaceObject.menu.duo.tituloContainer);
        
        // Criando o subtitulo do menu de jogo duo

        interfaceObject.menu.duo.subtitulo = document.createElement("h3");

        interfaceObject.menu.duo.subtitulo.classList.add("titulo-nivel");

        interfaceObject.menu.duo.subtitulo.textContent = "Jogador 1: Escolha cuidadosamente a palavra que o Jogador 2 terá que adivinhar. A dificuldade será definida pelo número de letras e complexidade da palavra. Digite a palavra abaixo. Ela ficará oculta para manter o mistério!";

        interfaceObject.menu.duo.main.append(interfaceObject.menu.duo.subtitulo);

        // Criando o input de palavra

        interfaceObject.menu.duo.input = document.createElement("input");

        interfaceObject.menu.duo.input.setAttribute("type", "password");

        interfaceObject.menu.duo.input.setAttribute("placeholder", "DIGITE A PALVRA SECRETA");

        interfaceObject.menu.duo.input.classList.add("input-palavra-secreta");

        interfaceObject.menu.duo.input.setAttribute("maxlength", "15");

        interfaceObject.menu.duo.input.setAttribute("autocomplete", "off");

        interfaceObject.menu.duo.input.setAttribute("id", "palavra-secreta");

        interfaceObject.menu.duo.main.append(interfaceObject.menu.duo.input);

        // Criando o container dos botões

        interfaceObject.menu.duo.buttonsContainer = document.createElement("nav");

        interfaceObject.menu.duo.buttonsContainer.classList.add("menu-navegacao");

        // Criando o botão confirmar

        interfaceObject.menu.duo.buttons = {};

        interfaceObject.menu.duo.buttons.confirm = {};

        interfaceObject.menu.duo.buttons.confirm.button = document.createElement("button");

        interfaceObject.menu.duo.buttons.confirm.button.classList.add("menu-botao");

        interfaceObject.menu.duo.buttons.confirm.button.setAttribute("id", "confirmar-palavra");

        interfaceObject.menu.duo.buttons.confirm.title = document.createElement("p");

        interfaceObject.menu.duo.buttons.confirm.title.classList.add("texto-fixo");

        interfaceObject.menu.duo.buttons.confirm.title.textContent = "CONFIRMAR";

        interfaceObject.menu.duo.buttons.confirm.button.append(interfaceObject.menu.duo.buttons.confirm.title);

        interfaceObject.menu.duo.buttonsContainer.append(interfaceObject.menu.duo.buttons.confirm.button);

        // Criando o botão voltar

        interfaceObject.menu.duo.buttons.back = {};

        interfaceObject.menu.duo.buttons.back.button = document.createElement("button");

        interfaceObject.menu.duo.buttons.back.button.classList.add("menu-botao");

        interfaceObject.menu.duo.buttons.back.button.setAttribute("id", "back");

        interfaceObject.menu.duo.buttons.back.title = document.createElement("p");

        interfaceObject.menu.duo.buttons.back.title.classList.add("texto-fixo");

        interfaceObject.menu.duo.buttons.back.title.textContent = "VOLTAR";

        interfaceObject.menu.duo.buttons.back.button.append(interfaceObject.menu.duo.buttons.back.title);

        interfaceObject.menu.duo.buttonsContainer.append(interfaceObject.menu.duo.buttons.back.button);

        interfaceObject.menu.duo.main.append(interfaceObject.menu.duo.buttonsContainer);

        // Adicionando as nodes list vivas dos botões, para consulta rápida e cache inteligente

        interfaceObject.menu.duo.buttons.all = interfaceObject.menu.duo.main.getElementsByClassName("menu-botao");

        initializeObject.changeSubStatus("Criando a interface do jogo da forca...");

        // Criando a interface do jogo da forca

        interfaceObject.menu.forca = {};

        interfaceObject.menu.forca.main = document.createElement("main");

        interfaceObject.menu.forca.main.classList.add("menu-forca");

        interfaceObject.menu.forca.main.classList.add("forcaMenu");

        // Criando o container do título do jogo

        interfaceObject.menu.forca.tituloContainer = document.createElement("div")

        interfaceObject.menu.forca.tituloContainer.classList.add("titulo-container");

        interfaceObject.menu.forca.svgCaveira = interfaceObject.svg.caveira.cloneNode(true);

        interfaceObject.menu.forca.svgCaveira.classList.add("caveira-svg");

        interfaceObject.menu.forca.svgCaveira.style.width = "47px";

        interfaceObject.menu.forca.svgCaveira.style.height = "70px";

        interfaceObject.menu.forca.tituloContainer.append(interfaceObject.menu.forca.svgCaveira);

        interfaceObject.menu.forca.titulo = document.createElement("h1");

        interfaceObject.menu.forca.titulo.classList.add("titulo-jogo");

        interfaceObject.menu.forca.titulo.textContent = "Jogo da Forca";

        interfaceObject.menu.forca.tituloContainer.append(interfaceObject.menu.forca.titulo);

        interfaceObject.menu.forca.svgForca = interfaceObject.svg.forca.cloneNode(true);

        interfaceObject.menu.forca.svgForca.classList.add("forca-svg");

        interfaceObject.menu.forca.svgForca.style.width = "29px";

        interfaceObject.menu.forca.svgForca.style.height = "70px";

        interfaceObject.menu.forca.tituloContainer.append(interfaceObject.menu.forca.svgForca);

        interfaceObject.menu.forca.main.append(interfaceObject.menu.forca.tituloContainer); 
        
        // Criando o desenho da forca

        interfaceObject.menu.forca.desenhoForca = document.createElement("div");

        interfaceObject.menu.forca.desenhoForca.classList.add("forca");

        interfaceObject.menu.forca.main.append(interfaceObject.menu.forca.desenhoForca);

        // Adicionando as imagens svg

        interfaceObject.menu.forca.cabeca = document.createElement("img");

        interfaceObject.menu.forca.cabeca.classList.add("parte");

        interfaceObject.menu.forca.cabeca.setAttribute("id", "cabeca");

        interfaceObject.menu.forca.cabeca.setAttribute("alt", "Cabeça do boneco da forca");

        interfaceObject.menu.forca.cabeca.setAttribute("src", "../assets/svg/forca/cabeca.svg");

        interfaceObject.menu.forca.desenhoForca.append(interfaceObject.menu.forca.cabeca);

        interfaceObject.menu.forca.corpo = document.createElement("img");

        interfaceObject.menu.forca.corpo.classList.add("parte");

        interfaceObject.menu.forca.corpo.setAttribute("id", "corpo");

        interfaceObject.menu.forca.corpo.setAttribute("alt", "Corpo do boneco da forca");

        interfaceObject.menu.forca.corpo.setAttribute("src", "../assets/svg/forca/corpo.svg");

        interfaceObject.menu.forca.desenhoForca.append(interfaceObject.menu.forca.corpo);

        interfaceObject.menu.forca.bracoEsquerdo = document.createElement("img");

        interfaceObject.menu.forca.bracoEsquerdo.classList.add("parte");

        interfaceObject.menu.forca.bracoEsquerdo.setAttribute("id", "braco-esquerdo");

        interfaceObject.menu.forca.bracoEsquerdo.setAttribute("alt", "Braço esquerdo do boneco da forca");

        interfaceObject.menu.forca.bracoEsquerdo.setAttribute("src", "../assets/svg/forca/braco-esquerdo.svg");

        interfaceObject.menu.forca.desenhoForca.append(interfaceObject.menu.forca.bracoEsquerdo);

        interfaceObject.menu.forca.bracoDireito = document.createElement("img");

        interfaceObject.menu.forca.bracoDireito.classList.add("parte");

        interfaceObject.menu.forca.bracoDireito.setAttribute("id", "braco-direito");

        interfaceObject.menu.forca.bracoDireito.setAttribute("alt", "Braço direito do boneco da forca");

        interfaceObject.menu.forca.bracoDireito.setAttribute("src", "../assets/svg/forca/braco-direito.svg");

        interfaceObject.menu.forca.desenhoForca.append(interfaceObject.menu.forca.bracoDireito);

        interfaceObject.menu.forca.pernaEsquerda = document.createElement("img");

        interfaceObject.menu.forca.pernaEsquerda.classList.add("parte");

        interfaceObject.menu.forca.pernaEsquerda.setAttribute("id", "perna-esquerda");

        interfaceObject.menu.forca.pernaEsquerda.setAttribute("alt", "Perna esquerda do boneco da forca");

        interfaceObject.menu.forca.pernaEsquerda.setAttribute("src", "../assets/svg/forca/perna-esquerda.svg");

        interfaceObject.menu.forca.desenhoForca.append(interfaceObject.menu.forca.pernaEsquerda);

        interfaceObject.menu.forca.pernaDireita = document.createElement("img");

        interfaceObject.menu.forca.pernaDireita.classList.add("parte");

        interfaceObject.menu.forca.pernaDireita.setAttribute("id", "perna-direita");

        interfaceObject.menu.forca.pernaDireita.setAttribute("alt", "Perna direita do boneco da forca");

        interfaceObject.menu.forca.pernaDireita.setAttribute("src", "../assets/svg/forca/perna-direita.svg");

        interfaceObject.menu.forca.desenhoForca.append(interfaceObject.menu.forca.pernaDireita);

        // Criando o container das letras

        interfaceObject.menu.forca.letrasContainer = document.createElement("div");

        interfaceObject.menu.forca.letrasContainer.classList.add("letras-tentadas");

        interfaceObject.menu.forca.letrasContainer.textContent = "Letras tentadas: ";

        interfaceObject.menu.forca.main.append(interfaceObject.menu.forca.letrasContainer);

        // Criando o span das letras erradas

        interfaceObject.menu.forca.letrasErradas = document.createElement("span");

        interfaceObject.menu.forca.letrasErradas.setAttribute("id", "letras-erradas");

        interfaceObject.menu.forca.letrasContainer.append(interfaceObject.menu.forca.letrasErradas);

        // Criando o input de letra

        interfaceObject.menu.forca.input = document.createElement("input");

        interfaceObject.menu.forca.input.setAttribute("type", "text");

        interfaceObject.menu.forca.input.setAttribute("placeholder", "LETRA");

        interfaceObject.menu.forca.input.classList.add("input-letra");

        interfaceObject.menu.forca.input.setAttribute("maxlength", "1");

        interfaceObject.menu.forca.input.setAttribute("autocomplete", "off");

        interfaceObject.menu.forca.input.setAttribute("id", "entrada-letra");

        interfaceObject.menu.forca.main.append(interfaceObject.menu.forca.input);

        // Criando a div tracos

        interfaceObject.menu.forca.tracosContainer = document.createElement("div");

        interfaceObject.menu.forca.tracosContainer.classList.add("tracos");

        interfaceObject.menu.forca.tracosContainer.setAttribute("id", "tracos");

        interfaceObject.menu.forca.main.append(interfaceObject.menu.forca.tracosContainer);

        // Criando o botão de voltar

        interfaceObject.menu.forca.buttons = {};

        interfaceObject.menu.forca.buttons.back = {};

        interfaceObject.menu.forca.buttons.back.button = document.createElement("button");

        interfaceObject.menu.forca.buttons.back.button.classList.add("menu-botao");

        interfaceObject.menu.forca.buttons.back.button.setAttribute("id", "back");

        interfaceObject.menu.forca.buttons.back.title = document.createElement("p");

        interfaceObject.menu.forca.buttons.back.title.classList.add("texto-fixo");

        interfaceObject.menu.forca.buttons.back.title.textContent = "VOLTAR";

        interfaceObject.menu.forca.buttons.back.button.append(interfaceObject.menu.forca.buttons.back.title);

        interfaceObject.menu.forca.main.append(interfaceObject.menu.forca.buttons.back.button);

        // Criando a div de mensagem final

        interfaceObject.menu.forca.mensagemFinal = document.createElement("div");

        interfaceObject.menu.forca.mensagemFinal.classList.add("mensagem-final");

        interfaceObject.menu.forca.mensagemFinal.setAttribute("id", "mensagem-final");

        interfaceObject.menu.forca.main.append(interfaceObject.menu.forca.mensagemFinal);

        // Cache inteligente das partes do corpo do boneco da forca

        interfaceObject.menu.forca.partes = document.getElementsByClassName("parte");

        // Criando os handlers do menu principal

        interfaceObject.handlers.menu = {};

        interfaceObject.handlers.menu.start = {};

        interfaceObject.handlers.menu.start.buttons = {};

        interfaceObject.handlers.menu.start.buttons.mouseenter = function(event) {

            const target = event.target;

            if( target.classList.contains("menu-botao") ) {

                for( let i = 0, len = interfaceObject.menu.start.buttons.mouseenter.length; i < len; i++ ) {
                    
                    if( interfaceObject.menu.start.buttons.mouseenter[i] === target ) continue;

                    interfaceObject.menu.start.buttons.mouseenter[i].classList.remove("menu-botao-hover");

                }

                if( target.classList.contains("menu-botao-hover") ) return;

                const targetComputedStyle = window.getComputedStyle(target);

                const targetPaddingTop = parseFloat(targetComputedStyle.paddingTop);
                const targetPaddingBottom = parseFloat(targetComputedStyle.paddingBottom);

                const titleElement = target.querySelector(".menu-botao-titulo");

                const titleComputedStyle = window.getComputedStyle(titleElement);

                const titleMarginTop = parseFloat(titleComputedStyle.marginTop);
                const titleMarginBottom = parseFloat(titleComputedStyle.marginBottom);

                const titleHeight = titleElement.getBoundingClientRect().height;

                const afterElement = target.querySelector(".menu-botao-after");

                const afterComputedStyle = window.getComputedStyle(afterElement);

                const afterMarginTop = parseFloat(afterComputedStyle.marginTop);
                const afterMarginBottom = parseFloat(afterComputedStyle.marginBottom);

                const afterHeight = afterElement.getBoundingClientRect().height;

                const totalHeight = targetPaddingTop + targetPaddingBottom + titleMarginTop + titleMarginBottom + titleHeight + afterMarginTop + afterMarginBottom + afterHeight;

                const targetSvg = target.querySelector(".menu-botao-svg");

                const svgComputedStyle = window.getComputedStyle(targetSvg);

                const svgMarginTop = parseFloat(svgComputedStyle.marginTop);
                const svgMarginBottom = parseFloat(svgComputedStyle.marginBottom);

                const svgHeight = targetSvg.getBoundingClientRect().height;

                const minHeight = svgMarginTop + svgMarginBottom + svgHeight + targetPaddingTop + targetPaddingBottom;

                let maxHeight = 0;

                if( totalHeight < minHeight ) maxHeight = minHeight;
                else maxHeight = totalHeight;

                target.classList.add("menu-botao-hover");
                target.style.maxHeight = maxHeight + "px";

            }

        };

        interfaceObject.handlers.menu.start.buttons.mouseleave = function(event) {

            for( let i = 0, len = interfaceObject.menu.start.buttons.mouseenter.length; i < len; i++ ) {
                
                interfaceObject.menu.start.buttons.mouseenter[i].style.maxHeight = '';
                
                interfaceObject.menu.start.buttons.mouseenter[i].classList.remove("menu-botao-hover");

            }

        };

        interfaceObject.handlers.menu.start.buttons.click = {};

        interfaceObject.handlers.menu.start.buttons.click.solo = function(event) {

            // Tirando os efeitos de hover dos botões do menu principal

            for( let i = 0, len = interfaceObject.menu.start.buttons.mouseenter.length; i < len; i++ ) {
                
                interfaceObject.menu.start.buttons.mouseenter[i].style.maxHeight = '';
                
                interfaceObject.menu.start.buttons.mouseenter[i].classList.remove("menu-botao-hover");

            }

            // Trocando o menu principal pelo menu de jogo solo

            interfaceObject.menu.start.main.replaceWith(interfaceObject.menu.solo.main);

        }

        interfaceObject.handlers.menu.start.buttons.click.duo = function(event) {

            // Tirando os efeitos de hover dos botões do menu principal

            for( let i = 0, len = interfaceObject.menu.start.buttons.mouseenter.length; i < len; i++ ) {

                interfaceObject.menu.start.buttons.mouseenter[i].style.maxHeight = '';

                interfaceObject.menu.start.buttons.mouseenter[i].classList.remove("menu-botao-hover");

            }

            // Trocando o menu principal pelo menu de jogo duo

            interfaceObject.menu.start.main.replaceWith(interfaceObject.menu.duo.main);

        }

        // Criando os handlers do menu de jogo solo

        interfaceObject.handlers.menu.solo = {};

        interfaceObject.handlers.menu.solo.buttons = {};

        interfaceObject.handlers.menu.solo.buttons.click = {};

        interfaceObject.handlers.menu.solo.buttons.click.back = function(event) {

            // Trocando o menu de jogo solo pelo menu principal

            interfaceObject.menu.solo.main.replaceWith(interfaceObject.menu.start.main);

        }

        interfaceObject.handlers.menu.solo.buttons.click.easy = function(event) {

            // Escolhe uma palavra do nivel fácil

            const easyLength = resourcesObject.words.default.easy.length;

            const randomIndex = Math.floor(Math.random() * (easyLength - 1));

            const word = resourcesObject.words.default.easy[randomIndex].normalizeWord();

            gameObject.resetDisable();

            gameObject.word = word;

            gameObject.init();

            interfaceObject.menu.solo.main.replaceWith(interfaceObject.menu.forca.main);

        }

        interfaceObject.handlers.menu.solo.buttons.click.normal = function(event) {

            // Escolhe uma palavra do nivel normal

            const normalLength = resourcesObject.words.default.normal.length;

            const randomIndex = Math.floor(Math.random() * (normalLength - 1));

            const word = resourcesObject.words.default.normal[randomIndex].normalizeWord();

            gameObject.resetDisable();

            gameObject.word = word;

            gameObject.init();

            interfaceObject.menu.solo.main.replaceWith(interfaceObject.menu.forca.main);

        }

        interfaceObject.handlers.menu.solo.buttons.click.hard = function(event) {

            // Escolhe uma palavra do nivel difícil

            const hardLength = resourcesObject.words.default.hard.length;

            const randomIndex = Math.floor(Math.random() * (hardLength - 1));

            const word = resourcesObject.words.default.hard[randomIndex].normalizeWord();

            gameObject.resetDisable();

            gameObject.word = word;

            gameObject.init();

            interfaceObject.menu.solo.main.replaceWith(interfaceObject.menu.forca.main);

        }

        // Criando os handlers do menu de jogo duo

        interfaceObject.handlers.menu.duo = {};

        interfaceObject.handlers.menu.duo.buttons = {};

        interfaceObject.handlers.menu.duo.buttons.click = {};

        interfaceObject.handlers.menu.duo.buttons.click.back = function(event) {

            // Limpando o input de palavra secreta

            interfaceObject.menu.duo.input.value = '';

            // Trocando o menu de jogo duo pelo menu principal

            interfaceObject.menu.duo.main.replaceWith(interfaceObject.menu.start.main);

        }

        interfaceObject.handlers.menu.duo.buttons.confirm = {};

        interfaceObject.handlers.menu.duo.buttons.confirm.click = function(event) {

            // Verifica se o input está vazio ou contém apenas espaços em branco

            if( interfaceObject.menu.duo.input.value.trim().empty() ) {

                interfaceObject.menu.duo.input.value = '';

                alert("Por favor, digite uma palavra válida.");

                return;

            }

            // Verifica se o input contém apenas caracteres alpha

            if( ! interfaceObject.menu.duo.input.value.isAlpha() ) {
                
                alert("São permitidos apenas letras de A a Z, sem acentos, números, cedilhas ou caracteres especiais. Por favor, digite uma palavra válida.");
                
                interfaceObject.menu.duo.input.value = interfaceObject.menu.duo.input.value.preg_replace(/[^a-zA-Z]/g, '');

                return;

            }

            // Verifica se o input contém mais de 15 caracteres

            if( interfaceObject.menu.duo.input.value.visibleLength() > 15 ) {

                alert("A palavra secreta não pode ter mais de 15 letras. Por favor, digite uma palavra válida.");

                interfaceObject.menu.duo.input.value = interfaceObject.menu.duo.input.value.slice(0, 15);

                return;

            }

            // Após confirmado que a palvra é valida, coloca ela para uppercase e normaliza

            interfaceObject.menu.duo.input.value = interfaceObject.menu.duo.input.value.normalizeWord();

            const word = interfaceObject.menu.duo.input.value;

            // Determina a dificuldade da palavra

            let difficulty = "easy";

            if( word.visibleLength() > 6 && word.visibleLength() <= 9 ) {

                difficulty = "normal";

            } else if( word.visibleLength() > 9 ) {

                difficulty = "hard";

            }

            gameObject.resetDisable();

            gameObject.word = word;

            gameObject.init();

            interfaceObject.menu.duo.main.replaceWith(interfaceObject.menu.forca.main);

            interfaceObject.menu.duo.input.value = '';

        }

        interfaceObject.handlers.menu.duo.input = {};

        interfaceObject.handlers.menu.duo.input.input = function(event) {

            // Verifica se o input está vazio ou contém apenas espaços em branco

            if( interfaceObject.menu.duo.input.value.trim().empty() ) {

                interfaceObject.menu.duo.input.value = '';

                return;

            }

            // Verifica se o input contém apenas caracteres alpha

            if( ! interfaceObject.menu.duo.input.value.isAlpha() ) {
            
                alert("São permitidos apenas letras de A a Z, sem acentos, números, cedilhas ou caracteres especiais. Por favor, digite uma palavra válida.");
            
                interfaceObject.menu.duo.input.value = interfaceObject.menu.duo.input.value.preg_replace(/[^a-zA-Z]/g, '');

                return;

            }

            // Verifica se o input contém mais de 15 caracteres

            if( interfaceObject.menu.duo.input.value.visibleLength() > 15 ) {

                alert("A palavra secreta não pode ter mais de 15 letras. Por favor, digite uma palavra válida.");

                interfaceObject.menu.duo.input.value = interfaceObject.menu.duo.input.value.slice(0, 15);

                return;

            }

        }

        interfaceObject.handlers.menu.duo.input.change = function(event) {

            // Verifica se o input está vazio ou contém apenas espaços em branco

            if( interfaceObject.menu.duo.input.value.trim().empty() ) {

                interfaceObject.menu.duo.input.value = '';

                return;

            }

            // Verifica se o input contém apenas caracteres alpha

            if( ! interfaceObject.menu.duo.input.value.isAlpha() ) {
            
                alert("São permitidos apenas letras de A a Z, sem acentos, números, cedilhas ou caracteres especiais. Por favor, digite uma palavra válida.");
            
                interfaceObject.menu.duo.input.value = interfaceObject.menu.duo.input.value.preg_replace(/[^a-zA-Z]/g, '');

                return;

            }

            // Verifica se o input contém mais de 15 caracteres

            if( interfaceObject.menu.duo.input.value.visibleLength() > 15 ) {

                alert("A palavra secreta não pode ter mais de 15 letras. Por favor, digite uma palavra válida.");

                interfaceObject.menu.duo.input.value = interfaceObject.menu.duo.input.value.slice(0, 15);

                return;

            }

        }    
        
        // Criando os handlers do menu de jogo da forca

        interfaceObject.handlers.menu.forca = {};

        interfaceObject.handlers.menu.forca.buttons = {};

        interfaceObject.handlers.menu.forca.buttons.click = {};

        interfaceObject.handlers.menu.forca.buttons.click.back = function(event) {

            gameObject.resetDisable();

            interfaceObject.menu.forca.main.replaceWith(interfaceObject.menu.start.main);

        }

        interfaceObject.handlers.menu.forca.input = {};

        interfaceObject.handlers.menu.forca.input.input = function(event) {

            // Verifica se o input está vazio ou contém apenas espaços em branco

            if( interfaceObject.menu.forca.input.value.trim().empty() ) {

                interfaceObject.menu.forca.input.value = '';

                return;

            }

            // Verifica se o input contém apenas caracteres alpha

            if( ! interfaceObject.menu.forca.input.value.isAlpha() ) {
            
                alert("São permitidos apenas letras de A a Z, sem acentos, números, cedilhas ou caracteres especiais. Por favor, digite uma letra válida.");
            
                interfaceObject.menu.forca.input.value = interfaceObject.menu.forca.input.value.preg_replace(/[^a-zA-Z]/g, '');

                return;

            }

            // Verifica se o input contém mais de 1 caractere

            if( interfaceObject.menu.forca.input.value.visibleLength() > 1 ) {

                alert("A letra deve ser apenas um caractere. Por favor, digite uma letra válida.");

                interfaceObject.menu.forca.input.value = interfaceObject.menu.forca.input.value.slice(0, 1);

                return;

            }

        }

        interfaceObject.handlers.menu.forca.input.change = function(event) {

            // Verifica se o input está vazio ou contém apenas espaços em branco

            if( interfaceObject.menu.forca.input.value.trim().empty() ) {

                interfaceObject.menu.forca.input.value = '';

                return;

            }

            // Verifica se o input contém apenas caracteres alpha

            if( ! interfaceObject.menu.forca.input.value.isAlpha() ) {
            
                alert("São permitidos apenas letras de A a Z, sem acentos, números, cedilhas ou caracteres especiais. Por favor, digite uma letra válida.");
            
                interfaceObject.menu.forca.input.value = interfaceObject.menu.forca.input.value.preg_replace(/[^a-zA-Z]/g, '');

                return;

            }

            // Verifica se o input contém mais de 1 caractere

            if( interfaceObject.menu.forca.input.value.visibleLength() > 1 ) {

                alert("A letra deve ser apenas um caractere. Por favor, digite uma letra válida.");

                interfaceObject.menu.forca.input.value = interfaceObject.menu.forca.input.value.slice(0, 1);

                return;

            }

        }

        interfaceObject.handlers.menu.forca.input.keydown = function(event) {

            if(event.key === "Enter") gameObject.verificarLetras(interfaceObject.menu.forca.input.value);

        }


        // Criando os listeners do menu principal

        interfaceObject.menu.start.buttons.solo.button.addEventListener("mouseenter", interfaceObject.handlers.menu.start.buttons.mouseenter);
        interfaceObject.menu.start.buttons.solo.button.addEventListener("mouseleave", interfaceObject.handlers.menu.start.buttons.mouseleave);
        interfaceObject.menu.start.buttons.solo.button.addEventListener("click", interfaceObject.handlers.menu.start.buttons.click.solo);

        interfaceObject.menu.start.buttons.duo.button.addEventListener("mouseenter", interfaceObject.handlers.menu.start.buttons.mouseenter);
        interfaceObject.menu.start.buttons.duo.button.addEventListener("mouseleave", interfaceObject.handlers.menu.start.buttons.mouseleave);
        interfaceObject.menu.start.buttons.duo.button.addEventListener("click", interfaceObject.handlers.menu.start.buttons.click.duo);

        interfaceObject.menu.start.buttons.options.button.addEventListener("mouseenter", interfaceObject.handlers.menu.start.buttons.mouseenter);
        interfaceObject.menu.start.buttons.options.button.addEventListener("mouseleave", interfaceObject.handlers.menu.start.buttons.mouseleave);

        interfaceObject.menu.start.buttons.exit.button.addEventListener("mouseenter", interfaceObject.handlers.menu.start.buttons.mouseenter);
        interfaceObject.menu.start.buttons.exit.button.addEventListener("mouseleave", interfaceObject.handlers.menu.start.buttons.mouseleave);

        // Criando os listeners do menu de jogo solo

        interfaceObject.menu.solo.buttons.back.button.addEventListener("click", interfaceObject.handlers.menu.solo.buttons.click.back);

        interfaceObject.menu.solo.buttons.easy.button.addEventListener("click", interfaceObject.handlers.menu.solo.buttons.click.easy);

        interfaceObject.menu.solo.buttons.normal.button.addEventListener("click", interfaceObject.handlers.menu.solo.buttons.click.normal);

        interfaceObject.menu.solo.buttons.hard.button.addEventListener("click", interfaceObject.handlers.menu.solo.buttons.click.hard);

        // Criando os listeners do menu de jogo duo

        interfaceObject.menu.duo.buttons.back.button.addEventListener("click", interfaceObject.handlers.menu.duo.buttons.click.back);

        interfaceObject.menu.duo.input.addEventListener("input", interfaceObject.handlers.menu.duo.input.input);

        interfaceObject.menu.duo.input.addEventListener("change", interfaceObject.handlers.menu.duo.input.change);

        interfaceObject.menu.duo.buttons.confirm.button.addEventListener("click", interfaceObject.handlers.menu.duo.buttons.confirm.click);

        // Criando os listeners do menu de jogo da forca

        interfaceObject.menu.forca.buttons.back.button.addEventListener("click", interfaceObject.handlers.menu.forca.buttons.click.back);

        interfaceObject.menu.forca.input.addEventListener("input", interfaceObject.handlers.menu.forca.input.input);

        interfaceObject.menu.forca.input.addEventListener("change", interfaceObject.handlers.menu.forca.input.change);

        interfaceObject.menu.forca.input.addEventListener("keydown", interfaceObject.handlers.menu.forca.input.keydown);

        interfaceObject.body.append(interfaceObject.menu.start.main);

        document.documentElement.replaceWith(interfaceObject.body);

    }

});

document.addEventListener("DOMContentLoaded", function() {

    initializeObject.init();

});