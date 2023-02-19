#include "numbergenerator.h"
#include <stdlib.h>
#include <time.h>

using namespace CapEngine;

NumberGenerator::NumberGenerator() { srand(time(NULL)); }

int NumberGenerator::getRandom(int start, int end)
{
  return rand() % end + start + 1;
}
