#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>


double average(std::vector<double> vec)
{
	double avg = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		avg += vec[i];
	}
	avg /= vec.size();

	return avg;
}

int main()
{
	std::string line;
	std::vector<double> input;
	double value;

	std::cout << "Bitte Zahlen eingeben, Leerzeichen zur Trennung! '.' fuer Dezimalzahlen!" << std::endl;

	
	std::getline(std::cin, line);

	std::istringstream iss(line);
	while (iss >> value) {
		input.push_back(value);
	}

	std::cout << "Der Durchschnitt betraegt: ";
	std::cout << average(input) << std::endl;

	std::cout << "Enter zum Beenden druecken...";
	std::cin.get();

	return 0;
}