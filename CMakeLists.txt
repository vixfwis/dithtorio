cmake_minimum_required(VERSION 3.10)
project(dithtorio VERSION 1.0.1 DESCRIPTION "Image to Factorio blueprint library")
set(CMAKE_CXX_STANDARD 11)
add_executable(dithtorio dithtorio.cpp dithtorio.h compression.h blueprint.cpp debug.h)

find_package(OpenCV REQUIRED)
target_link_libraries(dithtorio ${OpenCV_LIBS})

find_library(ZLIB_LIBRARY z
        HINTS
        /usr/lib/
        /usr/local/lib
        )
target_link_libraries(dithtorio ${ZLIB_LIBRARY})