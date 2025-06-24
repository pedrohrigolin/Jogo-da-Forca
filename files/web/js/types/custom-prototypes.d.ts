interface Array<T> {
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
  implode(separator?: string): string;

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
  join(separator?: string): string;

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
  empty(): boolean;
}

interface String {
  /**
   * @function empty
   * @memberof String.prototype
   * @description
   * Verifica se a string está vazia.
   * Retorna true se a string não possui caracteres, false caso contrário.
   * 
   * @returns {boolean} true se a string estiver vazia, false caso contrário.
   */
  empty(): boolean;

  /**
   * @function toUpperCase
   * @memberof String.prototype
   * @description
   * Converte todos os caracteres da string para maiúsculo, respeitando Unicode.
   * Utiliza ICU no backend para garantir suporte a acentuação e caracteres especiais.
   * 
   * @returns {string} String convertida para maiúsculo.
   */
  toUpperCase(): string;

  /**
   * @function toLowerCase
   * @memberof String.prototype
   * @description
   * Converte todos os caracteres da string para minúsculo, respeitando Unicode.
   * Utiliza ICU no backend para garantir suporte a acentuação e caracteres especiais.
   * 
   * @returns {string} String convertida para minúsculo.
   */
  toLowerCase(): string;

  /**
   * @function removeSpaces
   * @memberof String.prototype
   * @description
   * Remove todos os espaços em branco da string, incluindo espaços Unicode.
   * Utiliza ICU para identificar e remover todos os tipos de espaçamento.
   * 
   * @returns {string} String sem espaços.
   */
  removeSpaces(): string;

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
  normalize(form?: string): string;

  /**
   * @function removeAcentos
   * @memberof String.prototype
   * @description
   * Remove acentos e o caractere "ç" da string, substituindo por seus equivalentes sem acentuação.
   * Focado em palavras do português brasileiro. Utiliza normalização Unicode no backend.
   * 
   * @returns {string} String sem acentos e sem "ç".
   */
  removeAcentos(): string;

  /**
   * @function trim
   * @memberof String.prototype
   * @description
   * Remove espaços em branco do início e do fim da string, incluindo espaços Unicode.
   * Equivalente ao PHP trim(), utilizando ICU para suporte total a Unicode.
   * 
   * @returns {string} String sem espaços no início e fim.
   */
  trim(): string;

  /**
   * @function rtrim
   * @memberof String.prototype
   * @description
   * Remove espaços em branco do fim da string (right trim), incluindo espaços Unicode.
   * Equivalente ao PHP rtrim(), utilizando ICU para suporte total a Unicode.
   * 
   * @returns {string} String sem espaços no fim.
   */
  rtrim(): string;

  /**
   * @function ltrim
   * @memberof String.prototype
   * @description
   * Remove espaços em branco do início da string (left trim), incluindo espaços Unicode.
   * Equivalente ao PHP ltrim(), utilizando ICU para suporte total a Unicode.
   * 
   * @returns {string} String sem espaços no início.
   */
  ltrim(): string;

  /**
   * @function trimStart
   * @memberof String.prototype
   * @description
   * Alias para rtrim(). Remove espaços em branco do fim da string.
   * 
   * @returns {string} String sem espaços no fim.
   */
  trimStart(): string;

  /**
   * @function trimEnd
   * @memberof String.prototype
   * @description
   * Alias para ltrim(). Remove espaços em branco do início da string.
   * 
   * @returns {string} String sem espaços no início.
   */
  trimEnd(): string;

  /**
   * @function normalizeWord
   * @memberof String.prototype
   * @description
   * Normaliza uma palavra removendo espaços, acentos e convertendo para maiúsculo.
   * Combina removeSpaces(), removeAcentos() e toUpperCase() em uma única chamada.
   * 
   * @returns {string} Palavra normalizada.
   */
  normalizeWord(): string;

