#ifndef CAPENGINE_SCENE2DUTILSTESTS_H
#define CAPENGINE_SCENE2DUTILSTESTS_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>
#include <variant>

#include "../captypes.h"
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
    // int metadata
    jsoncons::json intData(1);
    const MetadataType intValue = JSONUtils::readMetadata(intData);
    CPPUNIT_ASSERT_NO_THROW(std::get<int>(intValue));
    CPPUNIT_ASSERT_EQUAL(1, std::get<int>(intValue));

    // double metadata
    jsoncons::json doubleData(9.99);
    const MetadataType doubleValue = JSONUtils::readMetadata(doubleData);
    CPPUNIT_ASSERT_NO_THROW(std::get<double>(doubleValue));
    CPPUNIT_ASSERT_EQUAL(9.99, std::get<double>(doubleValue));

    // double metadata
    jsoncons::json stringData("yo");
    const MetadataType stringValue = JSONUtils::readMetadata(stringData);
    CPPUNIT_ASSERT_NO_THROW(std::get<std::string>(stringValue));
    CPPUNIT_ASSERT("yo" == std::get<std::string>(stringValue));
}

} // namespace Testing
} // namespace CapEngine

#endif // CAPENGINE_SCENE2DUTILSTESTS_H
