#include "scene2dutils.h"

#include "scene2dschema.h"
#include "CapEngineException.h"

#include <jsoncons/json.hpp>

namespace CapEngine { namespace Utils {

Rectangle readRectangle(const jsoncons::json &in_json){
	try {
		using namespace Schema::Scene2d;
		
		Rectangle rect;
		rect.x = in_json[kPosition][kXCoord].as<int>();
		rect.y = in_json[kPosition][kYCoord].as<int>();
		rect.width = in_json[kPosition][kWidth].as<int>();
		rect.height = in_json[kPosition][kHeight].as<int>();
		return rect;
	}
	catch(std::exception &e){
		std::stringstream msg;
		msg << "Unable to parse rectangle from json."
				<< std::endl << jsoncons::pretty_print(in_json)
				<< std::endl << boost::diagnostic_information(e) << std::endl;
		BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
	}
}

}}