  /**
   * @function normalizeLineBreaks
   * @memberof String.prototype
   * @description
   * Normaliza todas as quebras de linha da string para o formato Unix (\n).
   * Converte \r\n, \r, \v, \f, etc. para \n usando regex no backend.
   * 
   * @returns {string} String com quebras de linha normalizadas.
   */
  normalizeLineBreaks(): string;

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
  removeExtraLineBreaks(normalize?: boolean): string;

  /**
   * @function isAlpha
   * @memberof String.prototype
   * @description
   * Verifica se a string contém apenas caracteres alfabéticos (A-Z, a-z).
   * Retorna true se todos os caracteres são letras, false caso contrário.
   * 
   * @returns {boolean} true se todos os caracteres são letras, false caso contrário.
   */
  isAlpha(): boolean;

  /**
   * @function concat
   * @memberof String.prototype
   * @description
   * Concatena a string original com uma ou mais strings adicionais.
   * 
   * @param {...string} string Strings a serem concatenadas.
   * @returns {string} String resultante da concatenação.
   */
  concat(...string: string[]): string;

  /**
   * @function includes
   * @memberof String.prototype
   * @description
   * Verifica se a string contém a substring informada.
   * 
   * @param {string} string Substring a ser buscada.
   * @returns {boolean} true se a substring for encontrada, false caso contrário.
   */
  includes(string: string): boolean;

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
  indexOf(string: string): number;

  /**
   * @function firstIndexOf
   * @memberof String.prototype
   * @description
   * Retorna o índice da primeira ocorrência da substring informada, considerando caracteres visíveis (Unicode).
   * 
   * @param {string} string Substring a ser buscada.
   * @returns {number} Índice da primeira ocorrência ou -1 se não encontrada.
   */
  firstIndexOf(string: string): number;

  /**
   * @function lastIndexOf
   * @memberof String.prototype
   * @description
   * Retorna o índice da última ocorrência da substring informada, considerando caracteres visíveis (Unicode).
   * 
   * @param {string} string Substring a ser buscada.
   * @returns {number} Índice da última ocorrência ou -1 se não encontrada.
   */
  lastIndexOf(string: string): number;

  /**
   * @function startsWith
   * @memberof String.prototype
   * @description
   * Verifica se a string começa com a substring informada.
   * 
   * @param {string} string Substring a ser verificada.
   * @returns {boolean} true se a string começar com a substring, false caso contrário.
   */
  startsWith(string: string): boolean;

  /**
   * @function endsWith
   * @memberof String.prototype
   * @description
   * Verifica se a string termina com a substring informada.
   * 
   * @param {string} string Substring a ser verificada.
   * @returns {boolean} true se a string terminar com a substring, false caso contrário.
   */
  endsWith(string: string): boolean;

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
  charAt(index: number): string;

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
  repeat(count?: number): string;

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
  slice(start: number, end?: number): string;

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
  substring(start: number, end?: number): string;

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
  substr(start: number, length: number): string;

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
  explode(separator?: string, limit?: number): string[];

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
  split(separator?: string | RegExp, limit?: number): string[];

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
    replace(search: string | RegExp, replaceValue: string, offset?: number): string;

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
    replaceAll(search: string | RegExp, replaceValue: string, offset?: number, limit?: number): string;

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
    preg_replace(search: string, replaceValue: string, offset?: number, limit?: number): string;

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
    preg_split(search: string, limit?: number): string[];

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
    search(search: string | RegExp): number;

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
    searchAll(search: string | RegExp, limit?: number): number[];

    /**
     * @function visibleLength
     * @memberof String.prototype
     * @description
     * Retorna o tamanho da string em caracteres visíveis (graphemes/code points Unicode), e não em bytes.
     * Utiliza ICU no backend para contagem correta de caracteres Unicode.
     * 
     * @returns {number} Quantidade de caracteres visíveis na string.
     */
    visibleLength(): number;
}