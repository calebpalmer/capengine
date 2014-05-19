#ifndef XML_PARSER_TEST
#define XML_PARSER_TEST

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../xml_parser.h"
#include "../CapEngineException.h"

#include <string>
#include <iostream>

using namespace CapEngine;
using namespace std;

class XmlParserTest : public CppUnit::TestFixture {
 private:
  XmlParser* parser;

 public:

  virtual void setup() {
    cout << "setup called" << endl;
    string xmlFile("test.xml");
    parser = new XmlParser(xmlFile);  
    CPPUNIT_ASSERT(parser != nullptr);
  }

  virtual void teardown() {
    delete parser;
  }

  void testGetRoot(){
    XmlNode node = parser->getRoot();
    CPPUNIT_ASSERT(node != nullptr);
  }

  void testRootNodeName(){
    XmlNode node = parser->getRoot();
    CPPUNIT_ASSERT(parser->getNodeName(node) == "tests");
  }
  
  CPPUNIT_TEST_SUITE(XmlParserTest);
  CPPUNIT_TEST(testGetRoot);
  CPPUNIT_TEST(testRootNodeName);
  CPPUNIT_TEST_SUITE_END();


};

#endif // XML_PARSER_TEST
