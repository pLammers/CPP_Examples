#include "calculator.h"

int main()
{
	calculator *calc = new calculator();

	while (1)
	{
		try
		{
			calc->run();
		}
		catch (std::exception e)
		{
			std::cout << std::endl << "Excecution failed with Error: " << e.what() << std::endl;
		}
	}
	std::cin.get();

	return 0;
}