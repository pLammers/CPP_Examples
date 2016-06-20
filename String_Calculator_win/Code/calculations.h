#pragma once
#include <vector>
#include <sstream>
#include <iomanip>

class calculations
{
public:
	calculations();
	~calculations();

	//Addiert zwei Werte und gibt Ergebnis zurueck
	double add(double, double);
	//Subtrahiert zwei Werte und gibt Ergebnis zurueck
	double sub(double, double);
	//Multipliziert zwei Werte und gibt Ergebnis zurueck
	double mul(double, double);
	//Dividiert zwei Werte voneinander und gibt Ergebnis zurueck
	double div(double, double);
	//Gibt den Durchschnitt aller Werte im uebergebenen vector zurueck
	double avg(std::vector<double>);

private:

	std::stringstream string_stream;
};

