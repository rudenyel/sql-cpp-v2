cmake_minimum_required(VERSION 3.27)
project(sql_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(sql_cpp
        main.h
        main.cpp
        wrapper.h
        wrapper.cpp
        sqlite3.h
        sqlite3.c)
#target_link_libraries(sql_cpp LINK_PUBLIC sqlite3)
