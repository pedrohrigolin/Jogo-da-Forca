cmake_minimum_required(VERSION 3.20)

# --- Definição do Projeto ---
project(JogoDaForca LANGUAGES CXX C)

# CORREÇÃO RUNTIME WINDOWS: Força a linkagem estática do runtime C++,
# correspondendo ao triplet 'x64-windows-static' do vcpkg.
# Isso deve ser definido ANTES do comando project().
# Esta é a forma mais robusta de fazer isso.
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Define as macros de Unicode
add_compile_definitions(UNICODE _UNICODE)

# Define os padrões C++ e C
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# --- Encontrando Dependências ---
find_package(pcre2 CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n io data)

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

    # --- Configuração para Windows ---
    set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cef/win")

    # No Windows, o FindCEF.cmake cuida da maioria das dependências do sistema
    include(${CEF_ROOT}/cmake/FindCEF.cmake)

    # Compila a libcef_dll_wrapper como uma biblioteca ESTÁTICA
    file(GLOB_RECURSE CEF_WRAPPER_SOURCES "${CEF_ROOT}/libcef_dll/*.cc")
    
    add_library(cef_wrapper STATIC ${CEF_WRAPPER_SOURCES})

    target_include_directories(cef_wrapper PRIVATE ${CEF_ROOT})

    # Definições necessárias para compilar o wrapper no Windows
    target_compile_definitions(cef_wrapper PRIVATE 
        "WRAPPING_CEF_SHARED" "WIN32" "_WINDOWS" "UNICODE" "_UNICODE" "NOMINMAX"
    )

    # Adiciona a flag de exceção /EHsc
    target_compile_options(cef_wrapper PRIVATE /EHsc)

endif()

# --- Estrutura do Projeto ---

file(GLOB SOURCES_CPP "src/*.cpp")
file(GLOB SOURCES_C "src/*.c")

if(UNIX AND NOT APPLE)

    add_executable(JogoDaForca ${SOURCES_CPP} ${SOURCES_C})

else()

    file(GLOB RESOURCE "resources/resource.rc")
    add_executable(JogoDaForca ${SOURCES_CPP} ${SOURCES_C} ${RESOURCE})

endif()

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

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:JogoDaForca>/resources"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/files"
            "$<TARGET_FILE_DIR:JogoDaForca>/files"

        COMMENT "Copiando recursos do CEF para o diretório bin/"

    )

elseif(WIN32)

    add_custom_command(TARGET JogoDaForca POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${CEF_ROOT}/Release" 
            "$<TARGET_FILE_DIR:JogoDaForca>"

        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${CEF_ROOT}/Resources" 
            "$<TARGET_FILE_DIR:JogoDaForca>"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:JogoDaForca>/resources"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/files"
            "$<TARGET_FILE_DIR:JogoDaForca>/files"

        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon/forca_icon.ico"
            "$<TARGET_FILE_DIR:JogoDaForca>"

        COMMENT "Copiando recursos do CEF e ícone da aplicação para o diretório bin/"
    )

endif()

set_target_properties(JogoDaForca PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

if(UNIX AND NOT APPLE)

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

elseif(WIN32)

    target_include_directories(JogoDaForca PRIVATE
        "${CEF_INCLUDE_DIRS}" "${CEF_ROOT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )

    # CORREÇÃO WINDOWS: Adiciona as flags necessárias também ao executável
    target_compile_definitions(JogoDaForca PRIVATE NOMINMAX)
    target_compile_options(JogoDaForca PRIVATE /EHsc)

endif()

# --- Linkagem das Bibliotecas ---

# VERSÃO FINAL COM GRUPOS DE LINKAGEM
if(UNIX AND NOT APPLE)

    target_link_libraries(JogoDaForca PRIVATE

        # Dependências não cíclicas
        PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX
        cryptopp::cryptopp
        nlohmann_json::nlohmann_json
        ICU::uc ICU::i18n ICU::io ICU::data

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

elseif(WIN32)

    target_link_libraries(JogoDaForca PRIVATE
    
        PCRE2::8BIT PCRE2::16BIT PCRE2::32BIT PCRE2::POSIX
        cryptopp::cryptopp
        nlohmann_json::nlohmann_json
        ICU::uc ICU::i18n ICU::io ICU::data

        cef_wrapper
        "${CEF_ROOT}/Release/libcef.lib" 
        ${CEF_LIBRARIES} # No Windows, esta variável já inclui libcef.lib e as libs de sistema (user32, gdi32, etc)

    )

endif()