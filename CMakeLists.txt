cmake_minimum_required(VERSION 3.9)

project(capengine)

include(GNUInstallDirs)
set(CMAKE_EXPORT_COMPILE_COMMANDS "1" CACHE STRING "")


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/jsoncons/include"
  )


if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-ggdb3 -Wall -Wextra -Wpedantic -Wno-reorder -Wno-unused -std=c++17 -fsanitize=address -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address)
endif()

add_definitions(-DUNIX -DDEBUG)

# capengine
add_library(capengine SHARED
  Point2d.cpp Time.cpp VideoManager.cpp EventDispatcher.cpp vector.cpp matrix.cpp collision.cpp fontmanager.cpp
  capcommon.cpp numbergenerator.cpp pcm.cpp soundplayer.cpp filesystem.cpp tileset.cpp map2d.cpp logger.cpp timestep.cpp
  scanconvert.cpp xml_parser.cpp asset_manager.cpp mouse.cpp locator.cpp textbutton.cpp gameobject.cpp game_management.cpp
  runner.cpp platformermap.cpp colour.cpp button_group.cpp controller.cpp physics.cpp IEventSubscriber.cpp
  flashing_animation.cpp viewport.cpp objectfactory.cpp tools.cpp animator.cpp mapobjectdescriptor.cpp eventsubscriber.cpp
  windowwidget.cpp absolutelayout.cpp label.cpp captypes.cpp widgetstate.cpp widget.cpp gridlayout.cpp utils.cpp linearlayout.cpp
  uiconfigmanager.cpp uibutton.cpp orientable.cpp tilecopycontrol.cpp uiutils.cpp quaternion.cpp dialogstate.cpp pancontrol.cpp
  widgetdecorator.cpp simplecommand.cpp commandmanager.cpp aggregatecommand.cpp textbox.cpp font.cpp property.cpp listproperty.cpp
  uipropertyvisitor.cpp simpleobjectmanager.cpp scene2d.cpp scene2dstate.cpp camera2d.cpp layerfactory.cpp componentfactory.cpp
  imagelayer.cpp scene2dutils.cpp scene2dschema.cpp bitmapcollisionlayer.cpp gameobjectutils.cpp componentutils.cpp
  boxcollider.cpp rigidbodycomponent.cpp placeholdergraphics.cpp keyboard.cpp vectorcollisionlayer.h sdlutils.cpp
  components.cpp
  )

target_link_libraries(capengine
  ${CONAN_LIBS}
  stdc++fs
  sndio
  )

# editor
add_executable(editor editor.cpp map_panel.cpp tilesetpanel.cpp)

target_link_libraries(editor
  capengine
  ${CONAN_LIBS}
  stdc++fs)

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

install(
  TARGETS capengine
  DESTINATION ./
  )

add_subdirectory(test)

add_custom_target(
  doc
  COMMAND doxygen doxygenfile
  )
