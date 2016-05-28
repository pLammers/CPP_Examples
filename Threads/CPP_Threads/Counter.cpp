#include "Counter.h"



Counter::Counter()
{
}


Counter::~Counter()
{
}

void Counter::count()
{
	for (int i = 0; i < 10; i++) std::cout << i << std::endl;
}
