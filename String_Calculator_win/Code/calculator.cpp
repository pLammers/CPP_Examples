#include "calculator.h"

calculator::calculator()
{
	calculations *calc = new calculations();
	this->calc = calc;
}


calculator::~calculator()
{
}


void calculator::run()
{
	std::string formula;
	
	std::cout << "\nBitte Formel eingeben('man' fuer Anleitung)...\n> ";
	std::getline(std::cin, formula);

	//Wenn eingabe 'man' ist geben wir das Handbuch aus, ansonsten loesen wir die Formel
	if (formula == "man") this->manual();
	else std::cout << "Ergebnis: " << this->solveFormula(formula) << std::endl;

	//Loeschen cin und die Formel
	std::cin.clear();
	formula.clear();
	
}


//Bekommt Formel als String übergeben und gibt Ergebnis zurueck
std::string calculator::solveFormula(std::string formula)
{
	//Wenn erstes Zeichen '[' ist -> Durchschnitt berechnen
	if (formula[0] == '[') formula = solveAverage(formula);

	//Wenn Klammern in der Formel existieren(höchste Priorität)...
	if (formula.find("(") < 10000) formula = solveFormula(solveBrackets(formula));

	//Uberpruefen ob letztes Zeichen in der Formen eine Zahl ist, faengt Fehler durch fehlerhafte Eingabe wie: '2+3+4+' ab
	if (!isdigit(formula[formula.size()-1])) return formula;

	//tmp_formula enthaelt keine Vorzeichen am Anfang der Formel, wird zum finden der Operatoren( + , - , * , / ) verwendet
	std::string tmp_formula = formula;
	tmp_formula.erase(0, 1);

	//Wenn eine Multiplikation oder eine Division in der Formel existiert(mittlere Priorität)...
	if (tmp_formula.find("*") < 10000 || tmp_formula.find("/") < 10000) formula = solveFormula(solveMulDiv(formula));
	tmp_formula = formula;
	tmp_formula.erase(0, 1);

	//Wenn eine Addition oder eine Subtraktion in der Formel existiert(niedrigste Priorität)... 
	if ((tmp_formula.find("+") < 10000 && tmp_formula.find("+") >= 0) || (tmp_formula.find("-") < 10000 && tmp_formula.find("-") >= 0)) formula = solveFormula(solveAddSub(formula));

	//"+" als Vorzeichen wird entfernt
	if (formula[0] == '+') formula.erase(0, 1);

	return formula;
}


//Wird aufgerufen, wenn "[" erstes Zeichen der Formel ist
std::string calculator::solveAverage(std::string formula)
{
	std::vector<double> values;

	//Entfernen '[' am anfang der Formel
	formula.erase(0, 1);

	//Solange ',' vorhanden ist..
	while (formula.find(",") > 0 && formula.find(",") < 10000)
	{
		//Casten Zahl zu double und schreiben sie in den vector values
		values.push_back(std::stof(formula.substr(0, formula.find(","))));
		//Entfernen die Zahl und das folgende ',' aus dem String
		formula.erase(0, formula.find(",")+1);
	}

	//Fuegen die Letzte Zahl zum vector hinzu
	values.push_back(std::stof(formula.substr(0, formula.find("]"))));

	//Rechnen den Durchschnitt aus und schreiben das Ergebnis in formula
	formula = std::to_string(calc->avg(values));

	return formula;
}


//Wird aufgerufen, wenn "(" in der Formel existiert
std::string calculator::solveBrackets(std::string formula)
{
	int posOpen = 0, posClose = 0;
	std::string tempResult;

	//Suchen Positionen von '(' und ')'
	posOpen = formula.find("(");
	posClose = formula.find(")");

	//Falls ")" nicht existiert, löschen wir "(" aus dem String
	if (posClose > 10000 || posClose < 0) { formula = formula.substr(0, posOpen) + formula.substr(posOpen + 1); return formula; }

	//Rufen mit Inhalt der Klammer unsere Funktion zum Lösen von Formeln auf
	tempResult = this->solveFormula(formula.substr(posOpen + 1, posClose - posOpen - 1));

	//Schneiden die Klammers+Inhalt aus dem String und ersetzten sie durch das Ergebnis der Klammer(Inhalt der Klammer wurde ausgerechnet)
	formula = formula.substr(0, posOpen) + tempResult + formula.substr(posClose + 1, formula.size());

	return formula;
}


