/**
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
Array.prototype.implode = function(separator = ""){
    return callUserFunc.sync("implode", this, separator);
}

Array.prototype.join = function(separator = ""){
    return callUserFunc.sync("implode", this, separator);
}