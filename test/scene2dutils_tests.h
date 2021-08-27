#ifndef CAPENGINE_SCENE2DUTILSTESTS_H
#define CAPENGINE_SCENE2DUTILSTESTS_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include <variant>

#include "../scene2dutils.h"
#include "cppunit/TestAssert.h"

namespace CapEngine
{
namespace Testing
{

class Scene2dUtilsTest : public CppUnit::TestFixture
{
  public:
    void testReadRectangle();
    void testReadComponentMetadata();

    CPPUNIT_TEST_SUITE(Scene2dUtilsTest);
    CPPUNIT_TEST(testReadRectangle);
    CPPUNIT_TEST(testReadComponentMetadata);
    CPPUNIT_TEST_SUITE_END();
};

//! Tests reading a rectangle from json.
void Scene2dUtilsTest::testReadRectangle()
{
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

//! Test reading component metadata
void Scene2dUtilsTest::testReadComponentMetadata()
{
    jsoncons::json intData(1);
    const Component::MetadataType value = JSONUtils::readMetadata(intData);
    CPPUNIT_ASSERT_NO_THROW(std::get<int>(value));
    CPPUNIT_ASSERT_EQUAL(1, std::get<int>(value));
}

} // namespace Testing
} // namespace CapEngine

#endif // CAPENGINE_SCENE2DUTILSTESTS_H
