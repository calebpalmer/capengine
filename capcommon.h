#ifndef CAPCOMMON_H
#define CAPCOMMON_H

#include <functional>
#include <string>

namespace CapEngine
{

#ifdef WIN
#define LINESEP "\r\n"
#endif
#ifdef UNIX
#define LINESEP "\n"
#endif
#ifdef OSX
#define LINESEP "\r"
#endif

#define CARTESIAN_Y_TO_WINDOW_Y(H, Y) H - Y

void sleep(int ms);
std::string strip(const std::string str);
std::string replace(const std::string str, const std::string rep,
                    const std::string val);
void sdlTry(std::function<int()> func);
bool doubles_equal(const double &left, const double &right);
} // namespace CapEngine

#endif
