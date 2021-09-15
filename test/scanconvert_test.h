#ifndef SCANCONVERTTEST_H
#define SCANCONVERTTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
/* #include "../scanconvert.h" */
/* #include "../VideoManager.h" */
/* #include "../captypes.h" */
#include "../CapEngine.h"

#include <fstream>
#include <iostream>

using namespace CapEngine;

namespace
{
bool comparePixel(Pixel lhs, Pixel rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

SurfacePtr getTestSurfaceWithLine(VideoManager &in_videoManager)
{
    SurfacePtr surface = in_videoManager.createSurfacePtr(20, 20);
    fillRectangle(surface.get(), {0, 0, 20, 20}, {0xff, 0xff, 0xff, 0xFF});
    drawLine(2, 2, 8, 8, surface.get());
    return surface;
}

std::vector<Pixel> getPixels(VideoManager &videoManager, SurfacePtr surface)
{
    CapEngine::real width = videoManager.getSurfaceWidth(surface.get());
    CapEngine::real height = videoManager.getSurfaceHeight(surface.get());

    std::vector<Pixel> pixels(width * height);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            pixels.push_back(getPixelComponents(surface.get(), i, j));
        }
    }
    return pixels;
}
} // namespace

class ScanconvertTest : public CppUnit::TestFixture
{
    VideoManager *m_videoManager = nullptr;

  public:
    void setUp()
    {
        WindowParams windowParams = {"test", 320,   210,   32,    false,
                                     false,  false, false, "test"};

        init(windowParams);
        m_videoManager = Locator::videoManager;
    }

    void tearDown() { m_videoManager->shutdown(); }

    void testDrawLine()
    {
        SurfacePtr surface = m_videoManager->createSurfacePtr(20, 20);
        drawLine(2, 2, 8, 8, surface.get());

        Pixel black = {0x00, 0x00, 0x00, 0x00};

        Pixel pixel = getPixelComponents(
            surface.get(), 2, m_videoManager->toScreenCoord(surface.get(), 2));
        CPPUNIT_ASSERT(comparePixel(black, pixel));

        pixel = getPixelComponents(
            surface.get(), 8, m_videoManager->toScreenCoord(surface.get(), 8));
        CPPUNIT_ASSERT(comparePixel(black, pixel));
    }

    void testGetSlopeAtPixel()
    {
        auto surface = getTestSurfaceWithLine(*m_videoManager);

        // TODO remove when done debugging
        m_videoManager->saveSurface(surface.get(), "/tmp/surface.bmp");
        std::string surfaceASCII = surfaceToASCII(surface.get());
        std::fstream fileStream("/tmp/surface.txt", std::ios_base::out);
        if (fileStream.good()) {
            fileStream << surfaceASCII;
            fileStream.close();
        } else {
            throw CapEngineException("Could not open file");
        }

        Pixel solidPixel = {0x00, 0x00, 0x00, 0xFF};
        boost::optional<CapEngine::real> slope =
            getSlopeAtPixel(surface.get(), 15, 15, solidPixel, 3);

        CPPUNIT_ASSERT(!slope);

        slope = getSlopeAtPixel(surface.get(), 5,
                                m_videoManager->toScreenCoord(surface.get(), 5),
                                solidPixel, 3);
        std::cout << slope.get() << std::endl;
        CapEngine::real expected = 1.0;

        std::cout << "Expected: " << expected << std::endl
                  << "Actual" << slope.get() << std::endl;

        CPPUNIT_ASSERT(slope);
        CPPUNIT_ASSERT(expected == slope.get());
    }

    void testGetPixel()
    {
        SurfacePtr surface =
            m_videoManager->loadSurfacePtr("testdata/pixel_test.png");
        CPPUNIT_ASSERT(surface != nullptr);

        Pixel black = {0x00, 0x00, 0x00, 0x00};
        Pixel blue = {0x00, 0x05, 0xff, 0x00};
        Pixel white = {0xff, 0xff, 0xff, 0x00};

        Pixel pixel = getPixelComponents(
            surface.get(), 0, m_videoManager->toScreenCoord(surface.get(), 0));
        CPPUNIT_ASSERT(comparePixel(blue, pixel));

        pixel = getPixelComponents(
            surface.get(), 19,
            m_videoManager->toScreenCoord(surface.get(), 19));
        CPPUNIT_ASSERT(comparePixel(blue, pixel));

        pixel = getPixelComponents(
            surface.get(), 2, m_videoManager->toScreenCoord(surface.get(), 2));
        CPPUNIT_ASSERT(comparePixel(black, pixel));

        pixel = getPixelComponents(
            surface.get(), 6, m_videoManager->toScreenCoord(surface.get(), 6));
        CPPUNIT_ASSERT(comparePixel(black, pixel));

        pixel = getPixelComponents(
            surface.get(), 1, m_videoManager->toScreenCoord(surface.get(), 1));
        CPPUNIT_ASSERT(comparePixel(white, pixel));

        pixel = getPixelComponents(
            surface.get(), 10, m_videoManager->toScreenCoord(surface.get(), 3));
        CPPUNIT_ASSERT(comparePixel(white, pixel));
    }

    CPPUNIT_TEST_SUITE(ScanconvertTest);
    // CPPUNIT_TEST(testGetSlopeAtPixel);
    CPPUNIT_TEST(testGetPixel);
    CPPUNIT_TEST(testDrawLine);
    CPPUNIT_TEST_SUITE_END();
};

#endif // SCANCONVERTTEST_H
