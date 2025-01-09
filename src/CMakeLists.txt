cmake_minimum_required(VERSION 3.5)
project(HashTable)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加库
add_library(hash_table STATIC hash_table.c)
# 添加可执行文件
add_executable(main main.c hash_table.c)
#链接库
target_link_libraries(main PRIVATE hash_table)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