//Wird aufgerufen, wenn "*" oder "/" in der Formel existieren
std::string calculator::solveMulDiv(std::string formula)
{
	int mulPos, divPos;

	//Suchen Positionen von '*' und '/'
	mulPos = formula.find("*");
	divPos = formula.find("/");

	//Wenn '*' existiert und '/' nicht -> rufen solveMul auf
	//Wenn '/' existiert und '/' nicht -> rufen solveDiv auf
	//Wenn beide existieren rufen wir das Zeichen mit der niedrigeren Position auf
	if (mulPos < 10000 && mulPos > 0 && mulPos < divPos || divPos < 0) formula = solveMul(formula);
	else formula = solveDiv(formula);


	return formula;
}


//Wird aufgerufen, wenn "+" oder "-" in der Formel existieren
std::string calculator::solveAddSub(std::string formula)
{
	int addPos, subPos;

	//Entfernen Vorzeichen an der ersten Position des Strings zum suchen nach Operatoren
	std::string tmp_formula = formula;
	tmp_formula.erase(0, 1);

	//Suchen Positionen von '+' und '-'
	addPos = tmp_formula.find("+");
	subPos = tmp_formula.find("-");

	//Wenn '+' existiert und '-' nicht -> rufen solveAdd auf
	//Wenn '-' existiert und '+' nicht -> rufen solveSub auf
	//Wenn beide existieren rufen wir das Zeichen mit der niedrigeren Position auf
	if (addPos < 10000 && addPos > 0 && addPos < subPos || subPos < 0) formula = this->solveAdd(formula);
	else formula = this->solveSub(formula);


	return formula;
}


//Wird aufgerufen fuer Addition(von "solveAddSub()")
std::string calculator::solveAdd(std::string formula)
{
	int addPos;
	double summandA, summandB;
	std::string sum;

	//Wenn '+' als Vorzeichen am Anfang des String existiert, wird es entfernt
	if (formula[0] == '+') formula.erase(0, 1);

	//Suchen Position von "+"
	addPos = formula.find("+");

	//Casten String von Anfang bis "+" zu double
	summandA = std::stof(formula.substr(0, addPos));

	//Entfernen summandA sowie "+" aus dem String(z.B. aus "4+5+6" wird "5+6" und 4 steht in summandA)
	formula.erase(0, addPos + 1);


	int i = 0;

	//Suchen naechstes Zeichen im String, das keine Zahl und kein "." ist.. Position dieses Zeichens steht in i
	do
	{
		i++;
		while (isdigit(formula[i]) != 0)
		{
			i++;
		}
		if (i >= formula.length()) break;
	} while (formula.substr(i, 1) == ".");

	//Casten String von Anfang bis Zeichen an pos i zu double
	summandB = std::stof(formula.substr(0, i));

	//Schneiden summandB aus dem String(z.B. aus "4+5+6" wird "+6", 4 steht in summandA und 5 in summandB)
	formula.erase(0, i);

	//Fuehren die Addition durch und Casten das Ergebnis anschließend in String
	sum = std::to_string(calc->add(summandA, summandB));

	//Fuegen das Ergebnis zum urspruenglichen String hinzu(z.B. aus "4+5+6" wird "9+6")
	formula = sum + formula;

	//Geben den neuen String zurueck
	return formula;
}


