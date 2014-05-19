#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <string>
#include <vector>

namespace CapEngine{

  typedef xmlNodePtr XmlNode;

  // This class is just a thin wrapper over libxml2 just for RAII
  class XmlParser{ 
  public:
    XmlParser(const std::string xmlPath);
    ~XmlParser();
    std::vector<XmlNode> getNodes(const std::string expression);
    XmlNode getRoot();
    std::string getNodeName(XmlNode node);
    std::vector<XmlNode> getNodeChildren(XmlNode node);
    XmlNode getNextNode(XmlNode node);
    std::string getStringValue(XmlNode node);
    std::string getAttribute(XmlNode node, const std::string key);
    bool nodeNameCompare(XmlNode node, const std::string name);

  private:
    std::string mXmlPath;
    xmlDocPtr mpDoc;
    xmlXPathContextPtr mpXPathContext;
    
  };

}

#endif // XML_PARSER_H
 
