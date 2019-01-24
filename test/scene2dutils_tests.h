#ifndef CAPENGINE_SCENE2DUTILSTESTS_H
#define CAPENGINE_SCENE2DUTILSTESTS_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../scene2dutils.h"

namespace CapEngine { namespace Testing {

class Scene2dUtilsTest : public CppUnit::TestFixture {
public:
	void testReadRectangle();
	
	CPPUNIT_TEST_SUITE(Scene2dUtilsTest);
	CPPUNIT_TEST(testReadRectangle);
	CPPUNIT_TEST_SUITE_END();

};

//! Tests reading a rectangle from json.
void Scene2dUtilsTest::testReadRectangle(){
	jsoncons::json rect = jsoncons::json::parse(R"(
{
    "x" : 1,
    "y" : 2,
    "width" : 10,
    "height" : 10
}
)");

	Rectangle expected(1, 2, 10, 10);
	Rectangle actual = JSONUtils::readRectangle(rect);
	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

}}

#endif// CAPENGINE_SCENE2DUTILSTESTS_H
