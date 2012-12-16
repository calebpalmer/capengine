#include "numbergenerator.h"
#include <time.h>
#include <stdlib.h>

using namespace CapEngine;

NumberGenerator::NumberGenerator() {
  srand( time(NULL) );
}

int NumberGenerator::getRandom(int start, int end){
  return rand() % end + start + 1;
}
