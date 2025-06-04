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
#include "forcaRegex.h"

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

    // forcaInitialize::init();

    // std::cout<<forcaRegex::normalizePattern("  /[a-zA-Z].+?[0-9]/imu ")<<std::endl;

    // std::cout<<"B"<<forcaStrings::rtrim("  AAAA   ")<<"B"<<std::endl;

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
    |====================================
    |   TESTE PARA O PDCURSES/NCURSES
    |====================================
    */
   
    // Inicializa o modo curses
    initscr();              // Inicia a tela
    cbreak();               // Desativa o buffer de linha (recebe input imediatamente)
    noecho();               // Não mostra os caracteres digitados
    keypad(stdscr, TRUE);   // Habilita teclas especiais (setas, etc.)

    mvprintw(0, 0, "Iniciando o jogo da forca...\n\n");

    if( forcaInitialize::init() ){

        mvprintw(2, 0, "Inicializacao concluida!\n\n");

        forcaUtils::clear_screen();

    }
    else{
        mvprintw(2, 0, "Erro ao iniciar o jogo da forca!\n\n");
    }

    // Mensagem inicial
    mvprintw(4, 0, "Pressione qualquer tecla para testar o pdcurses...");
    
    refresh();              // Atualiza a tela

    getch();                // Espera por uma tecla

    mvprintw(6, 0, "Funcionando! Pressione outra tecla para testar o pcre2 (regex)...");

    refresh();

    getch();

    /* 
    |=================================
    |   TESTE PARA REGEX COM PCRE2
    |=================================
    */

    const char patternString[] = "hello";
    const char subjectString[] = "hello world";

    std::string patternStringChar = patternString;
    std::string subjectStringChar = subjectString;

    // Regex pattern
    PCRE2_SPTR pattern = reinterpret_cast<PCRE2_SPTR>( patternStringChar.data() );

    // String para buscar
    PCRE2_SPTR subject = reinterpret_cast<PCRE2_SPTR>( subjectStringChar.data() );

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

    mvprintw(8, 0, "Padrao a se buscar: ");
    mvprintw(8, 21, patternString);

    mvprintw(10, 0, "String onde sera realizada a busca: ");
    mvprintw(10, 37, subjectString);

    if (rc >= 0) {

        mvprintw(12, 0, "Resultado: o padrao regex ");
        mvprintw(12, 27, patternString);
        mvprintw(12, 27 + strlen(patternString), "  foi encontrado em  ");
        mvprintw(12, 27 + strlen(patternString) + 22, subjectString);

    } else {

        mvprintw(12, 0, "Resultado: o padrao regex ");
        mvprintw(12, 27, patternString);
        mvprintw(12, 27 + strlen(patternString), "  NAO foi encontrado em  ");
        mvprintw(12, 27 + strlen(patternString) + 26, subjectString);

    }

    // Limpar alocação
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    mvprintw(14, 0, "Aperte uma tecla para sair...");

    refresh();

    getch();

    // Finaliza o modo curses
    endwin();

    return 0;
}