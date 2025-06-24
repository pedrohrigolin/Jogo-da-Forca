/**
 * @file strings.js
 *
 * Implementa um motor próprio de manipulação de strings em JavaScript,
 * sobrescrevendo e criando métodos utilitários diretamente no protótipo de String.
 *
 * Os métodos definidos aqui permitem operações avançadas e padronizadas, como remoção de espaços,
 * normalização de acentuação, conversão de caixa, manipulação de quebras de linha, entre outros.
 *
 * Todas as funções utilizam a interface `callUserFunc.sync` para centralizar a lógica de processamento,
 * tornando o tratamento de strings mais flexível e consistente em toda a aplicação web.
 *
 * Exemplos de métodos adicionados ou sobrescritos:
 * - toUpperCase, toLowerCase, trim, rtrim, ltrim
 * - removeSpaces, removeAcentos, normalizeWord
 * - normalizeLineBreaks, removeExtraLineBreaks, isAlpha
 *
 * Útil para padronizar e facilitar o tratamento de strings, criando uma camada própria de manipulação
 * que pode ser facilmente expandida conforme as necessidades do projeto.
 */

/**
 * @function empty
 * @memberof String.prototype
 * @description
 * Verifica se a string está vazia.
 * Retorna true se a string não possui caracteres, false caso contrário.
 * 
 * @returns {boolean} true se a string estiver vazia, false caso contrário.
 */
String.prototype.empty = function(){
    return this.toString().length === 0;
}

/**
 * @function toUpperCase
 * @memberof String.prototype
 * @description
 * Converte todos os caracteres da string para maiúsculo, respeitando Unicode.
 * Utiliza ICU no backend para garantir suporte a acentuação e caracteres especiais.
 * 
 * @returns {string} String convertida para maiúsculo.
 */
String.prototype.toUpperCase = function(){
    return callUserFunc.sync("toUpperCase", this.toString());
}

/**
 * @function toLowerCase
 * @memberof String.prototype
 * @description
 * Converte todos os caracteres da string para minúsculo, respeitando Unicode.
 * Utiliza ICU no backend para garantir suporte a acentuação e caracteres especiais.
 * 
 * @returns {string} String convertida para minúsculo.
 */
String.prototype.toLowerCase = function(){
    return callUserFunc.sync("toLowerCase", this.toString());
}

/**
 * @function removeSpaces
 * @memberof String.prototype
 * @description
 * Remove todos os espaços em branco da string, incluindo espaços Unicode.
 * Utiliza ICU para identificar e remover todos os tipos de espaçamento.
 * 
 * @returns {string} String sem espaços.
 */
String.prototype.removeSpaces = function(){
    return callUserFunc.sync("removeSpaces", this.toString());
}

/**
 * @function normalize
 * @memberof String.prototype
 * @description
 * Normaliza a string Unicode de acordo com a forma especificada ("NFC", "NFD", "NFKC" ou "NFKD").
 * Garante que diferentes representações de caracteres equivalentes sejam convertidas para uma forma canônica comum.
 * Utiliza ICU no backend. Inspirada em String.prototype.normalize do JavaScript.
 * 
 * @param {string} [form="NFC"] Forma de normalização.
 * @returns {string} String normalizada.
 * @throws {Error} Se a forma de normalização for inválida.
 */
String.prototype.normalize = function(form = "NFC"){
    return callUserFunc.sync("normalize", this.toString(), form);
}

/**
 * @function removeAcentos
 * @memberof String.prototype
 * @description
 * Remove acentos e o caractere "ç" da string, substituindo por seus equivalentes sem acentuação.
 * Focado em palavras do português brasileiro. Utiliza normalização Unicode no backend.
 * 
 * @returns {string} String sem acentos e sem "ç".
 */
String.prototype.removeAcentos = function(){
    return callUserFunc.sync("removeAcentos", this.toString());
}

/**
 * @function trim
 * @memberof String.prototype
 * @description
 * Remove espaços em branco do início e do fim da string, incluindo espaços Unicode.
 * Equivalente ao PHP trim(), utilizando ICU para suporte total a Unicode.
 * 
 * @returns {string} String sem espaços no início e fim.
 */
String.prototype.trim = function(){
    return callUserFunc.sync("trim", this.toString());
}

/**
 * @function rtrim
 * @memberof String.prototype
 * @description
 * Remove espaços em branco do fim da string (right trim), incluindo espaços Unicode.
 * Equivalente ao PHP rtrim(), utilizando ICU para suporte total a Unicode.
 * 
 * @returns {string} String sem espaços no fim.
 */
