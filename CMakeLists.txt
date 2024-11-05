cmake_minimum_required(VERSION 3.20)

project(Primey)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS YES)

set(cpp_files_core
    "src/Primey.cpp"
)

set(h_files_core

)

add_executable(Primey ${cpp_files_core} ${h_files_core})

find_package(Threads)

target_link_libraries(Primey
    ${CMAKE_THREAD_LIBS_INIT}
)
