/**
 * @global
 * @file array.js
 *
 * Implementa um motor próprio de manipulação de arrays em JavaScript,
 * sobrescrevendo e criando métodos utilitários diretamente no protótipo de Array.
 *
 * Os métodos definidos aqui permitem operações avançadas e padronizadas, como a junção de elementos
 * com diferentes separadores, de forma centralizada e consistente em toda a aplicação web.
 *
 * Todas as funções utilizam a interface `callUserFunc.sync` para centralizar a lógica de processamento,
 * tornando o tratamento de arrays mais flexível e padronizado.
 *
 * Exemplos de métodos adicionados ou sobrescritos:
 * - implode
 * - join
 *
 * Útil para padronizar e facilitar o tratamento de arrays, criando uma camada própria de manipulação
 * que pode ser facilmente expandida conforme as necessidades do projeto.
 */

/**
 * @function implode
 * @memberof Array.prototype
 * @description
 * Junta todos os elementos do array em uma única string, usando o separador informado.
 * Este método é inspirado na função implode do PHP, mas utiliza o backend C++ para garantir suporte avançado a Unicode e performance.
 * Internamente, chama a função C++ via callUserFunc.sync, que utiliza ICU para manipulação correta de strings e arrays.
 *
 * @param {string} [separator=""] - String usada como separador entre os elementos.
 * @returns {string} String resultante da junção dos elementos do array.
 */
Array.prototype.implode = function(separator = ""){
    return callUserFunc.sync("implode", this, separator);
}

/**
 * @function join
 * @memberof Array.prototype
 * @description
 * Alias para implode. Junta todos os elementos do array em uma única string, usando o separador informado.
 * Mantém compatibilidade com a API nativa do JavaScript, mas utiliza a lógica personalizada do backend para manipulação avançada.
 *
 * @param {string} [separator=""] - String usada como separador entre os elementos.
 * @returns {string} String resultante da junção dos elementos do array.
 */
Array.prototype.join = function(separator = ""){
    return callUserFunc.sync("implode", this, separator);
}

/**
 * @function empty
 * @memberof Array.prototype
 * @description
 * Verifica se o array está vazio.
 * Retorna true se o array não possui elementos, e false caso contrário.
 * Útil para simplificar verificações de arrays vazios em toda a aplicação.
 *
 * @returns {boolean} true se o array estiver vazio, false caso contrário.
 */
Array.prototype.empty = function(){
    return this.length === 0;
}