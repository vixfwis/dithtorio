cmake_minimum_required(VERSION 3.10)
project(Dithering)
set(CMAKE_CXX_STANDARD 11)
add_executable(dithering dithering.cpp dithering.h base64.h compression.h blueprint.cpp blueprint.h)

find_package(OpenCV REQUIRED)
target_link_libraries(dithering ${OpenCV_LIBS})

include_directories()

find_library(ZLIB_LIBRARY z
        HINTS
        /usr/lib/
        /usr/local/lib
        )
target_link_libraries(dithering ${ZLIB_LIBRARY})