cmake_minimum_required(VERSION 3.6)

project(SDLgame)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

set(SOURCE_FILES src/main.cpp)
add_executable(SDLgame ${SOURCE_FILES})
set_property(TARGET SDLgame PROPERTY CXX_STANDARD 11)

# See /cmake/README.md for SDL setup reference
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_library(TinyXML STATIC lib/tinyxml/tinyxml2.cpp lib/tinyxml/tinyxml2.h)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_NET_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR})
target_link_libraries(SDLgame
        TinyXML
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_NET_LIBRARY}
        ${SDL2_MIXER_LIBRARY})