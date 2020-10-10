cmake_minimum_required(VERSION 3.9)

project(capengine)

include(GNUInstallDirs)
set(CMAKE_EXPORT_COMPILE_COMMANDS "1" CACHE STRING "")

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/sdl2-cmake-modules")
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(LibXml2 REQUIRED)
find_package(cppunit REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${LIBXML2_INCLUDE_DIR}
  ${CPPUNIT_INCLUDE_DIRS}
  "${CMAKE_CURRENT_SOURCE_DIR}/jsoncons/include"
  )

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-g -Wall -Wextra -Wpedantic -Wno-reorder -Wno-unused -std=c++17)
endif()

add_definitions(-DUNIX -DDEBUG)

add_library(capengine
  Point2d.cpp Time.cpp VideoManager.cpp EventDispatcher.cpp vector.cpp matrix.cpp collision.cpp fontmanager.cpp
  capcommon.cpp numbergenerator.cpp pcm.cpp soundplayer.cpp filesystem.cpp tileset.cpp map2d.cpp logger.cpp timestep.cpp
  scanconvert.cpp xml_parser.cpp asset_manager.cpp mouse.cpp locator.cpp textbutton.cpp gameobject.cpp game_management.cpp
  runner.cpp platformermap.cpp colour.cpp button_group.cpp controller.cpp physics.cpp IEventSubscriber.cpp
  flashing_animation.cpp viewport.cpp objectfactory.cpp tools.cpp animator.cpp mapobjectdescriptor.cpp eventsubscriber.cpp
  windowwidget.cpp absolutelayout.cpp label.cpp captypes.cpp widgetstate.cpp widget.cpp gridlayout.cpp utils.cpp linearlayout.cpp
  uiconfigmanager.cpp uibutton.cpp orientable.cpp tilecopycontrol.cpp uiutils.cpp quaternion.cpp dialogstate.cpp pancontrol.cpp
  widgetdecorator.cpp simplecommand.cpp commandmanager.cpp aggregatecommand.cpp textbox.cpp font.cpp property.cpp listproperty.cpp
  uipropertyvisitor.cpp simpleobjectmanager.cpp scene2dstate.cpp camera2d.cpp layerfactory.cpp componentfactory.cpp
  imagelayer.cpp scene2dutils.cpp scene2dschema.cpp bitmapcollisionlayer.cpp gameobjectutils.cpp componentutils.cpp
  boxcollider.cpp rigidbodycomponent.cpp placeholdergraphics.cpp keyboard.cpp
  )

target_link_libraries(capengine ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${Boost_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  libsndfile.so
  stdc++fs
  )


add_executable(editor editor.cpp map_panel.cpp tilesetpanel.cpp)

target_link_libraries(editor capengine
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${Boost_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  libsndfile.so
  stdc++fs
  )


add_executable(widgettest widget_test.cpp)

target_link_libraries(widgettest capengine
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${Boost_LIBRARIES}
  ${LIBXML2_LIBRARIES}
  libsndfile.so
  stdc++fs
  )

add_subdirectory(test)

add_custom_target(
  doc
  COMMAND doxygen doxygenfile
  )


# installs
INSTALL(TARGETS capengine widgettest editor
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION bin
  ARCHIVE DESTINATION bin
  )

# installing dependencies
# set(MY_BINARY_LOCATION $<TARGET_FILE:capengine> CACHE STRING "")
# FILE(
#   GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/dependencies.cmake"
#   INPUT "${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake.in"
#   )
# INSTALL(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/dependencies.cmake")

install(DIRECTORY res
  DESTINATION ./)