//Wird aufgerufen fuer Subtraktion(von "solveAddSub()")
std::string calculator::solveSub(std::string formula)
{
	int subPos;
	double summandA, summandB;
	std::string sum;

	//Entfernen Vorzeichen am Anfang der Formel zum suchen des Operanten
	std::string tmp_formula = formula;
	tmp_formula.erase(0, 1);

	//Suchen Position von Operand "+"
	subPos = tmp_formula.find("-");
	//Casten String von Anfang bis "-" zu double
	summandA = std::stof(formula.substr(0, subPos + 1));
	//Entfernen summandA sowie "+" aus dem String(z.B. aus "4+5+6" wird "5+6" und 4 steht in summandA)
	formula.erase(0, subPos + 2);

	int i = 0;

	//Suchen naechstes Zeichen im String, das keine Zahl und kein "." ist.. Position dieses Zeichens steht in i
	do
	{
		i++;
		while (isdigit(formula[i]) != 0)
		{
			i++;
		}
		if (i >= formula.length()) break;
	} while (formula.substr(i, 1) == ".");

	//Casten String von Anfang bis Zeichen an pos i zu double
	summandB = std::stof(formula.substr(0, i));

	//Schneiden summandB aus dem String(z.B. aus "4+5+6" wird "+6", 4 steht in summandA und 5 in summandB)
	formula.erase(0, i);

	//Fuehren die Addition durch und Casten das Ergebnis anschließend in String
	sum = std::to_string(calc->sub(summandA, summandB));

	//Fuegen das Ergebnis zum urspruenglichen String hinzu(z.B. aus "4+5+6" wird "9+6")
	formula = sum + formula;

	//Geben den neuen String zurueck
	return formula;
}


//Wird aufgerufen fuer Multiplikation(von "solveMulDiv()")
std::string calculator::solveMul(std::string formula)
{
	int mulPos;
	double faktorA, faktorB;
	std::string produkt;

	//Suchen Position von '*'
	mulPos = formula.find("*");

	//Initialisieren counter an Position von '*'
	int start = mulPos;

	//Gehen den String schrittweiße durch(Richtung Anfang des Strings) bis ein Operator oder der Anfang des Strings gefunden wurde
	do
	{
		start--;
		if (start <= 0) break;
		while (isdigit(formula[start]) != 0)
		{
			start--;
			if (start <= 0) break;
		}
		if (start <= 0) break;

	} while (formula.substr(start, 1) == ".");

	//Lesen den Faktor links des '*'-Zeichens ein(von der Position des naechsten Operators links des '*' bis zum '*')
	faktorA = std::stof(formula.substr(start, mulPos - start));

	//Intialisieren counter an Position von '*'+1
	int end = mulPos + 1;

	//Gehen den String schrittweiße durch(Richtung Ende des Strings) bis ein Operator oder das Ende des Strings gefunden wurde
	do
	{
		if (end >= formula.length()) break;
		end++;
		if (end >= formula.length()) break;
		while (isdigit(formula[end]) != 0)
		{
			end++;
		}
		if (end >= formula.length()) break;
	} while (formula.substr(end, 1) == ".");

	//Lesen den Faktor rechts des '*'-Zeichens ein(von der Position des naechsten Operators rechts des '*' bis zum '*'+1)
	faktorB = std::stof(formula.substr(mulPos + 1, end - mulPos));

	//Multiplizieren die beiden Faktoren miteinander
	produkt = std::to_string(calc->mul(faktorA, faktorB));

	//Wenn das Ergebnis positiv ist schreiben wir ein '+' vor das Ergebnis
	if (std::stof(produkt) >= 0) formula = formula.substr(0, start) + "+" + produkt + formula.substr(end, formula.size());
	else formula = formula.substr(0, start) + produkt + formula.substr(end, formula.size());

	//Wenn 2x '-' am Anfang der Formel steht loeschen wir beide '-'
	if (formula.substr(0, 1) == "-" && formula.substr(1, 1) == "-") formula.erase(0, 2);

	return formula;
}


