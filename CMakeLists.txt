cmake_minimum_required(VERSION 3.6)

project(SDLgame)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

set(SOURCE_FILES Move.cpp)
add_executable(SDLgame ${SOURCE_FILES})

# For Windows you should create environment variable "SDL2DIR"
# with path to SDL MinGW folder as a value
# (e.g. C:\Programs\SDL2-2.0.4\i686-w64-mingw32)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
set(INCLUDES ${INCLUDES} ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})

include_directories(${INCLUDES})
target_link_libraries(SDLgame ${LIBS})