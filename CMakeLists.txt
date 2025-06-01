cmake_minimum_required(VERSION 3.10)
project(JogoDaForca)

set(CMAKE_CXX_STANDARD 17)

# Inclui headers PCRE2
include_directories(${CMAKE_SOURCE_DIR}/libs/pcre2/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(JogoDaForca ${SOURCES})

target_compile_definitions(JogoDaForca PRIVATE PCRE2_STATIC)

# Define saída no diretório bin/
set_target_properties(JogoDaForca PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

if (WIN32)
    # Windows com MinGW: linka libs estáticas PCRE2
    target_link_libraries(JogoDaForca
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-8.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-16.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-32.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-posix.a"
    )
else()
    # Unix (Linux/Mac), ajusta caminhos conforme seu setup
    target_link_libraries(JogoDaForca
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-8.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-16.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-32.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-posix.a"
        pthread
    )
endif()