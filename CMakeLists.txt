cmake_minimum_required(VERSION 3.5)
project(EaD)

SET(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

ADD_DEFINITIONS(-g -W -lpthread -Wall -std=c++11 -lrt -lcrypto -lstdc++ -D_GLIBCXX_USE_CXX11_ABI=0 -fpermissive -O3)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

include_directories(./include /usr/include BLAKE2b)
link_libraries("/usr/lib/x86_64-linux-gnu/librt.so")
link_libraries("/usr/lib/x86_64-linux-gnu/libcrypto.a")
link_directories(/lib/x86_64-linux-gnu)

message("${CMAKE_SOURCE_DIR}")

set(SOURCE_FILES EaD.cpp include/bch.h include/bloom_filter.hpp include/com_t.h include/tpdef.h
        src/bch.c src/com_t.cpp src/bloom_func.cpp include/bloom_func.h src/cache.cpp
        include/cache.h src/dedup.cpp include/dedup.h src/mt.cpp include/mt.h src/new_cache.cpp
        include/new_cache.h src/blake2b.c)
add_executable(EaD ${SOURCE_FILES})