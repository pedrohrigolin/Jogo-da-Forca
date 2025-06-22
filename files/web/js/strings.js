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


String.prototype.toUpperCase = function(){
    return callUserFunc.sync("toUpperCase", this.toString());
}

String.prototype.toLowerCase = function(){
    return callUserFunc.sync("toLowerCase", this.toString());
}

String.prototype.removeSpaces = function(){
    return callUserFunc.sync("removeSpaces", this.toString());
}

String.prototype.removeAcentos = function(){
    return callUserFunc.sync("removeAcentos", this.toString());
}

String.prototype.trim = function(){
    return callUserFunc.sync("trim", this.toString());
}

String.prototype.rtrim = function(){
    return callUserFunc.sync("rtrim", this.toString());
}

String.prototype.ltrim = function(){
    return callUserFunc.sync("ltrim", this.toString());
}

String.prototype.trimStart = function(){
    return callUserFunc.sync("rtrim", this.toString());
}

String.prototype.trimEnd = function(){
    return callUserFunc.sync("ltrim", this.toString());
}

String.prototype.normalizeWord = function(){
    return callUserFunc.sync("normalizeWord", this.toString());
}

String.prototype.normalizeLineBreaks = function(){
    return callUserFunc.sync("normalizeLineBreaks", this.toString());
}

String.prototype.removeExtraLineBreaks = function(normalize = true){
    return callUserFunc.sync("removeExtraLineBreaks", this.toString(), normalize);
}

String.prototype.isAlpha = function(){
    return callUserFunc.sync("checkAlphaCharacters", this.toString());
}

String.prototype.concat = function(...string){
    return callUserFunc.sync("concat", this.toString(), ...string);
}

String.prototype.includes = function(string){
    return callUserFunc.sync("includes", this.toString(), string);
}

String.prototype.indexOf = function(string){
    return callUserFunc.sync("firstIndexOf", this.toString(), string);
}

String.prototype.firstIndexOf = function(string){
    return callUserFunc.sync("firstIndexOf", this.toString(), string);
}

String.prototype.lastIndexOf = function(string){
    return callUserFunc.sync("lastIndexOf", this.toString(), string);
}

String.prototype.startsWith = function(string){
    return callUserFunc.sync("startsWith", this.toString(), string);
}

String.prototype.endsWith = function(string){
    return callUserFunc.sync("endsWith", this.toString(), string);
}

String.prototype.charAt = function(index){
    return callUserFunc.sync("charAt", this.toString(), index);
}

String.prototype.slice = function(start, end){
    return callUserFunc.sync("slice", this.toString(), start, end);
}

String.prototype.substring = function(start, end){
    return callUserFunc.sync("substring", this.toString(), start, end);
}

/**
 * Método depreciado e que foi sobreescrito somente com a intenção de 
 * ser um alias para String.prototype.substring. Para o uso igual era o método
 * original, a alternativa continua sendo usar o String.prototype.slice
 *
 * @global
 * @param	number	start 	
 * @param	number	length	
 * @return	string
 */
String.prototype.substr = function(start, end){
    return callUserFunc.sync("substring", this.toString(), start, end);
}

String.prototype.explode = function(separator = "", limit){
    return callUserFunc.sync("explode", this.toString(), separator, limit);
}

String.prototype.split = function(separator = "", limit){
    return callUserFunc.sync("explode", this.toString(), separator, limit);
}

String.prototype.replace = function(search, replaceValue, offset = 0){
    return callUserFunc.sync("str_replace", this.toString(), search, replaceValue, offset);
}

String.prototype.replaceAll = function(search, replaceValue, offset = 0, limit){
    return callUserFunc.sync("str_replace_all", this.toString(), search, replaceValue, offset, limit);
}

String.prototype.preg_replace = function(search, replaceValue, offset = 0, limit){
    return callUserFunc.sync("preg_replace", this.toString(), search, replaceValue, offset, limit);
}

String.prototype.preg_split = function(search, limit){
    return callUserFunc.sync("preg_split", this.toString(), search, limit);
}