#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <vector>

/**
 * Define uma constante de string indicando o sistema operacional em uso.
 * 
 * Se o código estiver sendo compilado em um sistema Windows (_WIN32 ou _WIN64),
 * define __FORCA_OS__ como "WINDOWS". Caso contrário, define como "UNIX".
 * 
 * Isso permite que o restante do código adapte comportamentos específicos
 * de acordo com o sistema operacional detectado em tempo de compilação.
 */
#if defined(_WIN32) || defined(_WIN64)
    const std::string __FORCA_OS__ = "WINDOWS";
#else
    const std::string __FORCA_OS__ = "UNIX";
#endif

/* 
|=========================================
|   FUNÇÕES DE NORMALIZAÇÃO DE STRING
|========================================= 
*/

/**
 * Remove os espaçamentos da string obtida. Isso é util para normalizar a string
 * e evitar erros no funcionamento do código, além de corrigir possíveis espaçamentos
 * não intencionais feitos pelo usuário. Remove espaços em branco, tabs, quebras de linha
 * e outros caracteres de espaçamento.
 *
 * @param   std::string text    String que terá seus espaços removidos
 * @return  std::string         String normalizada sem espaços
 */
std::string removeSpaces( std::string text ){

    std::string::size_type spacePos = text.find_first_of(" \t\n\r\v\f");

    while(spacePos != std::string::npos){

        // Remove o espaço encontrado.
        text.erase(spacePos, 1);
        
        /*          
        |    Busca o proximo espaço a partir da posição do ultimo espaço encontrado.
        |    Isso otimiza a busca pois o find não vai ficar percorrendo a string desde o
        |    começo toda vez que for usado. 
        */
        spacePos = text.find_first_of(" \t\n\r\v\f", spacePos);

    }

    return text;

}

/**
 * Remove acentos e caracteres especiais (ç) de uma string, substituindo-os 
 * por seus equivalentes sem acentuação. Por exemplo, 'á' é substituído por 'a',
 * 'ç' por 'c', etc.
 *
 * @param   std::string text    String que terá seus acentos removidos
 * @return  std::string         String normalizada sem acentos
 */
std::string removeAcentos( std::string text ) {

    int i;
    std::string::size_type pos;
    std::string::size_type stringSize;

    /**
     * Palavras acentuadas e ç. Serão usadas para comparação e depois
     * serão substituidas pela letra normal na função de normalização
     * de strings.
     * 
     * @var	string _REMOVE_ACENTOS_ACENTUADAS_
     */
    static const std::string _REMOVE_ACENTOS_ACENTUADAS_[] = {
        "á", "à", "â", "ã", "ä", "Á", "À", "Â", "Ã", "Ä",
        "é", "è", "ê", "ë", "É", "È", "Ê", "Ë",
        "í", "ì", "î", "ï", "Í", "Ì", "Î", "Ï",
        "ó", "ò", "ô", "õ", "ö", "Ó", "Ò", "Ô", "Õ", "Ö",
        "ú", "ù", "û", "ü", "Ú", "Ù", "Û", "Ü",
        "ç", "Ç"
    };

    /**
     * Palavras normais, sem acentuação e ç. Serão usadas para substituição,
     * na função de normalização de strings.
     * 
     * @var string _REMOVE_ACENTOS_NORMALIZADAS_
     */
    static const std::string _REMOVE_ACENTOS_NORMALIZADAS_[] = {
        "a", "a", "a", "a", "a", "A", "A", "A", "A", "A",
        "e", "e", "e", "e", "E", "E", "E", "E",
        "i", "i", "i", "i", "I", "I", "I", "I",
        "o", "o", "o", "o", "o", "O", "O", "O", "O", "O",
        "u", "u", "u", "u", "U", "U", "U", "U",
        "c", "C"
    };

    /**
     * Cache do tamanho da string constante ACENTUADOS. Evita ficar recalculando
     * desnecessáriamente o tamanho a acada função.
     * 
     * @var	int	_REMOVE_ACENTOS_MAP_SIZE_
     */
    static const int _REMOVE_ACENTOS_MAP_SIZE_ =
        sizeof(_REMOVE_ACENTOS_ACENTUADAS_) / sizeof(_REMOVE_ACENTOS_ACENTUADAS_[0]);

    for(i = 0; i < _REMOVE_ACENTOS_MAP_SIZE_; i++) {

        stringSize = _REMOVE_ACENTOS_ACENTUADAS_[i].size();
        pos = text.find(_REMOVE_ACENTOS_ACENTUADAS_[i]);

        while(pos != std::string::npos) {

            text.replace(pos, stringSize, _REMOVE_ACENTOS_NORMALIZADAS_[i]);
            pos = text.find(_REMOVE_ACENTOS_ACENTUADAS_[i], pos);
        
        }

    }

    return text;

}

