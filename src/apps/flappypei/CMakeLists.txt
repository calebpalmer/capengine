# main
find_package(Boost)

add_executable(flappypei
  main.cpp
  maingamestate.cpp
  playergraphicscomponent.cpp
  playerphysicscomponent.cpp
)

# submodule dependencies
target_include_directories(flappypei
  SYSTEM PUBLIC ../extern)

target_include_directories(flappypei
  SYSTEM PUBLIC
  ${CAPENGINE_INCLUDE_DIR}
  )

target_link_libraries(flappypei PRIVATE
  capengine
  stdc++_libbacktrace) # until <stacktrace> is not experimental we need to link to stdc++_libbacktrace

# resoources
file(GLOB_RECURSE RESOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/resources/*)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/flappypei_resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/flappypei_resources
  DEPENDS ${RESOURCE_FILES}
  COMMENT "Copying resources directory"
)

add_custom_target(flappypei_resources ALL DEPENDS ${CMAKE_BINARY_DIR}/flappypei_resources)
