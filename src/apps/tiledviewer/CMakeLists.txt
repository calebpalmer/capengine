# main
add_executable(tiledviewer
  tiledtest.cpp
  teststate.cpp)

target_include_directories(tiledviewer
  SYSTEM PUBLIC ../extern)

target_include_directories(tiledviewer
  SYSTEM PUBLIC
  ${CAPENGINE_INCLUDE_DIR}
  )

target_link_libraries(tiledviewer
  capengine)