//Wird aufgerufen fuer Division(von "solveMulDiv()")
std::string calculator::solveDiv(std::string formula)
{
	int divPos;
	double dividend, divisor;
	std::string quotient;

	//Suchen Position von '/'
	divPos = formula.find("/");

	//Initialisieren Counter an Position des '/'-1
	int start = divPos - 1;

	//Gehen den String schrittweiße durch(Richtung Anfang des Strings) bis ein Operator oder der Anfang des Strings gefunden wurde
	do
	{
		start--;
		if (start <= 0) break;
		while (isdigit(formula[start]) != 0)
		{
			start--;
		}
		if (start <= 0) break;

	} while (formula.substr(start, 1) == ".");
	if (start != 0)start++;

	//Lesen den Dividend links des '/'-Zeichens ein(von der Position des naechsten Operators links des '/' bis zum '/')
	dividend = std::stof(formula.substr(start, divPos - start));

	//Initialisieren Counter an Position des '/'+1
	int end = divPos + 1;

	//Gehen den String schrittweiße durch(Richtung Ende des Strings) bis ein Operator oder das Ende des Strings gefunden wurde
	do
	{
		end++;
		if (end >= formula.length()) break;
		while (isdigit(formula[end]) != 0)
		{
			end++;
		}
		if (end >= formula.length()) break;
	} while (formula.substr(end, 1) == ".");

	//Lesen den Divisor rechts des '/'-Zeichens ein(von der Position des naechsten Operators rechts des '/' bis zum '/')
	divisor = std::stof(formula.substr(divPos + 1, end - divPos));

	//Dividieren Dividend durch Divisor
	quotient = std::to_string(calc->div(dividend, divisor));

	//Fuegen das Ergebnis in die Formel ein
	formula = formula.substr(0, start) + quotient + formula.substr(end, formula.size());

	//Wenn 2x '-' am Anfang der Formel steht loeschen wir beide '-'
	if (formula.substr(0, 1) == "-" && formula.substr(1, 1) == "-") formula.erase(0, 2);

	return formula;
}


//Gibt Bedienungsanleitung auf dem Bildschirm aus
void calculator::manual()
{
	std::cout << "\nMANUAL: ";
	std::cout << "\nErlaubte Operationen: '+' '-' '*' '/' '()'";
	std::cout << "\nDezimalzahlen sind mit '.' zu trennen";
	std::cout << "\nAlle reellen Zahlen, sowie Brueche(in der Form 2/3) sind gestattet";
	std::cout << "\nNegative Zahlen mit einfachem '-' definieren";
	std::cout << "\nFehlerhafte Klammern(eine der Klammern vergessen) werden ignoriert und fuehren nicht zum Abbruch des Programms!";
	std::cout << "\nIn der gesamten Formel sind keine Leerzeichen gestattet!";
	std::cout << "\nBeispiel-Formel: 2+-3*0.5+(3*4/2)*1/2";
	std::cout << "\nZusaetzlich ist es moeglich den Durchschnitt einer Menge von Zahlen zu berechnen:";
	std::cout << "\n[1,2,3,4,5,6] berechnet den Durchschnitt aller Zahlen in den Klammern und gibt ihn aus";
	std::cout << "\nDiese Berechnung kann nicht innerhalb einer Formel benutzt werden, sondern nur eigenstaendig!";
	std::cout << "\nHierbei sind ebenfalls keine Leerzeichen oder andere Zeichen(ausser Zahlen und ',') erlaubt.\n";
}


