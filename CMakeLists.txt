cmake_minimum_required(VERSION 3.20)
project(laby C)

set(SDL2_TTF_PATH "/path/to/sdl2_ttf" CACHE BOOL "" FORCE)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")


file(GLOB_RECURSE PROJECT_SOURCES ./src/*.c)
file(GLOB_RECURSE PROJECT_HEADERS ./src/*.h)

set(CMAKE_C_STANDARD 11)

add_executable(laby ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
        include ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES})

if(MSVC)
    target_compile_options(laby PRIVATE /W4 /WX)
else()
    target_compile_options(laby PRIVATE -Wall)
endif()