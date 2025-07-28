# main
add_executable(rps
  main.cpp
  rockpaperscissorsstate.cpp
)

target_include_directories(rps
  SYSTEM PUBLIC ../extern)

target_include_directories(rps
  SYSTEM PUBLIC
  ${CAPENGINE_INCLUDE_DIR}
  )

target_link_libraries(rps PRIVATE
  capengine
  stdc++_libbacktrace) # until <stacktrace> is not experimental we need to link to stdc++_libbacktrace

file(GLOB_RECURSE RESOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/resources/*)

add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/rps_resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/rps_resources
  DEPENDS ${RESOURCE_FILES}
  COMMENT "Copying resources directory"
)


add_custom_target(resources ALL DEPENDS ${CMAKE_BINARY_DIR}/rps_resources)
