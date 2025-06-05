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

// int main(int argc, char* argv[]){

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
   
/*     // Inicializa o modo curses
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
 */
    /* 
    |=================================
    |   TESTE PARA REGEX COM PCRE2
    |=================================
    */

/*     const char patternString[] = "hello";
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
}  */

#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define MAX_ITEMS 15 // Total number of items for demonstration

// Function to print a message in the center of a window
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main() {
    ITEM **my_items;
    int c;
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;
    char item_str[30]; // Buffer for item names

    // Initialize ncurses
    initscr();
    start_color(); // Enable color
    cbreak();      // Line buffering disabled, Pass on ever char
    noecho();      // Don't echo() while we do getch
    keypad(stdscr, TRUE); // Enable Fx keys, arrows etc.

    // Initialize color pairs
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLUE); // For menu items
    init_pair(5, COLOR_BLACK, COLOR_WHITE); // For selected menu item

    // Create items
    n_choices = MAX_ITEMS;
    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *)); // +1 for NULL terminator

    for (i = 0; i < n_choices; ++i) {
        sprintf(item_str, "Item %d", i + 1);
        my_items[i] = new_item(strdup(item_str), strdup("Desc.")); // Name and description
    }
    my_items[n_choices] = (ITEM *)NULL; // Last item must be NULL

    // Create menu
    my_menu = new_menu((ITEM **)my_items);

    // Create the window to be associated with the menu
    // Parameters: height, width, starty, startx
    my_menu_win = newwin(10, 40, 4, (COLS - 40) / 2); // Centered window
    keypad(my_menu_win, TRUE);

    // Set main window and sub window
    set_menu_win(my_menu, my_menu_win);
    // Parameters for derwin: height, width, relative_starty, relative_startx
    set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1)); // Subwindow for items

    // Set menu mark to the string " * "
    set_menu_mark(my_menu, " * ");

    // Set menu foreground and background colors
    set_menu_fore(my_menu, COLOR_PAIR(4) | A_REVERSE); // Attributes for selected item
    set_menu_back(my_menu, COLOR_PAIR(5));            // Attributes for non-selected items
    set_menu_grey(my_menu, COLOR_PAIR(3));            // Attributes for non-selectable items (not used here)


    // Print a border around the main window and print a title
    box(my_menu_win, 0, 0);
    print_in_middle(my_menu_win, 1, 0, 40, "Menu Vertical Simples", COLOR_PAIR(1));
    mvwaddch(my_menu_win, 2, 0, ACS_LTEE); // Left T junction for line
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38); // Horizontal line
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE); // Right T junction for line

    mvprintw(LINES - 3, 0, "Use as setas Cima/Baixo para navegar.");
    mvprintw(LINES - 2, 0, "Pressione Enter para selecionar ou F1 para Sair.");
    refresh();

    // Post the menu
    post_menu(my_menu);
    wrefresh(my_menu_win);

    ITEM *selected_item_ptr;
    const char *selected_item_name;

    // Event loop
    while ((c = wgetch(my_menu_win)) != KEY_F(1)) { // F1 to exit
        switch (c) {
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case KEY_NPAGE: // Page Down
                 menu_driver(my_menu, REQ_SCR_DPAGE);
                 break;
            case KEY_PPAGE: // Page Up
                 menu_driver(my_menu, REQ_SCR_UPAGE);
                 break;
            case 10: // Enter key
                selected_item_ptr = current_item(my_menu);
                selected_item_name = item_name(selected_item_ptr);
                
                // Clear previous message
                move(LINES - 1, 0);
                clrtoeol();
                mvprintw(LINES - 1, 0, "Voce selecionou: %s", selected_item_name);
                refresh();
                
                // Optional: Unpost and re-post if you want the selection message to be more prominent
                // or if the menu needs to close after selection in a real app
                // unpost_menu(my_menu);
                // free_menu(my_menu);
                // for(i = 0; i < n_choices; ++i) free_item(my_items[i]);
                // endwin();
                // printf("Selecionado: %s\n", selected_item_name);
                // return 0;
                break;
        }
        wrefresh(my_menu_win); // Refresh menu window after action
    }

    // Unpost and free all the memory taken up
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (i = 0; i < n_choices; ++i) {
        free((char*)item_name(my_items[i])); // Free duplicated strings
        free((char*)item_description(my_items[i]));
        free_item(my_items[i]);
    }
    free(my_items); // Free the array of ITEM pointers
    delwin(my_menu_win);
    endwin(); // End curses mode
    return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) {
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x); // Get current cursor position (not used here, but good practice)
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh(); // Refresh the specific window or stdscr if win is stdscr
}