//DEBUG:
/*
//Bekommt Formel als String übergeben und gibt Ergebnis zurueck
std::string calculator::solveFormula(std::string formula)
{
	//tmp_formula enthaelt keine Vorzeichen am Anfang der Formel, wird zum finden der Operatoren( + , - , * , / ) verwendet
	std::string tmp_formula = formula;
	tmp_formula.erase(0, 1);

	//Wenn Klammern in der Formel existieren(höchste Priorität)...
	if (formula.find("(") < 10000) formula = solveFormula(solveBrackets(formula));
	tmp_formula = formula;
	tmp_formula.erase(0, 1);

	//Wenn eine Multiplikation oder eine Division in der Formel existiert(mittlere Priorität)...
	if (tmp_formula.find("*") < 10000 || tmp_formula.find("/") < 10000) formula = solveFormula(solveMulDiv(formula));
	tmp_formula = formula;
	tmp_formula.erase(0, 1);

	//Wenn eine Addition oder eine Subtraktion in der Formel existiert(niedrigste Priorität)... 
	if ((tmp_formula.find("+") < 10000 && tmp_formula.find("+") >= 0) || (tmp_formula.find("-") < 10000 && tmp_formula.find("-") >= 0)) formula = solveFormula(solveAddSub(formula));

	//"+" als Vorzeichen wird entfernt
	if (formula[0] == '+') formula.erase(0, 1);
	return formula;
}

//Wird aufgerufen, wenn "(" in der Formel existiert
std::string calculator::solveBrackets(std::string formula)
{
	std::cout << "\n BRACKET-SOLVE:";
	int posOpen = 0, posClose = 0;
	std::string tempResult;

	posOpen = formula.find("(");
	posClose = formula.find(")");

	//Falls ")" nicht existiert, löschen wir "(" aus dem String
	if (posClose > 10000 || posClose < 0) { formula = formula.substr(0, posOpen) + formula.substr(posOpen+1); return formula; }

	std::cout << "\nFormula before solve: " << formula;
	std::cout << "\nInside the Bracket: " << formula.substr(posOpen + 1, posClose - posOpen - 1);
	//Rufen mit Inhalt der Klammer unsere Funktion zum Lösen von Formeln auf
	tempResult = this->solveFormula(formula.substr(posOpen+1, posClose - posOpen - 1));
	std::cout << "\nposClose: " << posClose;
	std::cout << "\nTempResult: " << tempResult;
	std::cout << "\nFormula before Bracket-cut: " << formula;
	//Schneiden die Klammers+Inhalt aus dem String und ersetzten sie durch das Ergebnis der Klammer(Inhalt der Klammer wurde ausgerechnet)
	formula = formula.substr(0, posOpen) + tempResult + formula.substr(posClose + 1, formula.size());
	std::cout << "\nFormula after Bracket-cut: " << formula;
	std::cout << std::endl;

	return formula;
}


//Wird aufgerufen, wenn "*" oder "/" in der Formel existieren
std::string calculator::solveMulDiv(std::string formula)
{
	int mulPos, divPos;

	mulPos = formula.find("*");
	divPos = formula.find("/");

	if (mulPos < 10000 && mulPos > 0 && mulPos < divPos || divPos < 0) formula = solveMul(formula);
	else formula = solveDiv(formula);


	return formula;
}


//Wird aufgerufen, wenn "+" oder "-" in der Formel existieren
std::string calculator::solveAddSub(std::string formula)
{
	int addPos, subPos;

	std::string tmp_formula = formula;
	tmp_formula.erase(0, 1);

	addPos = tmp_formula.find("+");
	subPos = tmp_formula.find("-");
	std::cout << "\naddPos: " << addPos << "       subPos: " << subPos;
	
	if (addPos < 10000 && addPos > 0 && addPos < subPos || subPos < 0) formula = this->solveAdd(formula);
	else formula = this->solveSub(formula);

	return formula;
}


//Wird aufgerufen fuer Addition(von "solveAddSub()")
std::string calculator::solveAdd(std::string formula)
{
	std::cout << "\nADD-SOLVE: ";
	int addPos;
	double summandA, summandB;
	std::string sum;

	if (formula[0] == '+') formula.erase(0, 1);

	//Suchen Position von "+"
	addPos = formula.find("+");
	std::cout << "\nsummandA: " << formula.substr(0, addPos);
	//Casten String von Anfang bis "+" zu double
	summandA = std::stof(formula.substr(0, addPos));

	std::cout << "\nBefore erase: " << formula;
	//Entfernen summandA sowie "+" aus dem String(z.B. aus "4+5+6" wird "5+6" und 4 steht in summandA)
	formula.erase(0, addPos+1);
	std::cout << "\nAfter erase: " << formula;

	std::locale loc;
	int i = 0;

	//Suchen naechstes Zeichen im String, dass keine Zahl und kein "." ist.. Position dieses Zeichens steht in i
	do
	{
		i++;
		while (isdigit(formula[i]) != 0)
		{
			i++;
		}
		if (i >= formula.length()) break;
	} while (formula.substr(i, 1) == ".");

	//Casten String von Anfang bis Zeichen an pos i zu double
	summandB = std::stof(formula.substr(0, i));

	//Schneiden summandB aus dem String(z.B. aus "4+5+6" wird "+6", 4 steht in summandA und 5 in summandB)
	formula.erase(0, i);
	std::cout << "\nFinal-Erase pos: " << i;
	std::cout << "\nAfter final erase: " << formula << std::endl;

	//Fuehren die Addition durch und Casten das Ergebnis anschließend in String
	sum = std::to_string(calc->add(summandA, summandB));

	//Fuegen das Ergebnis zum urspruenglichen String hinzu(z.B. aus "4+5+6" wird "9+6")
	formula = sum + formula;

	//Geben den neuen String zurueck
	return formula;
}


//Wird aufgerufen fuer Subtraktion(von "solveAddSub()")
std::string calculator::solveSub(std::string formula)
{
	std::cout << "\nSUB-SOLVE: ";
	int subPos;
	double summandA, summandB;
	std::string sum, tmp_formula = formula;

	tmp_formula.erase(0, 1);

	//Suchen Position von Operand "+"
	subPos = tmp_formula.find("-");
	std::cout << "\nsubPos: " << subPos;
	//Casten String von Anfang bis "-" zu double
	summandA = std::stof(formula.substr(0, subPos+1));
	std::cout << "\nsummandA: " << summandA;
	std::cout << "\nBefore erase: " << formula;
	//Entfernen summandA sowie "+" aus dem String(z.B. aus "4+5+6" wird "5+6" und 4 steht in summandA)
	formula.erase(0, subPos + 2);
	std::cout << "\nAfter erase: " << formula;

	std::locale loc;
	int i = 0;

	//Suchen naechstes Zeichen im String, dass keine Zahl und kein "." ist.. Position dieses Zeichens steht in i
	do
	{
		i++;
		while (isdigit(formula[i]) != 0)
		{
			i++;
		}
		if (i >= formula.length()) break;
	} while (formula.substr(i, 1) == ".");

	//Casten String von Anfang bis Zeichen an pos i zu double
	summandB = std::stof(formula.substr(0, i));

	//Schneiden summandB aus dem String(z.B. aus "4+5+6" wird "+6", 4 steht in summandA und 5 in summandB)
	formula.erase(0, i);
	std::cout << "\nFinal-Erase pos: " << i;
	std::cout << "\nAfter final erase: " << formula << std::endl;

	//Fuehren die Addition durch und Casten das Ergebnis anschließend in String
	sum = std::to_string(calc->sub(summandA, summandB));

	//Fuegen das Ergebnis zum urspruenglichen String hinzu(z.B. aus "4+5+6" wird "9+6")
	formula = sum + formula;
	
	//Geben den neuen String zurueck
	return formula;
}


//Wird aufgerufen fuer Multiplikation(von "solveMulDiv()")
std::string calculator::solveMul(std::string formula)
{
	std::cout << "\nMUL-SOLVE: ";
	int mulPos;
	double faktorA, faktorB;
	std::string produkt;

	mulPos = formula.find("*");

	int start = mulPos;

	do
	{
		start--;
		if (start <= 0) break;
		while (isdigit(formula[start]) != 0)
		{
			start--;
			if (start <= 0) break;
		}
		if (start <= 0) break;
		std::cout << "substr(start, 1): " << formula.substr(start, 1);
	} while (formula.substr(start, 1) == ".");

	faktorA = std::stof(formula.substr(start, mulPos - start));
	
	int end = mulPos+1;
	do
	{
		if (end >= formula.length()) break;
		end++;
		if (end >= formula.length()) break;
		while (isdigit(formula[end]) != 0)
		{
			end++;
		}
		if (end >= formula.length()) break;
	} while (formula.substr(end, 1) == ".");

	std::cout << "\nmulPos: " << mulPos << "     end: " << end;
	std::cout << "\nfaktorB: " << formula.substr(mulPos+1, end - mulPos);
	faktorB = std::stof(formula.substr(mulPos+1, end - mulPos));

	produkt = std::to_string(calc->mul(faktorA, faktorB));

	std::cout << "\nFormula before cut: " << formula;
	std::cout << "\nsubstr(0, start): " << formula.substr(0, start);
	std::cout << "\nprodukt:" << produkt;
	std::cout << "\nend: " << end << "    formula.size(): " << formula.size();
	std::cout << "\nsubstr(end, formula.size()): " << formula.substr(end, formula.size());
	std::cout << "\nformula.size(): " << formula.size();
	if(std::stof(produkt) >= 0) formula = formula.substr(0, start) + "+" + produkt + formula.substr(end, formula.size());
	else formula = formula.substr(0, start) + produkt + formula.substr(end, formula.size());
	std::cout << "\nFormula after cut: " << formula << std::endl;

	if (formula.substr(0, 1) == "-" && formula.substr(1, 1) == "-") formula.erase(0, 2);

	return formula;
}


//Wird aufgerufen fuer Division(von "solveMulDiv()")
std::string calculator::solveDiv(std::string formula)
{
	std::cout << "\nDIV-SOLVE: ";
	int divPos;
	double dividend, divisor;
	std::string quotient;

	divPos = formula.find("/");

	int start = divPos -1;

	do
	{
		start--;
		if (start <= 0) break;
		while (isdigit(formula[start]) != 0)
		{
			start--;
		}
		if (start <= 0) break;

		std::cout << "\nformula.substr(start, 1): " << formula.substr(start, 1);
	} while (formula.substr(start, 1) == ".");
	if(start != 0)start++;
	std::cout << "\nstart: " << start << "     divPos:" << divPos;
	dividend = std::stof(formula.substr(start, divPos - start));
	std::cout << "\nDividend: " << dividend;

	int end = divPos+1;
	do
	{
		end++;
		if (end >= formula.length()) break;
		while (isdigit(formula[end]) != 0)
		{
			end++;
		}
		if (end >= formula.length()) break;
	} while (formula.substr(end, 1) == ".");

	std::cout << "\ndivPos: " << divPos << "        end: " << end;
	std::cout << "\ndivisor: " << formula.substr(divPos + 1, end - divPos + 1);

	divisor = std::stof(formula.substr(divPos + 1, end - divPos));

	quotient = std::to_string(calc->div(dividend, divisor));

	std::cout << "\nFormula before cut: " << formula;
	std::cout << "\nsubstr(0, start): " << formula.substr(0, start);
	std::cout << "\nQuotient::" << quotient;
	std::cout << "\nsubstr(end, formula.size()): " << formula.substr(end, formula.size());
	std::cout << "\nformula.size(): " << formula.size();
	formula = formula.substr(0, start) + quotient + formula.substr(end, formula.size());
	std::cout << "\nFormula after cut: " << formula << std::endl;

	if (formula.substr(0, 1) == "-" && formula.substr(1, 1) == "-") formula.erase(0, 2);

	return formula;
}
*/