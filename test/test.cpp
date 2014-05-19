#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCaller.h>
#include <cstdlib>

#include "vectortest.h"
#include "xml_parser_test.h"

int main(){
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(VectorTest::suite());
  runner.addTest(XmlParserTest::suite());
  
  if (runner.run() ){
    return EXIT_SUCCESS;
  }
  else{
    return EXIT_FAILURE;
  }
}
