cmake_minimum_required(VERSION 3.10)
project(JogoDaForca)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(jogo-da-forca ${SOURCES})

# Define saída no diretório bin/
set_target_properties(jogo-da-forca PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
