#ifndef CAPENGINE_H
#define CAPENGINE_H

//@brief - master include file for CapEngine
//@author - Caleb Palmer

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "CapEngineException.h"
#include "EventDispatcher.h"
#include "IEventSubscriber.h"
#include "Point2d.h"
#include "Time.h"
#include "VideoManager.h"
#include "animation.h"
#include "animator.h"
#include "asset_manager.h"
#include "button.h"
#include "button_group.h"
#include "capcommon.h"
#include "captypes.h"
#include "collision.h"
#include "colour.h"
#include "componentfactory.h"
#include "components.h"
#include "controller.h"
#include "eventsubscriber.h"
#include "filesystem.h"
#include "flashing_animation.h"
#include "fontmanager.h"
#include "game_management.h"
#include "gameobject.h"
#include "gamestate.h"
#include "keyboard.h"
#include "locator.h"
#include "logger.h"
#include "map2d.h"
#include "mapobjectdescriptor.h"
#include "matrix.h"
#include "mouse.h"
#include "null_ai_component.h"
#include "null_custom_component.h"
#include "null_graphics_component.h"
#include "null_input_component.h"
#include "null_physics_component.h"
#include "numbergenerator.h"
#include "objectfactory.h"
#include "pcm.h"
#include "physics.h"
#include "platformermap.h"
#include "runner.h"
#include "scanconvert.h"
#include "scene2dstate.h"
#include "soundplayer.h"
#include "textbutton.h"
#include "tileset.h"
#include "timestep.h"
#include "tools.h"
#include "uiobject.h"
#include "vector.h"
#include "viewport.h"
#include "widget.h"
#include "xml_parser.h"

#endif
