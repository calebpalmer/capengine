#include "colour.h"
#include "CapEngineException.h"

CapEngine::Colour::Colour() : m_r(0), m_g(0), m_b(0), m_a(255) {}

CapEngine::Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : m_r(r), m_g(g), m_b(b), m_a(a)
{
}

CapEngine::Colour::Colour(std::string const& in_colour){
    // Given a colour of the form #RRGGBBAA, parse it into the colour
    if(in_colour.size() != 9 && in_colour.size() != 7){
        throw CapEngineException("Invalid colour string");
    }

    std::string r_str = in_colour.substr(1, 2);
    std::string g_str = in_colour.substr(3, 2);
    std::string b_str = in_colour.substr(5, 2);
    std::string a_str = in_colour.size() == 9 ? in_colour.substr(7, 2) : "FF";

    m_r = std::stoi(r_str, nullptr, 16);
    m_g = std::stoi(g_str, nullptr, 16);
    m_b = std::stoi(b_str, nullptr, 16);
    m_a = std::stoi(a_str, nullptr, 16);
}
