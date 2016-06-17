#pragma once
#include "calculations.h"
#include <iostream>
#include <string>
#include <locale>
#include <exception>

class calculator
{
public:
	calculator();
	~calculator();

	void run();

private:

	calculations *calc;


	std::string solveFormula(std::string);

	//Wird aufgerufen, wenn "+" oder "-" in der Formel existieren
	std::string solveAddSub(std::string);
	//Wird aufgerufen, wenn "*" oder "/" in der Formel existieren
	std::string solveMulDiv(std::string);
	//Wird aufgerufen, wenn "(" in der Formel existiert
	std::string solveBrackets(std::string);

	//Wird aufgerufen fuer Addition(von "solveAddSub()")
	std::string solveAdd(std::string);
	//Wird aufgerufen fuer Subtraktion(von "solveAddSub()")
	std::string solveSub(std::string);
	//Wird aufgerufen fuer Multiplikation(von "solveMulDiv()")
	std::string solveMul(std::string);
	//Wird aufgerufen fuer Division(von "solveMulDiv()")
	std::string solveDiv(std::string);

	//Gibt Bedienungsanleitung auf dem Bildschirm aus
	void manual();
};

