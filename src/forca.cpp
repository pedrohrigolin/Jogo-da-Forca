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

    // ASCII Art - Título "FORCA"
    std::cout << "\n";
    std::cout << "  ███████╗ ██████╗ ██████╗  ██████╗ ███████╗\n";
    std::cout << "  ██╔════╝██╔═══██╗██╔══██╗██╔════╝ ██╔══██╗\n";
    std::cout << "  █████╗  ██║   ██║██████╔╝██║      ███████║\n";
    std::cout << "  ██╔══╝  ██║   ██║██╔══██╗██║      ██╔══██║\n";
    std::cout << "  ██║     ╚██████╔╝██║  ██║╚██████╗ ██║  ██║\n";
    std::cout << "  ╚═╝      ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝\n";
    std::cout << "\n";
    
    // Subtítulo decorativo
    std::cout << "         ═══════════════════════════════════\n";
    std::cout << "              🎮 JOGO DA FORCA EM C++ 🎮\n";
    std::cout << "         ═══════════════════════════════════\n";
    std::cout << "\n";

        // ASCII Art - Título "FORCA" estilo DOOM
    std::cout << "\n";
    std::cout << "  ████████  ██████  ██████   ██████  ████████\n";
    std::cout << "  ██       ██    ██ ██   ██ ██       ██    ██\n";
    std::cout << "  ██████   ██    ██ ██████  ██       ████████\n";
    std::cout << "  ██       ██    ██ ██   ██ ██       ██    ██\n";
    std::cout << "  ██        ██████  ██   ██  ██████  ██    ██\n";
    std::cout << "\n";
    std::cout << "   ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n";
    std::cout << "  ██                                        ██\n";
    std::cout << "  ██         >>> JOGO DA FORCA <<<          ██\n";
    std::cout << "  ██              [ C++ EDITION ]           ██\n";
    std::cout << "  ██                                        ██\n";
    std::cout << "   ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n";
    std::cout << "\n";

    // ASCII Art - Título "FORCA" (4 linhas)
    std::cout << "\n";
    std::cout << "  ███████  ██████  ██████   ██████  ██████ \n";
    std::cout << "  ██       ██  ██  ██   ██  ██      ██  ██ \n";
    std::cout << "  █████    ██  ██  ██████   ██      ██████ \n";
    std::cout << "  ██       ██████  ██   ██  ██████  ██  ██ \n";
    std::cout << "         ════════ 🎮 JOGO DA FORCA 🎮 ════════\n";
    std::cout << "\n";

    return 0;

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
    endwin(); */

    std::string texto = "Email: lucas123@example.com e jose@test.com";
    
    // Teste 1 — preg_match
    std::string padrao1 = "/\\w+@\\w+\\.com/";
    forcaRegex::RegexResult r1 = forcaRegex::preg_match(padrao1, texto, 0);

    std::cout << "\n[Teste 1: preg_match]\n";
    if (r1.match) {
        for (int key : r1.int_keys) {
            std::vector<forcaRegex::RegexStructData>* grupos = r1.get(key);
            for (const auto& g : *grupos) {
                std::cout << "[" << key << "] " << g.match << " (start: " << g.start << ", end: " << g.end << ")\n";
            }
        }
    } else {
        std::cout << "Nenhum match encontrado.\n";
    }

    // Teste 2 — preg_match_all
    std::string padrao2 = "/\\w+@\\w+\\.com/";
    forcaRegex::RegexResult r2 = forcaRegex::preg_match_all(padrao2, texto, 0);

    std::cout << "\n[Teste 2: preg_match_all]\n";
    if (r2.match) {
        for (int key : r2.int_keys) {
            std::vector<forcaRegex::RegexStructData>* grupos = r2.get(key);
            for (const auto& g : *grupos) {
                std::cout << "[" << key << "] " << g.match << " (start: " << g.start << ", end: " << g.end << ")\n";
            }
        }
    } else {
        std::cout << "Nenhum match encontrado.\n";
    }

    // Teste 3 — grupos nomeados
    std::string texto3 = "Usuário: lucas, ID: 42";
    std::string padrao3 = "/Usuário:\\s*(?<nome>\\w+),\\s*ID:\\s*(?<id>\\d+)/";

    forcaRegex::RegexResult r3 = forcaRegex::preg_match(padrao3, texto3, 0);

    std::cout << "\n[Teste 3: grupos nomeados]\n";
    if (r3.match) {
        for (const std::string& key : r3.string_keys) {
            std::vector<forcaRegex::RegexStructData>* grupos = r3.get(key);
            for (const auto& g : *grupos) {
                std::cout << "$" << key << ": " << g.match << " (start: " << g.start << ", end: " << g.end << ")\n";
            }
        }
    } else {
        std::cout << "Nenhum match encontrado.\n";
    }

    std::cout << "[Teste 1: substituição simples]" << std::endl;
    std::cout << forcaRegex::preg_replace("/foo/", "foo bar", "baz", 0) << std::endl;
    // Esperado: baz bar

    std::cout << "[Teste 2: case-insensitive (i)]" << std::endl;
    std::cout << forcaRegex::preg_replace("/FOO/i", "foo bar", "baz", 0) << std::endl;
    // Esperado: baz bar

    std::cout << "[Teste 3: nenhum casamento]" << std::endl;
    std::cout << forcaRegex::preg_replace("/xyz/", "foo bar", "baz", 0) << std::endl;
    // Esperado: foo bar

    std::cout << "[Teste 4: grupo capturado]" << std::endl;
    std::cout << forcaRegex::preg_replace("/(foo)/", "foo bar", "[$1]", 0) << std::endl;
    // Esperado: [foo] bar

    std::cout << "[Teste 5: múltiplas ocorrências]" << std::endl;
    std::cout << forcaRegex::preg_replace("/a/", "banana", "o", 0) << std::endl;
    // Esperado: bonono

    std::cout << "[Teste 6: offset > 0]" << std::endl;
    std::cout << forcaRegex::preg_replace("/a/", "banana", "o", 5) << std::endl;
    // Esperado: banano  (somente o 'a' no final é substituído)

    std::cout << "[Teste 7: usando \\d]" << std::endl;
    std::cout << forcaRegex::preg_replace("/\\d+/", "User42 has 7 apples", "#", 0) << std::endl;
    // Esperado: User# has # apples

    std::string result;

    // Teste 1
    result = forcaRegex::preg_replace("/(?P<word>\\w+)/", "Hello world", "[$word]", 0);
    std::cout << "Teste 1: " << result << std::endl; // Esperado: [Hello] [world]

    // Teste 2
    result = forcaRegex::preg_replace("/(?P<first>\\w+)\\s(?P<second>\\w+)/", "alpha beta", "$second $first", 0);
    std::cout << "Teste 2: " << result << std::endl; // Esperado: beta alpha

    // Teste 3
    result = forcaRegex::preg_replace("/(?P<num>\\d+)/", "Order 123, code 456", "#$num#", 0);
    std::cout << "Teste 3: " << result << std::endl; // Esperado: Order #123#, code #456#

    // Teste 4
    result = forcaRegex::preg_replace("/\\[(?P<tag>[a-z]+)\\](?P<content>.*?)\\[\\/(?P=tag)\\]/", "[b]bold[/b] and [i]italic[/i]", "<$tag>$content</$tag>", 0);
    std::cout << "Teste 4: " << result << std::endl; // Esperado: <b>bold</b> and <i>italic</i>

    return 0;
}  