#include "capcommon.h"

#include <SDL/SDL.h>

#include <sstream>

using namespace std;

void CapEngine::sleep(int ms){
  SDL_Delay(ms);
}

string CapEngine::strip(const string str){
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

string CapEngine::replace(const string str, const string rep, const string val){
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
