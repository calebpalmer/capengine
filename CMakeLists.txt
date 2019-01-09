cmake_minimum_required(VERSION 3.9)

project(capengine)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./cmake/cmake-scripts")
LIST(APPEND CMAKE_MODULE_PATH "./cmake/cmake-scripts") 

#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(LibXml2 REQUIRED)
find_package(cppunit REQUIRED)

include_directories(  ${SDL2_TTF_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS}
                    ${LIBXML2_INCLUDE_DIR}
		    ${CPPUNIT_INCLUDE_DIRS}
		    "./thirdparty/jsoncons/include"
		    "./thirdparthy/include"
                    )

link_directories("./thirdparty/lib")

#link_directories()

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-g -Wall -Wextra -Wpedantic -Wno-reorder -Wno-unused -std=c++17)
endif()

add_definitions(-DUNIX -DDEBUG)

add_library(capengine Point2d.cpp Time.cpp VideoManager.cpp EventDispatcher.cpp vector.cpp matrix.cpp collision.cpp fontmanager.cpp 
	    capcommon.cpp numbergenerator.cpp pcm.cpp soundplayer.cpp filesystem.cpp tileset.cpp map2d.cpp logger.cpp timestep.cpp 
	    scanconvert.cpp xml_parser.cpp asset_manager.cpp mouse.cpp locator.cpp textbutton.cpp gameobject.cpp game_management.cpp 
	    runner.cpp platformermap.cpp colour.cpp button_group.cpp controller.cpp physics.cpp IEventSubscriber.cpp 
	    flashing_animation.cpp viewport.cpp objectfactory.cpp tools.cpp animator.cpp mapobjectdescriptor.cpp eventsubscriber.cpp
	    windowwidget.cpp absolutelayout.cpp label.cpp captypes.cpp widgetstate.cpp widget.cpp gridlayout.cpp utils.cpp linearlayout.cpp
	    uiconfigmanager.cpp uibutton.cpp orientable.cpp tilecopycontrol.cpp uiutils.cpp quaternion.cpp dialogstate.cpp pancontrol.cpp
	    widgetdecorator.cpp simplecommand.cpp commandmanager.cpp aggregatecommand.cpp textbox.cpp font.cpp property.cpp listproperty.cpp
	    uipropertyvisitor.cpp simpleobjectmanager.cpp scene2dstate.cpp camera2d.cpp layerfactory.cpp componentfactory.cpp
	    imagelayer.cpp
	    )

add_executable(editor editor.cpp map_panel.cpp tilesetpanel.cpp)

add_executable(widgettest widget_test.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
add_executable(runtests test/test.cpp)


target_link_libraries(capengine SDL2
                      SDL2_image
                      ${SDL2_TTF_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${LIBXML2_LIBRARIES}
		      libsndfile.so
		      stdc++fs
                      )
target_link_libraries(runtests capengine
		      SDL2
                      SDL2_image
                      ${SDL2_TTF_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${LIBXML2_LIBRARIES}
		      ${CPPUNIT_LIBRARIES}
		      libsndfile.so
		      stdc++fs
                      )

target_link_libraries(editor capengine
		      SDL2
                      SDL2_image
                      ${SDL2_TTF_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${LIBXML2_LIBRARIES}
		      libsndfile.so
		      stdc++fs
                      )

target_link_libraries(widgettest capengine
		      SDL2
                      SDL2_image
                      ${SDL2_TTF_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${LIBXML2_LIBRARIES}
		      libsndfile.so
		      stdc++fs
		      )


add_custom_target(
  doc
  COMMAND doxygen doxygenfile
)		    
