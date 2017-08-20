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
  unsigned int start = replaced.find_first_not_of(" ");
  unsigned int end = replaced.find_last_not_of(" ");

  if(start == string::npos){
    start = 0;
  }
  if(end == string::npos){
    end = replaced.size() - 1;
  }

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

}
