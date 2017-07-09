#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCaller.h>
#include <cstdlib>

#include "vectortest.h"
#include "capcommontest.h"
#include "xml_parser_test.h"
#include "matrixtest.h"
#include "rectangletest.h"

int main(){
  CppUnit::TextUi::TestRunner runner;
  runner.addTest(VectorTest::suite());
  runner.addTest(CapCommonTest::suite());
  runner.addTest(XmlParserTest::suite());
  runner.addTest(MatrixTest::suite());
  runner.addTest(RectangleTest::suite());
  
  if (runner.run() ){
    return EXIT_SUCCESS;
  }
  else{
    return EXIT_FAILURE;
  }
}
