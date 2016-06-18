#include "calculations.h"



calculations::calculations()
{
}


calculations::~calculations()
{
}


//Addiert zwei Werte und gibt Ergebnis zurueck
double calculations::add(double value_a, double value_b)
{
	return value_a + value_b;
}


//Subtrahiert zwei Werte und gibt Ergebnis zurueck
double calculations::sub(double value_a, double value_b)
{
	return value_a - value_b;
}


//Multipliziert zwei Werte und gibt Ergebnis zurueck
double calculations::mul(double value_a, double value_b)
{
	return value_a * value_b;
}


//Dividiert zwei Werte durcheinander und gibt Ergebnis zurueck
double calculations::div(double value_a, double value_b)
{
	return value_a / value_b;
}


//Gibt den Durchschnitt aller Werte im uebergebenen vector zurueck
double calculations::avg(std::vector<double> values)
{
	double avg = 0;
	for (int i = 0; i < values.size(); i++)
	{
		avg += values[i];
	}
	avg /= values.size();

	return avg;
}
