cmake_minimum_required(VERSION 4.0)
project(MyUtils C)

set(CMAKE_C_STANDARD 11)
if(COMPILE_SHARED)
    add_library(MyUtils SHARED)
else()
    add_library(MyUtils STATIC)
endif()
target_compile_features(MyUtils PRIVATE c_std_11)
target_sources(
        MyUtils PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES include/MyUtils/error.h include/MyUtils/list.h include/MyUtils/macro_magics.h
)
target_sources(
        MyUtils PRIVATE
        src/error.c
        src/list.c
)

add_executable(Test main.c)
target_link_libraries(Test PRIVATE MyUtils)


