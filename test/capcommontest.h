#ifndef CAPCOMMONTEST_H
#define CAPCOMMONTEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../capcommon.h"
#include "../CapEngineException.h"

#include <string>
#include <iostream>
#include <vector>

using namespace CapEngine;
using namespace std;

class CapCommonTest : public CppUnit::TestFixture {
 public:
  void setup() {
  }

  void teardown() {
  }

  void testStripString(){
    string str = "  test  ";
    string stripped = strip(str);
    CPPUNIT_ASSERT(stripped == "test");
  }

  void testStripStringNoFront(){
    string str = "test  ";
    string stripped = strip(str);
    CPPUNIT_ASSERT(stripped == "test");
  }

  void testStripStringNoBack(){
    string str = "  test";
    string stripped = strip(str);
    CPPUNIT_ASSERT(stripped == "test");
  }

  void testStripStringNothing(){
    string str = "test";
    string stripped = strip(str);
    CPPUNIT_ASSERT(stripped == "test");
  }

  void testReplace(){
    string str = "test";
    string replaced = replace(str, "t", "r");
    CPPUNIT_ASSERT(replaced == "resr");
  }

  void testReplaceMultiple(){
    string str = "test";
    string replaced = replace(str, "st", "am");
    CPPUNIT_ASSERT(replaced == "team");
  }

  void testReplaceNonSymmetric(){
    string str = "test";
    string replaced = replace(str, "st", "ase");
    CPPUNIT_ASSERT(replaced == "tease");
  }

  void testReplaceNonSymmetric2(){
    string str = "test";
    string replaced = replace(str, "test", "yo");
    CPPUNIT_ASSERT(replaced == "yo");
  }

  void testReplaceNewLine(){
    string str = "test\n";
    string replaced = replace(str, "\n", " ");
    CPPUNIT_ASSERT(replaced == "test ");
  }

  void testReplaceWindowsNewLine(){
    string str = "test\r\n";
    string replaced = replace(str, "\r\n", " ");
    CPPUNIT_ASSERT(replaced == "test ");
  }

  void testReplaceMacNewLine(){
    string str = "test\r";
    string replaced = replace(str, "\r", " ");
    CPPUNIT_ASSERT(replaced == "test ");
  }

  void testReplaceOSNewLine(){
    string str = "test" LINESEP;
    string replaced = replace(str, LINESEP, " ");
    CPPUNIT_ASSERT(replaced == "test ");
  }
  
  CPPUNIT_TEST_SUITE(CapCommonTest);
  CPPUNIT_TEST(testStripString);
  CPPUNIT_TEST(testStripStringNoFront);
  CPPUNIT_TEST(testStripStringNoBack);
  CPPUNIT_TEST(testStripStringNothing);
  CPPUNIT_TEST(testReplace);
  CPPUNIT_TEST(testReplaceMultiple);
  CPPUNIT_TEST(testReplaceNonSymmetric);
  CPPUNIT_TEST(testReplaceNonSymmetric2);
  CPPUNIT_TEST(testReplaceNewLine);
  CPPUNIT_TEST(testReplaceWindowsNewLine);
  CPPUNIT_TEST(testReplaceMacNewLine);
  CPPUNIT_TEST(testReplaceOSNewLine);
  CPPUNIT_TEST_SUITE_END();


};

#endif // CAPCOMMONTEST_H
