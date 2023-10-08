cmake_minimum_required(VERSION 3.16)
project(assign02_shared_memory)

set(CMAKE_CXX_COMPILER "clang")
set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(MAINPROC_SOURCE_FILES
        ipcEx.h
        mainProc.c)

set(CHILDPROC_SOURCE_FILES
        ipcEx.h
        childProc.c)

add_executable(mainProc  ${MAINPROC_SOURCE_FILES})
add_executable(childProc ${CHILDPROC_SOURCE_FILES})
