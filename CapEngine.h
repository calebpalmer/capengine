#ifndef CAPENGINE_H
#define CAPENGINE_H

//@brief - master include file for CapEngine
//@author - Caleb Palmer

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "captypes.h"
#include "capcommon.h"
#include "CapEngineException.h"
#include "VideoManager.h"
#include "Point2d.h"
#include "vector.h"
#include "matrix.h"
#include "Time.h"
#include "EventDispatcher.h"
#include "IEventSubscriber.h"
#include "collision.h"
#include "fontmanager.h"
#include "numbergenerator.h"
#include "soundplayer.h"
#include "pcm.h"
#include "logger.h"
#include "tileset.h"
#include "map2d.h"
#include "platformermap.h"
#include "filesystem.h"
#include "timestep.h"
#include "keyboard.h"
#include "mouse.h"
#include "scanconvert.h"
#include "asset_manager.h"
#include "xml_parser.h"
#include "locator.h"
#include "uiobject.h"
#include "button.h"
#include "textbutton.h"
#include "gameobject.h"
#include "gamestate.h"
#include "null_custom_component.h"
#include "null_input_component.h"
#include "null_graphics_component.h"
#include "null_physics_component.h"
#include "runner.h"
#include "game_management.h"

#endif
