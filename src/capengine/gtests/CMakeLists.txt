add_executable(gtests
  main.cpp
  testutils.cpp)

target_include_directories(gtests
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/../.."
  "${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/googletest/googletest/include"
  )

target_link_libraries(gtests
  PRIVATE
  gtest
  capengine)
