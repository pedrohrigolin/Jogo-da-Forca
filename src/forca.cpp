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

// Lib para regex. PCRE2 É O MESMO MOTOR REGEX USADO NO PHP.
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
    #include <curses.h>
#else
    #include <ncurses.h>
    #include <form.h>
    #include <menu.h>
    #include <panel.h>
#endif

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

int main(int argc, char* argv[]){

    forcaInitialize::init();

    std::cout<<forcaCore::words.levels.easy[12]<<std::endl;

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

    /* 
    |=================================
    |   TESTE PARA REGEX COM PCRE2
    |=================================
    */

        // Regex pattern
    PCRE2_SPTR pattern = (PCRE2_SPTR)"hello";

    // String para buscar
    PCRE2_SPTR subject = (PCRE2_SPTR)"hello world";

    int errornumber;
    PCRE2_SIZE erroroffset;

    // Compilar regex
    pcre2_code *re = pcre2_compile(
        pattern,               // padrão
        PCRE2_ZERO_TERMINATED, // comprimento da string
        0,                     // flags
        &errornumber,          // para código de erro
        &erroroffset,          // posição do erro
        NULL                   // contexto de compilação
    );

    if (re == NULL) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        std::cerr << "Erro ao compilar regex: " << buffer << " na posição " << erroroffset << std::endl;
        return 1;
    }

    // Criar o "match data" necessário para busca
    pcre2_match_data *match_data = pcre2_match_data_create_from_pattern(re, NULL);

    // Tentar casar
    int rc = pcre2_match(
        re,                   // código regex
        subject,              // string alvo
        PCRE2_ZERO_TERMINATED, // tamanho da string
        0,                    // início da busca
        0,                    // flags
        match_data,           // estrutura pra resultado
        NULL                  // contexto
    );

    if (rc < 0) {
        std::cout << "No match found." << std::endl;
    } else {
        std::cout << "Match found!" << std::endl;
    }

    // Limpar alocação
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    /* 
    |====================================
    |   TESTE PARA O PDCURSES/NCURSES
    |====================================
    */

/*         // Inicializa o modo curses
    initscr();              // Inicia a tela
    cbreak();               // Desativa o buffer de linha (recebe input imediatamente)
    noecho();               // Não mostra os caracteres digitados
    keypad(stdscr, TRUE);   // Habilita teclas especiais (setas, etc.)

    // Mensagem inicial
    mvprintw(5, 10, "Pressione qualquer tecla para continuar...");
    refresh();              // Atualiza a tela

    getch();                // Espera por uma tecla

    // Limpa e imprime outra mensagem
    clear();
    mvprintw(5, 10, "Funcionando! Pressione outra tecla para sair.");
    refresh();
    getch();

    // Finaliza o modo curses
    endwin(); */

    return 0;
}