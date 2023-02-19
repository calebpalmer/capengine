cmake_minimum_required(VERSION 3.9)
project(capengine_tests)

find_package(cppunit)

add_executable(runtests test.cpp)

target_link_libraries(runtests
  PRIVATE
  capengine
  cppunit::cppunit
  )

file(COPY testdata/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../bin/testdata)