String.prototype.rtrim = function(){
    return callUserFunc.sync("rtrim", this.toString());
}

/**
 * @function ltrim
 * @memberof String.prototype
 * @description
 * Remove espaços em branco do início da string (left trim), incluindo espaços Unicode.
 * Equivalente ao PHP ltrim(), utilizando ICU para suporte total a Unicode.
 * 
 * @returns {string} String sem espaços no início.
 */
String.prototype.ltrim = function(){
    return callUserFunc.sync("ltrim", this.toString());
}

/**
 * @function trimStart
 * @memberof String.prototype
 * @description
 * Alias para rtrim(). Remove espaços em branco do fim da string.
 * 
 * @returns {string} String sem espaços no fim.
 */
String.prototype.trimStart = function(){
    return callUserFunc.sync("rtrim", this.toString());
}

/**
 * @function trimEnd
 * @memberof String.prototype
 * @description
 * Alias para ltrim(). Remove espaços em branco do início da string.
 * 
 * @returns {string} String sem espaços no início.
 */
String.prototype.trimEnd = function(){
    return callUserFunc.sync("ltrim", this.toString());
}

/**
 * @function normalizeWord
 * @memberof String.prototype
 * @description
 * Normaliza uma palavra removendo espaços, acentos e convertendo para maiúsculo.
 * Combina removeSpaces(), removeAcentos() e toUpperCase() em uma única chamada.
 * 
 * @returns {string} Palavra normalizada.
 */
String.prototype.normalizeWord = function(){
    return callUserFunc.sync("normalizeWord", this.toString());
}

/**
 * @function normalizeLineBreaks
 * @memberof String.prototype
 * @description
 * Normaliza todas as quebras de linha da string para o formato Unix (\n).
 * Converte \r\n, \r, \v, \f, etc. para \n usando regex no backend.
 * 
 * @returns {string} String com quebras de linha normalizadas.
 */
String.prototype.normalizeLineBreaks = function(){
    return callUserFunc.sync("normalizeLineBreaks", this.toString());
}

/**
 * @function removeExtraLineBreaks
 * @memberof String.prototype
 * @description
 * Remove quebras de linha múltiplas consecutivas, substituindo por uma única quebra.
 * Se normalize for true, converte todas as quebras para \n antes de remover extras.
 * 
 * @param {boolean} [normalize=true] Se true, normaliza antes de remover extras.
 * @returns {string} String sem quebras de linha extras.
 */
String.prototype.removeExtraLineBreaks = function(normalize = true){
    return callUserFunc.sync("removeExtraLineBreaks", this.toString(), normalize);
}

/**
 * @function isAlpha
 * @memberof String.prototype
 * @description
 * Verifica se a string contém apenas caracteres alfabéticos (A-Z, a-z).
 * Retorna true se todos os caracteres são letras, false caso contrário.
 * 
 * @returns {boolean} true se todos os caracteres são letras, false caso contrário.
 */
String.prototype.isAlpha = function(){
    return callUserFunc.sync("checkAlphaCharacters", this.toString());
}

/**
 * @function concat
 * @memberof String.prototype
 * @description
 * Concatena a string original com uma ou mais strings adicionais.
 * 
 * @param {...string} string Strings a serem concatenadas.
 * @returns {string} String resultante da concatenação.
 */
String.prototype.concat = function(...string){
    return callUserFunc.sync("concat", this.toString(), ...string);
}

/**
 * @function includes
 * @memberof String.prototype
 * @description
 * Verifica se a string contém a substring informada.
 * 
 * @param {string} string Substring a ser buscada.
 * @returns {boolean} true se a substring for encontrada, false caso contrário.
 */
String.prototype.includes = function(string){
    return callUserFunc.sync("includes", this.toString(), string);
}

/**
 * @function indexOf
 * @memberof String.prototype
 * @description
 * Retorna o índice da primeira ocorrência da substring informada, considerando caracteres visíveis (Unicode).
 * Equivalente a firstIndexOf().
 * 
 * @param {string} string Substring a ser buscada.
 * @returns {number} Índice da primeira ocorrência ou -1 se não encontrada.
 */
String.prototype.indexOf = function(string){
    return callUserFunc.sync("firstIndexOf", this.toString(), string);
}

/**
 * @function firstIndexOf
 * @memberof String.prototype
 * @description
 * Retorna o índice da primeira ocorrência da substring informada, considerando caracteres visíveis (Unicode).
 * 
 * @param {string} string Substring a ser buscada.
 * @returns {number} Índice da primeira ocorrência ou -1 se não encontrada.
 */