/**
 * Converte todos os caracteres de uma string para maiúsculos. A função lida 
 * corretamente com caracteres ASCII estendido (>127) através de um cast para 
 * unsigned char.
 *
 * @param   std::string text    String que será convertida para maiúsculo
 * @return  std::string         String com todos os caracteres em maiúsculo
 */
std::string to_uppercase( std::string text ) {

    std::string::size_type i, length;

    length = text.size();

    for (i = 0; i < length; i++) {

        // Cast para unsigned char para evitar comportamento indefinido em caracteres >127
        text[i] = static_cast<char>( std::toupper(static_cast<unsigned char>( text[i] ) ) );

    }

    return text;

}

/**
 * Normaliza uma string removendo espaços, acentos e convertendo para maiúsculo.
 * Esta função combina as operações de removeSpaces(), removeAcentos() e 
 * to_uppercase() em uma única chamada.
 *
 * @param   std::string text    String que será normalizada
 * @return  std::string         String normalizada sem espaços, acentos e em maiúsculo
 */
std::string normalizeWord( std::string text ){

    text = removeSpaces(text);

    text = removeAcentos(text);

    text = to_uppercase(text);

    return text;

}

/*
|=========================================================
|   FUNÇÕES UTILITÁRIAS PARA A MANIPULAÇÃO DE ARQUIVOS
|=========================================================
*/

/**
 * Normaliza um caminho de arquivo, removendo espaços, convertendo todas as barras invertidas (\)
 * para barras normais (/) e eliminando barras duplicadas. 
 * 
 * Essa função é útil para padronizar caminhos de arquivos, evitando problemas de compatibilidade
 * entre sistemas operacionais e inconsistências causadas por múltiplas barras ou espaços.
 *
 * @param   std::string path    Caminho do arquivo a ser normalizado.
 * @return  std::string         Caminho normalizado, sem espaços, com barras '/' e sem duplicidade.
 */
std::string normalizePath( std::string path ){

    path = removeSpaces(path);

    std::string::size_type pos = path.find("\\");

    // Remove / invertidas, comium no windows mas que pode gerar problemas no codigo.
    while( pos != std::string::npos ){

        path.replace(pos, 1, "/");

        pos = path.find("\\", pos);

    }

    pos = path.find("/");

    // Remove / sequenciais
    while( pos != std::string::npos ){

        if( path[ pos - 1 ] == '/' ) path.erase(pos, 1);
        else pos++;

        pos = path.find("/", pos);

    }

    return path;

}

/**
 * Verifica se um arquivo existe no caminho especificado.
 *
 * O caminho é normalizado antes da verificação para evitar problemas de formatação.
 * A função tenta abrir o arquivo em modo de leitura; se conseguir, considera que o arquivo existe.
 *
 * @param   std::string filepath   Caminho do arquivo a ser verificado.
 * @return  bool                   true se o arquivo existe, false caso contrário.
 */
bool fileExist( std::string filepath ){

    filepath = normalizePath(filepath);

    std::ifstream file(filepath);

    bool exist = file.is_open();

    file.close();

    return exist;

}

