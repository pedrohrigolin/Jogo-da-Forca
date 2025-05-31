/*
|=================================================================
|                       INCLUDES DO PROJETO
|=================================================================
|
| BIBLIOTECAS PADRÃO C++
| ----------------------
| <iostream>  - Operações básicas de I/O
| <cstdlib>   - Funções gerais da biblioteca padrão
| <string>    - Manipulação de strings
| <cctype>    - Classificação e transformação de caracteres
| <fstream>   - Operações de arquivo
| <vector>    - Container dinâmico de elementos
|
| BIBLIOTECAS DO PROJETO
| ---------------------
| "forcaUtils.h" - Utilitários gerais do jogo da forca
|                  Contém funções auxiliares como clear_screen()
|                  e outras utilidades compartilhadas
|=================================================================
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <cctype>
#include <fstream>
#include <vector>
#include <map>
#include <stdexcept>
#include "forcaUtils.h"
#include "forcaStrings.h"
#include "forcaFiles.h"
#include "forcaInitialize.h"
#include "forcaCore.h"

/*
|================================================================
|   FUNÇÕES DE VERIFICAÇÃO DE ARQUIVO, CONTEUDO E INTEGRIDADE
|================================================================
*/




/* 
|==================
|   FUNÇÃO MAIN
|==================
*/

int main(){

    std::cout<<forcaUtils::filter_sanitize_boolean(0)<<std::endl;


    // std::string teste = " AB  C D EFGH IJ K  \n adsa AQUI áááâ üü  da \t adada  dsada \r dsads adadas \v dadagfa fdafa \f dasdsa dsada  ";

    // teste = forcaStrings::normalizeWord(teste);

    // if( forcaStrings::checkAlphaCharacters(teste) ){
    //     forcaUtils::clear_screen();
    //     std::cout<<"Esta correto!"<<std::endl;
    // }
    // else{
    //     forcaUtils::clear_screen();
    //     std::cout<<"Contem caracteres nao permitidos!"<<std::endl;
    // }

    // // std::cout<<teste<<std::endl;

    // std::string filepath = "../files/words/custom/levels/easy.txt";

    // forcaFiles::validate::validateFiles();

    // std::cout<<forcaFiles::utils::isEmpty(filepath)<<std::endl;

    // std::string normalized = normalizePath(filepath);

    // std::cout<<normalized<<std::endl;

    // forcaUtils::clear_screen();

    // std::string content = getContent(filepath);

    // std::cout<<content<<std::endl;

    return 0;
}