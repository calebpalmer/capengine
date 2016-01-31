#include "colour.h"

CapEngine::Colour::Colour() :
  m_r(0), m_g(0), m_b(0), m_a(255) {}
  
CapEngine::Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
  m_r(r), m_g(g), m_b(b), m_a(a)
  {}
