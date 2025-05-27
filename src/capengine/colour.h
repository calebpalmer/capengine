#ifndef COLOUR_H
#define COLOUR_H

#include <cinttypes>
#include <string>

namespace CapEngine
{
class Colour
{
   public:
    Colour();
    Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
    Colour(std::string const& in_colour);
    uint8_t m_r;
    uint8_t m_g;
    uint8_t m_b;
    uint8_t m_a;
};
}  // namespace CapEngine

#endif  //
