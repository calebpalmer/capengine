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

target_link_libraries(rps
  capengine)

add_custom_target(resources ALL
  COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/resources/ ${CMAKE_BINARY_DIR}/rps_resources
)
