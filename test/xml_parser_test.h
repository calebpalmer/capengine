#ifndef XML_PARSER_TEST_H
#define XML_PARSER_TEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../CapEngineException.h"
#include "../xml_parser.h"

#include <iostream>
#include <string>
#include <vector>

using namespace CapEngine;
using namespace std;

class XmlParserTest : public CppUnit::TestFixture
{
private:
  XmlParser *parser;

public:
  XmlParserTest() : CppUnit::TestFixture()
  {
    // because setup() / teardown() is not being called.  I don't know why.
    // string xmlFile("test.xml");
    // parser = new XmlParser(xmlFile);
  }

  void setUp()
  {
    string xmlFile("test.xml");
    parser = new XmlParser(xmlFile);
    CPPUNIT_ASSERT(parser != nullptr);
  }

  void tearDown() { delete parser; }

  void testGetRoot()
  {
    XmlNode node = parser->getRoot();
    CPPUNIT_ASSERT(node != nullptr);
  }

  void testRootNodeName()
  {
    XmlNode node = parser->getRoot();
    CPPUNIT_ASSERT(parser->getNodeName(node) == "tests");
  }

  void testGetNodeChildren()
  {
    XmlNode root = parser->getRoot();
    vector<XmlNode> nodes = parser->getNodeChildren(root);
    CPPUNIT_ASSERT(nodes.size() == 2);
  }

  void testGetNextNode()
  {
    XmlNode root = parser->getRoot();
    vector<XmlNode> nodes = parser->getNodeChildren(root);
    XmlNode node1 = nodes[0];
    XmlNode node2 = nodes[1];
    XmlNode nextNode = parser->getNextNode(node1);
    CPPUNIT_ASSERT(node2 == nextNode);
  }

  void testGetStringValue()
  {
    XmlNode root = parser->getRoot();
    vector<XmlNode> nodes = parser->getNodeChildren(root);
    XmlNode node1 = nodes[0];
    string textValue = parser->getStringValue(node1);
    CPPUNIT_ASSERT(textValue == "value1");

    XmlNode node2 = nodes[1];
    string textValue2 = parser->getStringValue(node2);
    CPPUNIT_ASSERT(textValue2 == "value2");
  }

  void testGetAttribute()
  {
    XmlNode root = parser->getRoot();
    vector<XmlNode> nodes = parser->getNodeChildren(root);
    XmlNode node1 = nodes[0];
    string attValue = parser->getAttribute(node1, "id");
    CPPUNIT_ASSERT(attValue == "1");
  }

  void testNodeNameCompare()
  {
    XmlNode node = parser->getRoot();
    CPPUNIT_ASSERT(parser->nodeNameCompare(node, "tests") &&
                   !(parser->nodeNameCompare(node, "test")));
  }

  void testGetNodes()
  {
    vector<XmlNode> nodes = parser->getNodes("//test");
    CPPUNIT_ASSERT(nodes.size() == 2 &&
                   parser->getNodeName(nodes[0]) == "test" &&
                   parser->getNodeName(nodes[1]) == "test");
  }

  CPPUNIT_TEST_SUITE(XmlParserTest);
  CPPUNIT_TEST(testGetRoot);
  CPPUNIT_TEST(testRootNodeName);
  CPPUNIT_TEST(testGetNodeChildren);
  // CPPUNIT_TEST(testGetNextNode);  // I'm not sure how I want this to behave
  CPPUNIT_TEST(testGetStringValue);
  CPPUNIT_TEST(testGetAttribute);
  CPPUNIT_TEST(testNodeNameCompare);
  CPPUNIT_TEST(testGetNodes);
  CPPUNIT_TEST_SUITE_END();
};

#endif // XML_PARSER_TEST_H
