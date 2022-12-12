cmake_minimum_required(VERSION 3.23)
project(SYSSecondLab)

set(CMAKE_CXX_STANDARD 14)
include_directories( "${CMAKE_SOURCE_DIR}" )
add_executable(SYSSecondLab main.cpp pipe.h signals.h check.h helper.h signals.cpp pipe.cpp helper.cpp)

find_library(LIBRT rt REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt")
target_link_libraries(SYSSecondLab ${LIBRT})
