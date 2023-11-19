#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cstdlib>

#include "../filesystem.h"
#include "camera2dtest.h"
#include "capcommontest.h"
#include "collisionstest.h"
#include "eventsubscribertest.h"
#include "matrixtest.h"
#include "quaterniontest.h"
#include "rectangletest.h"
#include "scanconvert_test.h"
#include "scene2dutils_tests.h"
#include "vectortest.h"
#include "xml_parser_test.h"

int main()
{
    CapEngine::init(CapEngine::WindowParams{"Tests", 0, 0, 0, false, false,
                                            false, false, "Tests"});

    // set current directory
    CapEngine::setCurrentDir(
        CapEngine::stripPath(CapEngine::getCurrentExecutablePath()));

    CppUnit::TextUi::TestRunner runner;
    runner.addTest(VectorTest::suite());
    runner.addTest(CapCommonTest::suite());
    runner.addTest(XmlParserTest::suite());
    runner.addTest(MatrixTest::suite());
    runner.addTest(RectangleTest::suite());
    runner.addTest(ScanconvertTest::suite());
    runner.addTest(Camera2dTest::suite());
    runner.addTest(CapEngine::QuaternionTest::suite());
    runner.addTest(CapEngine::Testing::Scene2dUtilsTest::suite());
    runner.addTest(CapEngine::Testing::CollisionsTests::suite());
    runner.addTest(CapEngine::Testing::EventSubscriberTest::suite());

    if (runner.run())
    {
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}
