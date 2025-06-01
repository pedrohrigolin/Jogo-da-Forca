cmake_minimum_required(VERSION 3.10)
project(JogoDaForca)

set(CMAKE_CXX_STANDARD 17)
file(GLOB SOURCES "src/*.cpp")
add_executable(JogoDaForca ${SOURCES})

# Inclui os headers comuns a todos
target_include_directories(JogoDaForca PRIVATE
    ${CMAKE_SOURCE_DIR}/include                  # Headers do projeto
    ${CMAKE_SOURCE_DIR}/libs/pcre2/include       # Headers do PCRE2
)

# Inclui headers do PDCurses só no Windows
if(WIN32)
    target_include_directories(JogoDaForca PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/pdcurses/include
    )
endif()

target_compile_definitions(JogoDaForca PRIVATE PCRE2_STATIC)

set_target_properties(JogoDaForca PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

if(WIN32)
    target_link_libraries(JogoDaForca
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-8.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-16.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-32.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/win/libpcre2-posix.a"
        "${CMAKE_SOURCE_DIR}/libs/pdcurses/win/pdcurses.a"
        Ws2_32
        Advapi32
        Userenv
    )
else()
    target_link_libraries(JogoDaForca
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-8.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-16.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-32.a"
        "${CMAKE_SOURCE_DIR}/libs/pcre2/unix/libpcre2-posix.a"
        pthread
        dl
        # Sem curses no Linux ainda
    )
endif()