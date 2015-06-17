#include "filesystem.h"

#include <iostream>

#include "CapEngineException.h"

#ifdef UNIX  // unix include files
#include <unistd.h>
#endif

using namespace std;

bool CapEngine::fileExists(const string& filepath){
  if(access(filepath.c_str(), F_OK) == 0){
    return true;
  }
  else{
    return false;
  }
}

void CapEngine::setCurrentDir(const string& directory){
#ifdef UNIX
    const char* path = directory.c_str();
    if(chdir(path) != 0){
      cerr << "Unable to set current directory to " << path << endl;
      throw CapEngine::CapEngineException("Unable to set current directory to path of executable");
    }
#endif //UNIX

#ifdef WIN

#endif //WIN
}

string CapEngine::getCurrentDir(){
  string strCurdir;
#ifdef UNIX
  //change dir code here
  char *curdir = 0;
  curdir = getcwd(NULL, 0);
  strCurdir = curdir;
  free(curdir);
  return strCurdir;
#endif
  
#ifdef WIN
  //change dir code here
#endif
  
  return strCurdir;
}

string CapEngine::getCurrentExecutablePath(){
  string strCurdir;
#ifdef UNIX
  //change dir code here
  const ssize_t buffsize = 255;
  char buffer[buffsize];
  char link[] = "/proc/self/exe";
  ssize_t charsRead = readlink(link, buffer, buffsize);
  if(charsRead < buffsize and charsRead > 0){
    buffer[charsRead] = '\0';
  }
  strCurdir = buffer;
#endif
  
#ifdef WIN
  //change dir code here
#endif
  
  return strCurdir;
}

string CapEngine::stripPath(const string& path){
  char pathSeparator = '/';
#ifdef WIN
  pathSeparator = '\\';
#endif
  string newPath = path.substr(0, path.find_last_of(pathSeparator));
  return newPath;
}
