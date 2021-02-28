cmake_minimum_required(VERSION 3.9)
project(capengine_tests)

#set( CMAKE_VERBOSE_MAKEFILE on )

add_executable(runtests test.cpp)

target_link_libraries(runtests
	    capengine
	    ${CONAN_LIBS}
	    stdc++fs)

#configure_file(test.xml ${CMAKE_CURRENT_BINARY_DIR}/test.xml COPYONLY)
#file(COPY test.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
configure_file(test.xml ${CMAKE_CURRENT_BINARY_DIR}/../bin/)
