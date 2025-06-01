cmake_minimum_required(VERSION 3.10)
project(JogoDaForca)

# Define padrão C++17
set(CMAKE_CXX_STANDARD 17)

# Busca todos os arquivos .cpp na pasta src
file(GLOB SOURCES "src/*.cpp")

# Cria o executável com os arquivos fonte encontrados
add_executable(JogoDaForca ${SOURCES})

# Inclui diretórios para headers
target_include_directories(JogoDaForca PRIVATE
    ${CMAKE_SOURCE_DIR}/include                  # Headers do projeto
    ${CMAKE_SOURCE_DIR}/libs/pcre2/include       # Headers do PCRE2
    ${CMAKE_SOURCE_DIR}/libs/pdcurses/include    # Headers do PDCurses
)

# Define macro para indicar uso da biblioteca PCRE2 estática
target_compile_definitions(JogoDaForca PRIVATE PCRE2_STATIC)

# Define o diretório onde o executável será gerado
set_target_properties(JogoDaForca PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Linkagem das libs estáticas e libs do sistema, variando conforme o SO
if(WIN32)
    target_link_libraries(JogoDaForca
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-8.a"       # PCRE2 8-bit
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-16.a"      # PCRE2 16-bit
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-32.a"      # PCRE2 32-bit
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-posix.a"   # PCRE2 POSIX
        "${CMAKE_SOURCE_DIR}/libs/pdcurses/win/pdcurses.a" # PDCurses para Windows
        Ws2_32          # Winsock2 (rede)
        Advapi32        # API de segurança
        Userenv         # Ambiente do usuário
    )
else()
    target_link_libraries(JogoDaForca
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-8.a"       # PCRE2 8-bit
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-16.a"      # PCRE2 16-bit
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-32.a"      # PCRE2 32-bit
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-posix.a"   # PCRE2 POSIX
        "${CMAKE_SOURCE_DIR}/libs/pdcurses/unix/libpdcurses.a"   # PDCurses para Unix/Linux
        pthread         # Thread POSIX
        dl              # Linkagem dinâmica
    )
endif()