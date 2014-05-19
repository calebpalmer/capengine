#include "xml_parser.h"

#include "CapEngineException.h"

#include <sstream>

using namespace std;
using namespace CapEngine;

XmlParser::XmlParser(const string xmlPath) : mXmlPath(xmlPath) {
  mpDoc = xmlParseFile(xmlPath.c_str());
  if(mpDoc == NULL){
    ostringstream errorMessage;
    errorMessage << "Unable to parse file " << mXmlPath;
    throw CapEngineException(errorMessage.str());
  }
}

XmlParser::~XmlParser(){
  if(mpDoc)
    xmlFreeDoc(mpDoc);
  xmlCleanupParser();
}

vector<XmlNode> XmlParser::getNodes(const string _expression){
  mpXPathContext = xmlXPathNewContext(mpDoc);

  // need to cast off the const from string::c_str() and then cast to xmlChar*
  xmlChar* expression = reinterpret_cast<xmlChar *>(const_cast<char *>(_expression.c_str()));
  xmlXPathObjectPtr result = xmlXPathEvalExpression(expression, mpXPathContext);

  // iterate through xpath results and add to vector
  int i;
  xmlNodeSetPtr nodeset = result->nodesetval;
  vector<XmlNode> nodes;
  for(i = 0; i < nodeset->nodeNr; i++){
    // not sure of the ownership of the XmlNodePtr in nodeset->nodeTab[i]
    // maybe it lasts for as long as the xmlDoc exists?
    nodes.push_back(nodeset->nodeTab[i]);
  }
    
  xmlXPathFreeObject(result);
  return nodes;
}

XmlNode XmlParser::getRoot(){
  XmlNode root = xmlDocGetRootElement(mpDoc);
  if(root == NULL){
    throw CapEngineException("Could not get root node");
  }
  return root;
}

string XmlParser::getNodeName(XmlNode node){
  if(node == nullptr){
    throw CapEngineException("Node is invalid");
  }
  // this should use c++ style casts
  string name = (char *)node->name;
  return name;
}

vector<XmlNode> XmlParser::getNodeChildren(XmlNode node){
  if(node == nullptr){
    throw CapEngineException("Invalid node");
  }
  vector<XmlNode> children;
  XmlNode currentChild = node->xmlChildrenNode;
  while(currentChild != nullptr){
    children.push_back(currentChild);
    currentChild = currentChild->next;
  }

  return children;
}

XmlNode XmlParser::getNextNode(XmlNode node){
  return node->next;
}

string XmlParser::getStringValue(XmlNode node){
  string value = (char *)xmlNodeListGetString(mpDoc, node->xmlChildrenNode, 1);
  return value;
}

string XmlParser::getAttribute(XmlNode node, const string key){
  xmlChar *value;
  value = xmlGetProp(node, (xmlChar *)key.c_str());
  string retVal = (char *)value;
  xmlFree(value);
  return retVal;
}

bool nodeNameCompare(XmlNode node, const std::string name){
  if(!xmlStrcmp(node->name, (const xmlChar *)name.c_str() )){
    return true;
  }
  else{
    return false;
  }
}