String.prototype.firstIndexOf = function(string){
    return callUserFunc.sync("firstIndexOf", this.toString(), string);
}

/**
 * @function lastIndexOf
 * @memberof String.prototype
 * @description
 * Retorna o índice da última ocorrência da substring informada, considerando caracteres visíveis (Unicode).
 * 
 * @param {string} string Substring a ser buscada.
 * @returns {number} Índice da última ocorrência ou -1 se não encontrada.
 */
String.prototype.lastIndexOf = function(string){
    return callUserFunc.sync("lastIndexOf", this.toString(), string);
}

/**
 * @function startsWith
 * @memberof String.prototype
 * @description
 * Verifica se a string começa com a substring informada.
 * 
 * @param {string} string Substring a ser verificada.
 * @returns {boolean} true se a string começar com a substring, false caso contrário.
 */
String.prototype.startsWith = function(string){
    return callUserFunc.sync("startsWith", this.toString(), string);
}

/**
 * @function endsWith
 * @memberof String.prototype
 * @description
 * Verifica se a string termina com a substring informada.
 * 
 * @param {string} string Substring a ser verificada.
 * @returns {boolean} true se a string terminar com a substring, false caso contrário.
 */
String.prototype.endsWith = function(string){
    return callUserFunc.sync("endsWith", this.toString(), string);
}

/**
 * @function charAt
 * @memberof String.prototype
 * @description
 * Retorna o caractere visível (grapheme/code point Unicode) na posição informada.
 * Suporta Unicode corretamente, inspirado em String.prototype.charAt do JS.
 * 
 * @param {number} index Índice do caractere desejado.
 * @returns {string} Caractere na posição informada ou string vazia se fora dos limites.
 */
String.prototype.charAt = function(index){
    return callUserFunc.sync("charAt", this.toString(), index);
}

/**
 * @function repeat
 * @memberof String.prototype
 * @description
 * Repete a string original um número especificado de vezes.
 * Inspirada em String.prototype.repeat do JS.
 * 
 * @param {number} [count=0] Número de repetições.
 * @returns {string} String resultante da repetição.
 */
String.prototype.repeat = function(count = 0){
    return callUserFunc.sync("repeat", this.toString(), count);
}

/**
 * @function slice
 * @memberof String.prototype
 * @description
 * Extrai uma parte da string com base em índices de caracteres visíveis (Unicode).
 * Suporta índices negativos e Unicode corretamente.
 * 
 * @param {number} start Índice inicial.
 * @param {number} [end] Índice final (não incluído).
 * @returns {string} Substring extraída.
 */
String.prototype.slice = function(start, end){
    return callUserFunc.sync("slice", this.toString(), start, end);
}

/**
 * @function substring
 * @memberof String.prototype
 * @description
 * Extrai uma substring com base em índices de caracteres visíveis (Unicode).
 * Se end não for informado, extrai até o final da string.
 * 
 * @param {number} start Índice inicial.
 * @param {number} [end] Índice final (não incluído).
 * @returns {string} Substring extraída.
 */
String.prototype.substring = function(start, end){
    return callUserFunc.sync("substring", this.toString(), start, end);
}

/**
 * @function substr
 * @memberof String.prototype
 * @description
 * Método depreciado, mantido apenas como alias para substring para compatibilidade.
 * Recomenda-se usar slice ou substring.
 * 
 * @param {number} start Índice inicial.
 * @param {number} length Comprimento da substring.
 * @returns {string} Substring extraída.
 */
String.prototype.substr = function(start, end){
    return callUserFunc.sync("substring", this.toString(), start, end);
}

/**
 * @function explode
 * @memberof String.prototype
 * @description
 * Divide a string em um array de substrings usando o separador informado.
 * Inspirada na função explode do PHP, com suporte a Unicode.
 * 
 * @param {string} [separator=""] Separador usado para dividir a string.
 * @param {number} [limit] Limite máximo de divisões.
 * @returns {Array<string>} Array de substrings.
 */
String.prototype.explode = function(separator = "", limit){
    return callUserFunc.sync("explode", this.toString(), separator, limit);
}

/**
 * @function split
 * @memberof String.prototype
 * @description
 * Divide a string em um array de substrings usando o separador informado.
 * Alias para explode, compatível com a API nativa do JS, mas utiliza lógica personalizada no backend.
 * 
 * @param {string|RegExp} [separator=""] Padrão que determina onde cada divisão deve ocorrer.
 * @param {number} [limit] Limite máximo de divisões.
 * @returns {Array<string>} Array de substrings.
 */
