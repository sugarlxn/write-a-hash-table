cmake_minimum_required(VERSION 3.5)
project(HashTable)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加调试参数
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 添加库
add_library(hash_table STATIC hash_table.c prime.c)
# 添加可执行文件
add_executable(main main.c)
#链接库 链接数学库 m 以及hash_table library
target_link_libraries(main PRIVATE hash_table m)

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

