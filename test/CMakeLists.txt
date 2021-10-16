cmake_minimum_required(VERSION 3.9)
project(capengine_tests)

add_executable(runtests test.cpp)

target_link_libraries(runtests
	    capengine
	    ${CONAN_LIBS}
	    stdc++fs)

file(COPY testdata/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../bin/testdata)