String.prototype.split = function(separator = "", limit){
    return callUserFunc.sync("explode", this.toString(), separator, limit);
}

/**
 * @function replace
 * @memberof String.prototype
 * @description
 * Substitui a primeira ocorrência do padrão informado por outro valor.
 * Suporta busca por string ou expressão regular.
 * 
 * @param {string|RegExp} search Valor ou padrão a ser substituído.
 * @param {string} replaceValue Valor de substituição.
 * @param {number} [offset=0] Posição inicial da busca.
 * @returns {string} String resultante da substituição.
 */
String.prototype.replace = function(search, replaceValue, offset = 0){
    return callUserFunc.sync("str_replace", this.toString(), search, replaceValue, offset);
}

/**
 * @function replaceAll
 * @memberof String.prototype
 * @description
 * Substitui todas as ocorrências do padrão informado por outro valor.
 * Suporta busca por string ou expressão regular.
 * 
 * @param {string|RegExp} search Valor ou padrão a ser substituído.
 * @param {string} replaceValue Valor de substituição.
 * @param {number} [offset=0] Posição inicial da busca.
 * @param {number} [limit] Limite máximo de substituições.
 * @returns {string} String resultante da substituição.
 */
String.prototype.replaceAll = function(search, replaceValue, offset = 0, limit){
    return callUserFunc.sync("str_replace_all", this.toString(), search, replaceValue, offset, limit);
}

/**
 * @function preg_replace
 * @memberof String.prototype
 * @description
 * Realiza substituições usando expressões regulares (PCRE2, sintaxe PHP).
 * Suporta grupos capturados ($0, $1, $name) no valor de substituição.
 * 
 * @param {string} search Padrão regex (ex: /regex/flags).
 * @param {string} replaceValue Valor de substituição.
 * @param {number} [offset=0] Posição inicial da busca.
 * @param {number} [limit] Limite máximo de substituições.
 * @returns {string} String resultante da substituição.
 */
String.prototype.preg_replace = function(search, replaceValue, offset = 0, limit){
    return callUserFunc.sync("preg_replace", this.toString(), search, replaceValue, offset, limit);
}

/**
 * @function preg_split
 * @memberof String.prototype
 * @description
 * Divide a string em substrings usando um padrão regex como delimitador (PCRE2, sintaxe PHP).
 * 
 * @param {string} search Padrão regex usado como delimitador.
 * @param {number} [limit] Limite máximo de divisões.
 * @returns {Array<string>} Array de substrings.
 */
String.prototype.preg_split = function(search, limit){
    return callUserFunc.sync("preg_split", this.toString(), search, limit);
}

/**
 * @function search
 * @memberof String.prototype
 * @description
 * Retorna o índice da primeira ocorrência do padrão informado (string ou regex), em unidades UTF-16.
 * Inspirada em String.prototype.search do JS, mas com suporte a regex PCRE2 (PHP).
 * 
 * @param {string|RegExp} search Padrão a ser buscado.
 * @returns {number} Índice da primeira ocorrência ou -1 se não encontrada.
 */
String.prototype.search = function(search){
    return callUserFunc.sync("search", this.toString(), search);
}

/**
 * @function searchAll
 * @memberof String.prototype
 * @description
 * Retorna os índices de todas as ocorrências do padrão informado na string.
 *
 * Esta função é uma versão estendida da função nativa search do JavaScript, permitindo encontrar
 * todos os índices de matches do padrão especificado (string ou RegExp) na string original.
 *
 * O parâmetro opcional limit define o número máximo de matches que serão retornados.
 * Se o padrão for uma string, ele será convertido automaticamente para uma expressão regular.
 *
 * @param {string|RegExp} search  Padrão a ser buscado (string ou expressão regular).
 * @param {number} [limit]        (Opcional) Limite máximo de índices a retornar.
 * @returns {Array<number>} Array com os índices de todas as ocorrências.
 */
String.prototype.searchAll = function(search, limit){
    return callUserFunc.sync("searchAll", this.toString(), search, limit);
}

/**
 * @function visibleLength
 * @memberof String.prototype
 * @description
 * Retorna o tamanho da string em caracteres visíveis (graphemes/code points Unicode), e não em bytes.
 * Utiliza ICU no backend para contagem correta de caracteres Unicode.
 * 
 * @returns {number} Quantidade de caracteres visíveis na string.
 */
String.prototype.visibleLength = function(){
    return callUserFunc.sync("VisibleLength", this.toString());
}