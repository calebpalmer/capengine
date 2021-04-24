#include "sdlutils.h"

bool operator<(const SDL_KeyboardEvent &lhs, const SDL_KeyboardEvent &rhs)
{
  return false;
}

bool operator>(const SDL_KeyboardEvent &lhs, const SDL_KeyboardEvent &rhs)
{
  return rhs < lhs;
}
bool operator<=(const SDL_KeyboardEvent &lhs, const SDL_KeyboardEvent &rhs)
{
  return !(lhs > rhs);
}
bool operator>=(const SDL_KeyboardEvent &lhs, const SDL_KeyboardEvent &rhs)
{
  return !(lhs < rhs);
}
