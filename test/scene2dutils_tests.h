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

void Scene2dUtilsTest::testReadRectangle(){

}

}}

#endif// CAPENGINE_SCENE2DUTILSTESTS_H
