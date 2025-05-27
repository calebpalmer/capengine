add_definitions(-DUNIX -DDEBUG)

find_package(SDL2)
find_package(SDL2_image)
find_package(SDL2_ttf)
find_package(libxml2)
find_package(libsndfile)
find_package(Boost)

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
  components.cpp gamestate.cpp animatorv2.cpp tiledscene.cpp tiledmap.cpp tiledtileset.cpp tiledtilelayer.cpp tiledobjectgroup.cpp
  tiledcustomproperty.cpp logging.cpp
  )

target_include_directories(
  capengine
  PUBLIC
  "../../extern/jsoncons/include"
)

target_link_libraries(capengine
  SDL2::SDL2
  SDL2_image::SDL2_image
  SDL2_ttf::SDL2_ttf
  Boost::Boost
  libxml2::libxml2
  libsndfile::libsndfile
  stdc++fs
  sndio
  )

add_subdirectory(test)
add_subdirectory(gtests)

add_custom_target(test_files ALL cp -r ${CMAKE_CURRENT_SOURCE_DIR}/test_files ${CMAKE_BINARY_DIR})
add_custom_target(supportfiles ALL cp -r ${CMAKE_CURRENT_SOURCE_DIR}/supportfiles ${CMAKE_BINARY_DIR})
