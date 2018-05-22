#include "capcommon.h"

#include "CapEngineException.h"

#include <SDL2/SDL.h>

#include <sstream>
#include <boost/exception/all.hpp>

using namespace std;

namespace CapEngine{

void sleep(int ms){
  SDL_Delay(ms);
}

string strip(const string str){
  // strip newlines off first
  string replaced = replace(str, LINESEP, " ");
  
  // strip off any other leading or trailing spaces
  std::size_t start = replaced.find_first_not_of(" ");
  std::size_t end = replaced.find_last_not_of(" ");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
  if(start == string::npos){
    start = 0;
  }
  if(end == string::npos){
    end = replaced.size() - 1;
  }
#pragma GCC diagnostic pop

  string stripped = replaced.substr(start, end - start + 1);
  return stripped;
}

string replace(const string str, const string rep, const string val){
  int length = str.size();
  int repLength = rep.size();

  ostringstream replaced;
  int i = 0;
  while(i < length){
    if((str.substr(i, repLength)).compare(rep) == 0){
      replaced << val;
      i += repLength;
    }
    else{
      replaced << str.substr(i, 1);
      i++;
    }
  }

  return replaced.str();
}


void sdlTry(std::function<int()> func){
  if(func() != 0){
    std::stringstream msg;
    msg << "SDL Exception: " << SDL_GetError();
    BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
  }
}


//! checks if two double are equal
/** 

 https://stackoverflow.com/questions/4010240/comparing-doubles

 \param left
 \param right
 \return - true if they are equal, false otherwise
*/
bool doubles_equal(const double& left, const double& right){
		return left == right || 
		std::abs(left - right) <= std::abs(std::min(left, right)) * std::numeric_limits<double>::epsilon();
}

}
