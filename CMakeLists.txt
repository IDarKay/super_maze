cmake_minimum_required(VERSION 3.20)
project(laby C)



find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")


file(GLOB_RECURSE PROJECT_SOURCES ./src/*.c)
file(GLOB_RECURSE PROJECT_HEADERS ./src/*.h)

set(CMAKE_C_STANDARD 11)

add_executable(laby ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_link_libraries(laby ${SDL2_LIBRARIES})

if(MSVC)
    target_compile_options(laby PRIVATE /W4 /WX)
else()
    target_compile_options(laby PRIVATE -Wall)
endif()