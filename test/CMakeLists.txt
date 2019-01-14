cmake_minimum_required(VERSION 3.9)
project(capengine_tests)

add_executable(runtests test.cpp)

target_link_libraries(runtests capengine
		      ${SDL2_LIBRARY}
                      ${SDL2_IMAGE_LIBRARIES}
                      ${SDL2_TTF_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${LIBXML2_LIBRARIES}
		      ${CPPUNIT_LIBRARIES}
		      libsndfile.so
		      stdc++fs
                      )

configure_file(test.xml ${CMAKE_CURRENT_BINARY_DIR}/test.xml COPYONLY)
