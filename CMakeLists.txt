cmake_minimum_required(VERSION 3.10)
project(assembler)

set(CMAKE_CXX_STANDARD 17)


find_package(Boost 1.65.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()


add_executable(assembler main.cpp assembler.cpp assembler.hpp disassembler.hpp disassembler.cpp opcodes.hpp opcodes.cpp)