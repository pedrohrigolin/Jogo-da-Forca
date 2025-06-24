const resourcesObject = {

    words: {
        default: {},
        custom: {}
    }

};

const interfaceObject = {};

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

            // Criar elemento SVG principal
            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "209");
            svg.setAttribute("height", "311");
            svg.setAttribute("viewBox", "0 0 209 311");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            // Path 1
            const path1 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path1.setAttribute("d", "M178 254C168.4 258.8 161 257 158.5 255.5L46.5 211C40.1 208.2 38.8333 203.833 39 202C39.4 184.4 24.8333 184.333 17.5 186.5C3.9 194.1 9.83334 206 14.5 211C2.9 215 0.666671 224 1 228C2.6 246.8 17.6667 243.5 25 239.5C31.4 232.7 38.3333 234.333 41 236L142 278.5C152.4 280.5 153.667 286.333 153 289C152.2 305.8 164 310 170 310C190.8 308.4 188.333 292 184.5 284C204.5 278.4 201.833 264.333 198 258C195.333 254.667 187.6 249.2 178 254Z");
            path1.setAttribute("fill", "#EAEAEA");
            svg.appendChild(path1);

            // Path 2
            const path2 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path2.setAttribute("d", "M31.6869 254.468C41.2869 259.268 48.6869 257.468 51.1869 255.968L163.187 211.468C169.587 208.668 170.854 204.302 170.687 202.468C170.287 184.868 184.854 184.802 192.187 186.968C205.787 194.568 199.854 206.468 195.187 211.468C206.787 215.468 209.02 224.468 208.687 228.468C207.087 247.268 192.02 243.968 184.687 239.968C178.287 233.168 171.354 234.802 168.687 236.468L67.6869 278.968C57.2869 280.968 56.0202 286.802 56.6869 289.468C57.4869 306.268 45.6869 310.468 39.6869 310.468C18.8869 308.868 21.3535 292.468 25.1869 284.468C5.18687 278.868 7.85355 264.802 11.6869 258.468C14.3535 255.135 22.0869 249.668 31.6869 254.468Z");
            path2.setAttribute("fill", "#EAEAEA");
            svg.appendChild(path2);

            // Path 3 (corpo principal)
            const path3 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path3.setAttribute("d", "M72.5951 197.526C77.7951 208.726 103.762 209.86 116.095 209.026C134.595 208.026 156.095 200.026 156.595 186.526C157.095 173.026 156.595 172.026 171.595 164.026C186.595 156.026 184.595 146.526 182.095 135.026C179.595 123.526 182.095 124.526 182.095 120.526C182.095 116.526 192.595 100.026 178.595 60.0262C164.595 20.0262 123.095 4.0263 111.095 2.5263C99.0951 1.0263 69.0951 -7.9737 35.0951 20.5263C1.09511 49.0263 4.09511 90.0263 6.09511 108.526C8.09511 127.026 21.0951 141.026 22.5951 142.526C24.0951 144.026 24.0951 153.026 25.5951 157.526C27.0951 162.026 33.5951 170.526 48.0951 171.526C62.5951 172.526 66.0951 183.526 72.5951 197.526Z");
            path3.setAttribute("fill", "#EAEAEA");
            svg.appendChild(path3);

            // Path 4 (olho)
            const path4 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path4.setAttribute("d", "M101.5 153.5L112 134.5C116 129.3 120 132.333 121.5 134.5L128 149C132.8 155.8 130 160.167 128 161.5C125.2 163.1 121.5 160.833 120 159.5C116 156.7 113.667 158.333 113 159.5C109 163.1 105.333 162.333 104 161.5C100.8 160.3 101 155.667 101.5 153.5Z");
            path4.setAttribute("fill", "#1B1919");
            path4.setAttribute("stroke", "black");
            path4.setAttribute("stroke-width", "3");
            svg.appendChild(path4);

            // Path 5 (narina esquerda)
            const path5 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path5.setAttribute("d", "M64.5 90C39.7 107.2 49.5 127.833 57.5 136C61.5 139.333 72.1 144.5 82.5 138.5C95.5 131 104 117 96.5 98C90.5 82.8 72.6667 86.3334 64.5 90Z");
            path5.setAttribute("fill", "#1B1919");
            path5.setAttribute("stroke", "black");
            path5.setAttribute("stroke-width", "3");
            svg.appendChild(path5);

            // Path 6 (narina direita)
            const path6 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path6.setAttribute("d", "M150.5 138C127.7 135.6 127 110.333 129.5 97.9999C130.187 92.7999 136.786 89.1666 140 87.9999C162.4 83.9999 169.333 99.6666 170 108C171.6 133.6 157.667 138.667 150.5 138Z");
            path6.setAttribute("fill", "#1B1919");
            path6.setAttribute("stroke", "black");
            path6.setAttribute("stroke-width", "3");
            svg.appendChild(path6);

            // Mask 1
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

            // Group with mask 1
            const g1 = document.createElementNS("http://www.w3.org/2000/svg", "g");
            g1.setAttribute("mask", "url(#mask0_7_237)");

            const g1Path = document.createElementNS("http://www.w3.org/2000/svg", "path");
            g1Path.setAttribute("d", "M72.5951 197.526C77.7951 208.726 103.762 209.86 116.095 209.026C134.595 208.026 156.095 200.026 156.595 186.526C157.095 173.026 156.595 172.026 171.595 164.026C186.595 156.026 184.595 146.526 182.095 135.026C179.595 123.526 182.095 124.526 182.095 120.526C182.095 116.526 192.595 100.026 178.595 60.0262C164.595 20.0262 123.095 4.0263 111.095 2.5263C99.0951 1.0263 69.0951 -7.9737 35.0951 20.5263C1.09511 49.0263 4.09511 90.0263 6.09511 108.526C8.09511 127.026 21.0951 141.026 22.5951 142.526C24.0951 144.026 24.0951 153.026 25.5951 157.526C27.0951 162.026 33.5951 170.526 48.0951 171.526C62.5951 172.526 66.0951 183.526 72.5951 197.526Z");
            g1Path.setAttribute("fill", "#D9D9D9");
            g1.appendChild(g1Path);
            svg.appendChild(g1);

            // Path 7 (detalhe de sombra)
            const path7 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path7.setAttribute("d", "M74.5 172C79.3 191.2 86.8333 203.333 90 207C77.6 205 70.5 197.833 68.5 194.5C63.7 175.7 54.3333 171.667 51.5 172L47.5 171L43.5 170L39 169.333C25.8 167.733 23.3333 150.833 23.5 142.833C7.50001 126.033 0.682915 100 2 88C3 60.5 12.7828 37.426 29.5 22.5C43.5 10 62.6667 3.33333 74.5 1.5C31.7 19.9 21.6667 53.5 19.5 70.5C13.5 102.9 26.6667 119.667 34 124V136C34.1667 143.5 38.4 158.9 54 160.5C69.6 162.1 74.1667 168.833 74.5 172Z");
            path7.setAttribute("fill", "#D9D9D9");
            svg.appendChild(path7);

            // Path 8 (detalhe da cauda)
            const path8 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path8.setAttribute("d", "M51.5 286C52.7 303.2 41 308.5 35 309C51 310.2 56 302.833 56.5 299C55.3 287.4 59.6667 283.167 62 282.5C94.5 268.5 160.7 240 165.5 238C171.5 235.5 176.5 234.5 185.5 241.5C192.7 247.1 199.833 243.167 202.5 240.5C196 242.5 191.5 243.5 184 234.5C178 227.3 169.167 228.167 165.5 229.5C137.167 242.333 77.4 269.1 65 273.5C52.6 277.9 50.8333 283.667 51.5 286Z");
            path8.setAttribute("fill", "#D9D9D9");
            svg.appendChild(path8);

            // Path 9 (detalhe das costas)
            const path9 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path9.setAttribute("d", "M120 262.5L144.5 250L140.5 247L105 263C112.667 267 131.9 275.3 141.5 278.5C152.3 282.1 152 288.5 152.5 292C152.5 307.2 164.833 310 170.5 310C157 305.5 155.5 293.5 157 286.5C158.2 280.9 152.5 276.833 149.5 275.5L120 262.5Z");
            path9.setAttribute("fill", "#D9D9D9");
            svg.appendChild(path9);

            // Path 10 (detalhe da pata)
            const path10 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path10.setAttribute("d", "M43.5 229.5L81.5 245L70.5 250C65.6667 247.667 53.5 242 43.5 238C31 233 33 236.5 21 242C11.4 246.4 4.66667 239.167 2.5 235C5 238 13.5 243.5 23 233.5C30.6 225.5 39.8333 227.5 43.5 229.5Z");
            path10.setAttribute("fill", "#D9D9D9");
            svg.appendChild(path10);

            return svg;

        },

        forca: function() {

            // Create main SVG element
            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "160");
            svg.setAttribute("height", "387");
            svg.setAttribute("viewBox", "0 0 160 387");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            // Path 1
            const path1 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path1.setAttribute("d", "M21.2186 307.5C9.61855 323.5 6.71855 338.5 6.71855 344C1.11855 330.8 0.051885 316.833 0.218552 311.5C4.21855 304.3 17.2186 295.833 23.2186 292.5C21.6186 298.5 21.2186 305 21.2186 307.5Z");
            path1.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path1);

            // Path 2
            const path2 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path2.setAttribute("d", "M28.2186 280.5C15.4186 281.7 5.55188 285.667 2.21855 287.5C2.21855 281.5 6.88522 273.333 9.21855 270C16.0186 264.8 37.3852 259.167 47.2186 257C34.4186 268.2 29.2186 277.333 28.2186 280.5Z");
            path2.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path2);

            // Path 3
            const path3 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path3.setAttribute("d", "M64.2186 235.5C57.8186 243.5 32.8852 250.5 21.2186 253L51.2186 217.5L77.2186 214.5C75.5519 218.167 70.6186 227.5 64.2186 235.5Z");
            path3.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path3);

            // Path 4
            const path4 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path4.setAttribute("d", "M93.2186 78.5L64.7186 107.5V111H93.2186V78.5Z");
            path4.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path4);

            // Path 5
            const path5 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path5.setAttribute("d", "M93.2186 64L64.7186 92V64L93.2186 34V64Z");
            path5.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path5);

            // Path 6
            const path6 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path6.setAttribute("d", "M91.7186 20.5L64.7186 42.5V17L78.2186 0H91.7186V20.5Z");
            path6.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path6);

            // Path 7
            const path7 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path7.setAttribute("d", "M98.7186 203.5C90.7186 209.9 87.7186 221.833 87.2186 227C80.4186 216.6 79.3852 207 79.7186 203.5H98.7186Z");
            path7.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path7);

            // Path 8 (main body with stroke)
            const path8 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path8.setAttribute("d", "M41.7186 191.5C36.9186 180.7 46.3852 173.333 51.7186 171C48.8852 169.833 42.9186 165.3 41.7186 156.5C40.5186 147.7 48.5519 142.833 52.7186 141.5C50.2186 141.667 44.8186 139.2 43.2186 128C41.6186 116.8 51.2186 112.333 56.2186 111.5C62.5519 111.167 79.1186 110.7 94.7186 111.5C114.219 112.5 113.719 119 115.219 126C116.419 131.6 110.052 139 106.719 142C108.552 142.5 112.819 145.6 115.219 154C117.619 162.4 110.552 168.833 106.719 171C115.519 173.8 117.052 182.167 116.719 186C116.719 207 86.7186 203.5 79.2186 203C71.7186 202.5 47.7186 205 41.7186 191.5Z");
            path8.setAttribute("fill", "#D7AF6F");
            path8.setAttribute("stroke", "#B89257");
            path8.setAttribute("stroke-width", "10");
            svg.appendChild(path8);

            // Path 9
            const path9 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path9.setAttribute("d", "M121.719 236.5L119.719 264.5C110.919 256.5 102.719 247.5 99.7186 244L104.719 214C111.119 224.8 118.719 233.5 121.719 236.5Z");
            path9.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path9);

            // Path 10
            const path10 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path10.setAttribute("d", "M152.719 276.5C153.519 281.7 142.719 294.667 137.219 300.5C137.219 286.9 131.219 277.833 128.219 275L135.219 249C140.719 256 151.919 271.3 152.719 276.5Z");
            path10.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path10);

            // Path 11
            const path11 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path11.setAttribute("d", "M36.7186 345.5V373C26.7186 366.6 20.2186 359.667 18.2186 357C17.0186 344.2 21.0519 326 23.2186 318.5C25.6186 333.7 33.2186 342.833 36.7186 345.5Z");
            path11.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path11);

            // Path 12
            const path12 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path12.setAttribute("d", "M68.7186 363C57.5186 361.4 49.3852 356.333 46.7186 354C45.5186 368.4 52.8852 379.667 56.7186 383.5C74.3186 388.7 92.3852 385.667 99.2186 383.5C84.0186 380.7 72.5519 368.667 68.7186 363Z");
            path12.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path12);

            // Path 13
            const path13 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path13.setAttribute("d", "M112.719 354C103.919 360 93.3852 362.5 89.2186 363C98.4186 373.4 112.052 376.667 117.719 377C134.919 369 145.552 352.667 148.719 345.5C137.919 352.7 120.219 354.167 112.719 354Z");
            path13.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path13);

            // Path 14
            const path14 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            path14.setAttribute("d", "M156.719 325C155.119 331.8 135.385 338.5 125.719 341C133.319 335.8 137.219 322.167 138.219 316C148.219 312.8 156.385 301 159.219 295.5C160.219 299.5 158.719 316.5 156.719 325Z");
            path14.setAttribute("fill", "#D7AF6F");
            svg.appendChild(path14);

            return svg;

        },

        easy: function() {

            // Criar elemento SVG principal
            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "213");
            svg.setAttribute("height", "210");
            svg.setAttribute("viewBox", "0 0 213 210");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            // Primeiro ellipse (fundo verde)
            const ellipse1 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse1.setAttribute("cx", "106.5");
            ellipse1.setAttribute("cy", "105");
            ellipse1.setAttribute("rx", "106.5");
            ellipse1.setAttribute("ry", "105");
            ellipse1.setAttribute("fill", "#1CD428");
            svg.appendChild(ellipse1);

            // Segundo ellipse (olho esquerdo - parte preta)
            const ellipse2 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse2.setAttribute("cx", "68.5");
            ellipse2.setAttribute("cy", "54.5");
            ellipse2.setAttribute("rx", "68.5");
            ellipse2.setAttribute("ry", "54.5");
            ellipse2.setAttribute("transform", "matrix(1 0 0 -1 37 167)");
            ellipse2.setAttribute("fill", "black");
            svg.appendChild(ellipse2);

            // Terceiro ellipse (olho esquerdo - parte verde)
            const ellipse3 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse3.setAttribute("cx", "68.5");
            ellipse3.setAttribute("cy", "58.5");
            ellipse3.setAttribute("rx", "68.5");
            ellipse3.setAttribute("ry", "58.5");
            ellipse3.setAttribute("transform", "matrix(1 0 0 -1 37 159)");
            ellipse3.setAttribute("fill", "#1CD428");
            svg.appendChild(ellipse3);

            // Primeiro circle (pupila esquerda)
            const circle1 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle1.setAttribute("cx", "58");
            circle1.setAttribute("cy", "72");
            circle1.setAttribute("r", "21");
            circle1.setAttribute("fill", "black");
            svg.appendChild(circle1);

            // Segundo circle (pupila direita)
            const circle2 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle2.setAttribute("cx", "156");
            circle2.setAttribute("cy", "72");
            circle2.setAttribute("r", "21");
            circle2.setAttribute("fill", "black");
            svg.appendChild(circle2);

            // Terceiro circle (reflexo olho esquerdo)
            const circle3 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle3.setAttribute("cx", "54");
            circle3.setAttribute("cy", "84");
            circle3.setAttribute("r", "21");
            circle3.setAttribute("fill", "#1CD428");
            svg.appendChild(circle3);

            // Quarto circle (reflexo olho direito)
            const circle4 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle4.setAttribute("cx", "160");
            circle4.setAttribute("cy", "84");
            circle4.setAttribute("r", "21");
            circle4.setAttribute("fill", "#1CD428");
            svg.appendChild(circle4);

            return svg;

        },

        normal: function() {

            // Create the main SVG element
            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "213");
            svg.setAttribute("height", "210");
            svg.setAttribute("viewBox", "0 0 213 210");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            // Create the first ellipse (yellow background)
            const ellipse1 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse1.setAttribute("cx", "106.5");
            ellipse1.setAttribute("cy", "105");
            ellipse1.setAttribute("rx", "106.5");
            ellipse1.setAttribute("ry", "105");
            ellipse1.setAttribute("fill", "#D4CE1C");
            svg.appendChild(ellipse1);

            // Create the second ellipse (black mouth)
            const ellipse2 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse2.setAttribute("cx", "68.5");
            ellipse2.setAttribute("cy", "37.5");
            ellipse2.setAttribute("rx", "68.5");
            ellipse2.setAttribute("ry", "37.5");
            ellipse2.setAttribute("transform", "matrix(1 0 0 -1 37 167)");
            ellipse2.setAttribute("fill", "black");
            svg.appendChild(ellipse2);

            // Create the third ellipse (yellow mouth cover)
            const ellipse3 = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipse3.setAttribute("cx", "80");
            ellipse3.setAttribute("cy", "38");
            ellipse3.setAttribute("rx", "80");
            ellipse3.setAttribute("ry", "38");
            ellipse3.setAttribute("transform", "matrix(1 0 0 -1 26 158)");
            ellipse3.setAttribute("fill", "#D4CE1C");
            svg.appendChild(ellipse3);

            // Create the first circle (left eye)
            const circle1 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle1.setAttribute("cx", "58");
            circle1.setAttribute("cy", "72");
            circle1.setAttribute("r", "21");
            circle1.setAttribute("fill", "black");
            svg.appendChild(circle1);

            // Create the second circle (right eye)
            const circle2 = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            circle2.setAttribute("cx", "156");
            circle2.setAttribute("cy", "72");
            circle2.setAttribute("r", "21");
            circle2.setAttribute("fill", "black");
            svg.appendChild(circle2);

            return svg;

        },

        hard: function() {

            // Create main SVG element
            const svg = document.createElementNS("http://www.w3.org/2000/svg", "svg");
            svg.setAttribute("width", "213");
            svg.setAttribute("height", "210");
            svg.setAttribute("viewBox", "0 0 213 210");
            svg.setAttribute("fill", "none");
            svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");

            // Create red background ellipse
            const ellipseBg = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipseBg.setAttribute("cx", "106.5");
            ellipseBg.setAttribute("cy", "105");
            ellipseBg.setAttribute("rx", "106.5");
            ellipseBg.setAttribute("ry", "105");
            ellipseBg.setAttribute("fill", "#D41C1C");
            svg.appendChild(ellipseBg);

            // Create black mouth shape (path 1)
            const pathMouth1 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            pathMouth1.setAttribute("d", "M176.5 166.5C176.5 196.6 145.832 182.5 108 182.5C70.1685 182.5 39.5 196.6 39.5 166.5C39.5 136.4 70.1685 112 108 112C145.832 112 176.5 136.4 176.5 166.5Z");
            pathMouth1.setAttribute("fill", "black");
            svg.appendChild(pathMouth1);

            // Create red mouth cover (path 2)
            const pathMouth2 = document.createElementNS("http://www.w3.org/2000/svg", "path");
            pathMouth2.setAttribute("d", "M176 172.5C176 204.809 145.832 185 108 185C70.1685 185 39 204.809 39 172.5C39 140.191 70.1685 120 108 120C145.832 120 176 140.191 176 172.5Z");
            pathMouth2.setAttribute("fill", "#D41C1C");
            svg.appendChild(pathMouth2);

            // Create left eye (ellipse)
            const ellipseLeftEye = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipseLeftEye.setAttribute("cx", "68");
            ellipseLeftEye.setAttribute("cy", "74.5");
            ellipseLeftEye.setAttribute("rx", "25");
            ellipseLeftEye.setAttribute("ry", "24.5");
            ellipseLeftEye.setAttribute("fill", "black");
            svg.appendChild(ellipseLeftEye);

            // Create right eye (ellipse)
            const ellipseRightEye = document.createElementNS("http://www.w3.org/2000/svg", "ellipse");
            ellipseRightEye.setAttribute("cx", "151");
            ellipseRightEye.setAttribute("cy", "72");
            ellipseRightEye.setAttribute("rx", "26");
            ellipseRightEye.setAttribute("ry", "27");
            ellipseRightEye.setAttribute("fill", "black");
            svg.appendChild(ellipseRightEye);

            // Create left horn (path)
            const pathLeftHorn = document.createElementNS("http://www.w3.org/2000/svg", "path");
            pathLeftHorn.setAttribute("d", "M98 35V96L40 49.0769L98 35Z");
            pathLeftHorn.setAttribute("fill", "#D41C1C");
            svg.appendChild(pathLeftHorn);

            // Create right horn (path)
            const pathRightHorn = document.createElementNS("http://www.w3.org/2000/svg", "path");
            pathRightHorn.setAttribute("d", "M116 32V99L176 47.4615L116 32Z");
            pathRightHorn.setAttribute("fill", "#D41C1C");
            svg.appendChild(pathRightHorn);

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

        initializeObject.changeSubStatus("Criando o menu inicial...");

        interfaceObject.menu = {};

        interfaceObject.menu.start = {};

        interfaceObject.menu.start.main = document.createElement("main");

        interfaceObject.menu.start.main.classList.add("container");

        interfaceObject.menu.start.main.classList.add("startMenu");

    }

});