/**
 * Verifica se um arquivo pode ser lido no caminho especificado.
 *
 * O caminho é normalizado antes da verificação. A função tenta abrir o arquivo em modo de leitura
 * e verifica se o arquivo está acessível e em bom estado para leitura.
 *
 * @param   std::string filepath   Caminho do arquivo a ser verificado.
 * @return  bool                   true se o arquivo pode ser lido, false caso contrário.
 */
bool canRead( std::string filepath ){

    filepath = normalizePath(filepath);

    std::ifstream file(filepath);

    if( ! file.is_open() ) return false;

    if( ! file.good() ) return false;

    file.close();

    return true;

}

/**
 * Verifica se um arquivo pode ser escrito no caminho especificado.
 *
 * O caminho é normalizado antes da verificação. A função tenta abrir o arquivo em modo de escrita (append)
 * e realiza testes para garantir que é possível escrever no arquivo, verificando permissões e possíveis erros.
 *
 * @param   std::string filepath   Caminho do arquivo a ser verificado.
 * @return  bool                   true se o arquivo pode ser escrito, false caso contrário.
 */
bool canWrite( std::string filepath ){

    filepath = normalizePath(filepath);

    std::ofstream file(filepath, std::ios::app);

    if( ! file.is_open() ) return false;

    // Primeiro verifica com base na posição do ponteiro de escrita
    if( file.tellp() == -1 ) return false;

    // Agora tenta escreve um conteudo vazio mesmo
    file<<"";

    // Verifica se foi possivel escrever
    if( file.fail() ) return false;

    file.close();

    return true;

}

/*
|=====================================
|   FUNÇÕES DE CRIAÇÃO DE ARQUIVOS
|=====================================
*/

/**
 * Cria um arquivo no caminho especificado, com conteúdo opcional.
 *
 * O caminho é normalizado antes da criação. Se o arquivo já existir, verifica permissões de leitura e escrita.
 * Caso não seja possível ler ou escrever, exibe mensagens de erro e retorna false.
 * Se o arquivo não existir ou for possível sobrescrever, cria o arquivo e grava o conteúdo informado.
 *
 * @param   std::string filepath   Caminho do arquivo a ser criado.
 * @param   std::string content    Conteúdo a ser gravado no arquivo (opcional, padrão é string vazia).
 * @return  bool                   true se o arquivo foi criado e escrito com sucesso, false caso contrário.
 */
bool createFile( std::string filepath, std::string content = "" ){

    filepath = normalizePath(filepath);

    bool exist = fileExist(filepath);

    if( exist && ! canRead(filepath) ){

        std::cout<<"O arquivo nao tem permissao de leitura, ou ocorreu um erro ao tentar abrir o arquivo, verifique e tente novamente!"<<std::endl;

        return false;

    }

    if( ! canWrite(filepath) ){

        std::cout<<"O arquivo nao tem permissao de escrita, ou ocorreu um erro ao tentar abrir o arquivo, verifique e tente novamente!"<<std::endl;

        return false;

    }

    std::ofstream newFile;
    
    newFile.open(filepath);

    if( ! newFile.is_open() ) return false;

    newFile<<content;

    if( newFile.fail() ){

        std::cout<<"Erro ao gravar o conteudo no arquivo criado, tente novamente!"<<std::endl;

        newFile.close();

        return false;

    }

    newFile.close();
    
    return true;

}

/* 
|==================
|   FUNÇÃO MAIN
|==================
*/

int main(){

    // std::string teste = "   AB  C D EFGH IJ K  \n adsa AQUI áááâ üü  da \t adada  dsada \r dsads adadas \v dadagfa fdafa \f dasdsa dsada  ";

    // teste = normalizeWord(teste);

    // std::cout<<teste<<std::endl;

    std::string filepath = ".\\\\//////WORDS///////CUSTOM\\\\/////// \\\\  //// LEVELS/HARD.t    x t       ";

    // std::string normalized = normalizePath(filepath);

    // std::cout<<normalized<<std::endl;

    createFile(filepath);

    return 0;
}