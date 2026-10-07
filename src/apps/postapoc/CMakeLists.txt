# main
add_executable(postapoc
  main.cpp
  levelgamestate.cpp
  playergraphicscomponent.cpp
  playerphysicscomponent.cpp
  playerinputcomponent.cpp
  inputcontroller.cpp
)

# submodule dependencies
target_include_directories(postapoc
  SYSTEM PUBLIC ../extern)

target_include_directories(postapoc
  SYSTEM PUBLIC
  ${CAPENGINE_INCLUDE_DIR}
  )

target_link_libraries(postapoc PRIVATE
  capengine
  gsl::gsl-lite
  stdc++_libbacktrace
  ) # until <stacktrace> is not experimental we need to link to stdc++_libbacktrace

# resoources
file(GLOB_RECURSE RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/*)

add_custom_target(postapoc_resources ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/postapoc_resources
  DEPENDS ${RESOURCE_FILES}
  COMMENT "Copying resources directory"
)
