cmake_minimum_required(VERSION 3.10)
project(JogoDaForca)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cpp")
add_executable(JogoDaForca ${SOURCES})

# Output do executável
set_target_properties(JogoDaForca PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Diretórios de includes comuns
target_include_directories(JogoDaForca PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Define uso de PCRE2 estática
target_compile_definitions(JogoDaForca PRIVATE PCRE2_STATIC)

# Compilação e link de dependências por plataforma
if(WIN32)

    # Define as opções da PCRE2 antes de add_subdirectory
    set(PCRE2_BUILD_PCRE2_8 ON CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_PCRE2_16 ON CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_PCRE2_32 ON CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_PCRE2POSIX ON CACHE BOOL "" FORCE)
    set(PCRE2_SUPPORT_JIT ON CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_PCRE2GREP OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    # Adiciona a pasta do código-fonte do PCRE2 como subdiretório
    add_subdirectory(${CMAKE_SOURCE_DIR}/libs/pcre2/win ${CMAKE_SOURCE_DIR}/libs/pcre2/win/build)

    # Inclui headers específicos do Windows
    target_include_directories(JogoDaForca PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/pdcurses/include
    )

    # Linka bibliotecas PCRE2 (agora compiladas no momento do build)
    target_link_libraries(JogoDaForca PRIVATE
        pcre2-8
        pcre2-16
        pcre2-32
        pcre2-posix
        ${CMAKE_SOURCE_DIR}/libs/pdcurses/pdcurses.a
        Ws2_32
        Advapi32
        Userenv
    )

else() # Linux

    # Inclui headers e bibliotecas estáticas compiladas previamente
    target_include_directories(JogoDaForca PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/ncurses/include
        ${CMAKE_SOURCE_DIR}/libs/pcre2/include
    )

    target_link_libraries(JogoDaForca PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-8.a
        ${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-16.a
        ${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-32.a
        ${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-posix.a
        ${CMAKE_SOURCE_DIR}/libs/ncurses/libformw.a
        ${CMAKE_SOURCE_DIR}/libs/ncurses/libmenuw.a
        ${CMAKE_SOURCE_DIR}/libs/ncurses/libpanelw.a
        ${CMAKE_SOURCE_DIR}/libs/ncurses/libncursesw.a
        ${CMAKE_SOURCE_DIR}/libs/ncurses/libncurses++w.a
        pthread
        dl
    )

endif()