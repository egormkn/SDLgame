cmake_minimum_required(VERSION 3.6)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

include(${PROJECT_SOURCE_DIR}/cmake/LibraryPaths.cmake OPTIONAL)
# You can set custom library paths in file cmake/LibraryPaths.cmake
# For example:
# set(SDL2_DIR "C:\\Programs\\SDL2-2.0.4\\i686-w64-mingw32")
# set(SDL2_IMAGE_DIR "C:\\Programs\\SDL2_image-2.0.1\\i686-w64-mingw32")
# set(SDL2_MIXER_DIR "C:\\Programs\\SDL2_mixer-2.0.1\\i686-w64-mingw32")
# set(SDL2_NET_DIR "C:\\Programs\\SDL2_net-2.0.1\\i686-w64-mingw32")
# set(SDL2_TTF_DIR "C:\\Programs\\SDL2_ttf-2.0.14\\i686-w64-mingw32")

# See /cmake/README.md for library setup reference
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

add_subdirectory(client)
add_subdirectory(server)
