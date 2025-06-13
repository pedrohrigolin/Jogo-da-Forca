cmake_minimum_required(VERSION 3.20)

# --- Definição do Projeto ---
project(JogoDaForca LANGUAGES CXX C)

# Define os padrões C++ e C
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- Encontrando Dependências ---
find_package(pcre2 CONFIG REQUIRED)

# --- Configuração do Chromium Embedded Framework (CEF) ---
if(UNIX AND NOT APPLE)
    # --- Configuração para Linux ---
    set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cef/linux") 

    # 1. Encontra dependências de sistema para o CEF
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GTK3 REQUIRED gtk+-3.0) 
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)

    # 2. Executa o script do CEF para encontrar a biblioteca principal
    include(${CEF_ROOT}/cmake/FindCEF.cmake)

    # 3. Compila a libcef_dll_wrapper como uma biblioteca ESTÁTICA
    file(GLOB_RECURSE CEF_WRAPPER_SOURCES "${CEF_ROOT}/libcef_dll/*.cc")
    
    add_library(cef_wrapper STATIC ${CEF_WRAPPER_SOURCES})

    target_include_directories(cef_wrapper PRIVATE ${CEF_ROOT})
    
    target_compile_definitions(cef_wrapper PRIVATE "WRAPPING_CEF_SHARED")
    
elseif(WIN32)
    # --- Configuração para Windows (A ser adicionada depois) ---
    message(STATUS "Configuração do CEF para Windows ainda não implementada.")
endif()

# --- Estrutura do Projeto ---
file(GLOB SOURCES_CPP "src/*.cpp")
file(GLOB SOURCES_C "src/*.c")

add_executable(JogoDaForca ${SOURCES_CPP} ${SOURCES_C})

# CORREÇÃO FINAL: GARANTE QUE O WRAPPER SEJA COMPILADO ANTES DO EXECUTÁVEL
add_dependencies(JogoDaForca cef_wrapper)

# CORREÇÃO DE EXECUÇÃO: Copia os recursos do CEF para a pasta de saída
if(UNIX AND NOT APPLE)
    add_custom_command(TARGET JogoDaForca POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CEF_ROOT}/Release"
                "$<TARGET_FILE_DIR:JogoDaForca>"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CEF_ROOT}/Resources"
                "$<TARGET_FILE_DIR:JogoDaForca>"
        COMMENT "Copiando recursos do CEF para o diretório bin/"
    )
endif()

set_target_properties(JogoDaForca PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

# --- Configuração de Includes ---
target_include_directories(JogoDaForca PRIVATE
    # Prioridade mais alta para includes do sistema/dependências
    "${GTK3_INCLUDE_DIRS}"
    "${CEF_INCLUDE_DIRS}"
    "${CEF_ROOT}"
    # Includes do nosso projeto
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Adiciona as flags de compilação do GTK (essencial)
target_compile_options(JogoDaForca PRIVATE "${GTK3_CFLAGS_OTHER}")

# --- Linkagem das Bibliotecas ---

# VERSÃO FINAL COM GRUPOS DE LINKAGEM
target_link_libraries(JogoDaForca PRIVATE
    # Dependências não cíclicas
    PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX

    # Inicia o grupo de linkagem para resolver dependências cíclicas
    -Wl,--start-group
    
    # Bibliotecas que dependem uma da outra
    "${CEF_ROOT}/Release/libcef.so" 
    cef_wrapper

    # Finaliza o grupo de linkagem
    -Wl,--end-group

    # Dependências de sistema (que o CEF usa)
    "${GTK3_LIBRARIES}"
    "${X11_LIBRARIES}"
    Threads::Threads
    dl
    rt
)