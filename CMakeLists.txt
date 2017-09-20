cmake_minimum_required(VERSION 3.9)

project(capengine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./cmake/sdl2-cmake-scripts")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(LibXml2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIRS}
                    ${LIBXML2_INCLUDE_DIR}
                    )

#link_directories()

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_definitions(-DUNIX)

add_library(libcapengine Point2d.cpp Time.cpp VideoManager.cpp EventDispatcher.cpp vector.cpp matrix.cpp collision.cpp fontmanager.cpp 
	    capcommon.cpp numbergenerator.cpp pcm.cpp soundplayer.cpp filesystem.cpp tileset.cpp map2d.cpp logger.cpp timestep.cpp 
	    scanconvert.cpp xml_parser.cpp asset_manager.cpp mouse.cpp locator.cpp textbutton.cpp gameobject.cpp game_management.cpp 
	    runner.cpp platformermap.cpp colour.cpp button_group.cpp controller.cpp physics.cpp IEventSubscriber.cpp tileset_panel.cpp 
	    map_panel.cpp flashing_animation.cpp viewport.cpp objectfactory.cpp tools.cpp animator.cpp)

target_link_libraries(libcapengine ${SDL2_LIBRARY} 
                      ${SDL2_IMAGE_LIBRARIES}
                      ${SDL2_TTF_LIBARIES}
                      ${Boost_LIBRARIES}
                      ${LIBXML2_LIBRARIES}
		      libsndfile.so
                      )

