cmake_minimum_required(VERSION 3.10)

if(NOT WIN32)

# Força o uso do gcc e g++
set(CMAKE_C_COMPILER /usr/bin/gcc CACHE FILEPATH "C compiler override" FORCE)
set(CMAKE_CXX_COMPILER /usr/bin/g++ CACHE FILEPATH "C++ compiler override" FORCE)

endif()

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

    set(PDCURSES_LIB_PATH ${CMAKE_SOURCE_DIR}/libs/pdcurses/wincon/pdcurses.a)

    # Alvo para compilar o PDCurses via Makefile.mgw
    add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/libs/pdcurses/wincon/pdcurses.a
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/pdcurses/wincon
        COMMAND mingw32-make -f Makefile
        COMMENT "Compilando PDCurses com Makefile"
    )

    add_custom_target(build_pdcurses DEPENDS ${CMAKE_SOURCE_DIR}/libs/pdcurses/wincon/pdcurses.a)
    add_dependencies(JogoDaForca build_pdcurses)

    target_link_libraries(JogoDaForca PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/pdcurses/wincon/pdcurses.a
    )

    # Linka bibliotecas PCRE2 (agora compiladas no momento do build)
    target_link_libraries(JogoDaForca PRIVATE
        pcre2-8
        pcre2-16
        pcre2-32
        pcre2-posix
        ${PDCURSES_LIB_PATH}
        Ws2_32
        Advapi32
        Userenv
    )

else() # Linux

    # Define as opções da PCRE2 antes de add_subdirectory
    set(PCRE2_BUILD_PCRE2_8 ON CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_PCRE2_16 ON CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_PCRE2_32 ON CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_PCRE2POSIX ON CACHE BOOL "" FORCE)
    set(PCRE2_SUPPORT_JIT ON CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(PCRE2_BUILD_PCRE2GREP OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    # Adiciona o código-fonte do PCRE2 como subdiretório
    add_subdirectory(${CMAKE_SOURCE_DIR}/libs/pcre2/unix ${CMAKE_SOURCE_DIR}/libs/pcre2/unix/build)

    # Define as opções do ncurses
    set(NCURSES_SRC_DIR ${CMAKE_SOURCE_DIR}/libs/ncurses)
    set(NCURSES_BUILD_DIR ${NCURSES_SRC_DIR}/build)
    set(NCURSES_LIB_DIR ${NCURSES_BUILD_DIR}/lib)
    set(NCURSES_INCLUDE_DIR ${NCURSES_BUILD_DIR}/include/ncursesw)

    # Comando de compilação do ncurses
    add_custom_command(
        OUTPUT ${NCURSES_LIB_DIR}/libncursesw.a
        WORKING_DIRECTORY ${NCURSES_SRC_DIR}
        COMMAND ./configure --prefix=${NCURSES_BUILD_DIR} --with-normal --without-shared --without-debug --enable-widec --enable-static
        COMMAND make -j
        COMMAND make install
        COMMENT "Compilando ncurses estático para Linux"
    )

    add_custom_target(build_ncurses DEPENDS ${NCURSES_LIB_DIR}/libncursesw.a)
    add_dependencies(JogoDaForca build_ncurses)

    # Diretórios de includes comuns
    target_include_directories(JogoDaForca PRIVATE
        ${NCURSES_INCLUDE_DIR}
        ${NCURSES_BUILD_DIR}/include
    )

    # Linkagem de libs estáticas compiladas no build
    target_link_libraries(JogoDaForca PRIVATE

        pcre2-8
        pcre2-16
        pcre2-32
        pcre2-posix

        ${NCURSES_LIB_DIR}/libncurses++w.a
        ${NCURSES_LIB_DIR}/libformw.a
        ${NCURSES_LIB_DIR}/libmenuw.a
        ${NCURSES_LIB_DIR}/libpanelw.a
        ${NCURSES_LIB_DIR}/libncursesw.a

        pthread
        dl
    )

endif()