cmake_minimum_required(VERSION 3.16)
project(PortScanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(scanner main.cpp asyncScanner.cpp)

if(WIN32)
    target_link_libraries(scanner PRIVATE ws2_32)
else()
    target_compile_options(scanner PRIVATE -Wall -Wextra -O2)
endif()