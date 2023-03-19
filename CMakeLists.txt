cmake_minimum_required(VERSION 3.22)

project(capengine VERSION 1.0
  DESCRIPTION "A Game Engine"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(GNUInstallDirs)

execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/conan_install.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include(.conan/conan_paths.cmake)

find_package(SDL2)
find_package(SDL2_image)
find_package(SDL2_ttf)
find_package(libxml2)
find_package(libsndfile)
find_package(Boost)

add_subdirectory(extern)
add_subdirectory(src)

add_custom_target(
  doc
  COMMAND doxygen doxygenfile
  )
