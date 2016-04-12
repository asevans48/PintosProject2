cmake_minimum_required(VERSION 3.5)
project(threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    flags.h
    init.c
    init.h
    interrupt.c
    interrupt.h
    intr-stubs.h
    io.h
    loader.h
    malloc.c
    malloc.h
    palloc.c
    palloc.h
    pte.h
    switch.h
    synch.c
    synch.h
    thread.c
    thread.h
    vaddr.h)

add_executable(threads ${SOURCE_FILES})