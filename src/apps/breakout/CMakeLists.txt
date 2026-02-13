# main
add_executable(breakout
  main.cpp
  maingamestate.cpp
  playergraphicscomponent.cpp
  playerphysicscomponent.cpp
  blockgraphicscomponent.cpp
  ballphysicscomponent.cpp
  ballgraphicscomponent.cpp
)

# submodule dependencies
target_include_directories(breakout
  SYSTEM PUBLIC ../extern)

target_include_directories(breakout
  SYSTEM PUBLIC
  ${CAPENGINE_INCLUDE_DIR}
  )

target_link_libraries(breakout PRIVATE
  capengine
  gsl::gsl-lite
  stdc++_libbacktrace
  ) # until <stacktrace> is not experimental we need to link to stdc++_libbacktrace

# resoources
file(GLOB_RECURSE RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/*)

add_custom_target(breakout_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/breakout_resources
  DEPENDS ${RESOURCE_FILES}
  COMMENT "Copying resources directory"
)
