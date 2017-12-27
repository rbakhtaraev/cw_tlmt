cmake_minimum_required(VERSION 3.5)
project(tlmt)

set(CMAKE_C_STANDARD 11)

find_package(FLEX)
find_package(BISON)
include_directories(.)

BISON_TARGET(MyParser bison.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(MyScanner flex.lex  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(tlmt
        ${FLEX_MyScanner_OUTPUTS} ./hash.c
        ${BISON_MyParser_OUTPUTS} ./translation.c
        )
#execute_process(COMMAND ../post_build.sh)
#project(asm)
#enable_language(ASM_NASM)
#set(CMAKE_C_COMPILER /usr/bin/gcc)
#set(CMAKE_ASM_NASM_FLAGS "${CMAKE_ASM_NASM_FLAGS} -f elf")
