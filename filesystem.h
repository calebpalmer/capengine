#ifndef FILESYSTEM_H
#define FILESYSTEM_H
// Platform dependant

#include <string>

namespace CapEngine
{
bool fileExists(const std::string &filepath);
void setCurrentDir(const std::string &directory);
std::string getCurrentDir();
std::string getCurrentExecutablePath();
std::string stripPath(const std::string &path);
} // namespace CapEngine

#endif // FILESYSTEM_H
