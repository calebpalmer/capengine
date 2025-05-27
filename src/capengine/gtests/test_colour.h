#ifndef TEST_COLOUR_H
#define TEST_COLOUR_H

#include <capengine/colour.h>

#include "gtest/gtest.h"

namespace CapEngine::Testing {

TEST(Colour, testColour)
{
    CapEngine::Colour colour(255, 255, 255, 255);
    EXPECT_EQ(colour.m_r, 255);
    EXPECT_EQ(colour.m_g, 255);
    EXPECT_EQ(colour.m_b, 255);
    EXPECT_EQ(colour.m_a, 255);
}

TEST(Colour, testColourFromString)
{
    CapEngine::Colour colour{"#ffffff"};
    EXPECT_EQ(colour.m_r, 0xff);
    EXPECT_EQ(colour.m_g, 0xff);
    EXPECT_EQ(colour.m_b, 0xff);
    EXPECT_EQ(colour.m_a, 0xff);
}

} // namespace CapEngine::Testing

#endif // TEST_COLOUR_H
