cmake_minimum_required(VERSION 3.28)
project(6502)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

add_executable(6502 src/main.cpp
        src/memory.cpp
        src/cpu.cpp
        src/cpu_instructions.cpp
)

target_link_libraries(6502 fmt::fmt)