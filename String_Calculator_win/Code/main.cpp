#include "calculator.h"
#include <Windows.h>

int main()
{
	//Veraendern Fenstergroeße
	//Suchen Standartfenster
	HWND hwnd = FindWindow("ConsoleWindowClass", NULL);
	//MoveWindow(Fenster, X-pos, Y-pos, Breite, Hoehe, repaint)
	MoveWindow(hwnd, 500, 300, 800, 400, TRUE);


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