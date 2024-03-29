# cmake -S ./ -B ./build -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++

cmake_minimum_required(VERSION 3.28.3)
project(clicker)

set(SRC_DIR ./src)
set( SRC 
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/io_manager.cpp
    ${SRC_DIR}/instruction.cpp
    ${SRC_DIR}/print.cpp
)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE ./includes)