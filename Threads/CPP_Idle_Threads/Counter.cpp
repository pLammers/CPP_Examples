#include "Counter.h"



Counter::Counter()
{
	available = true;
	threadStatus = true;
}


Counter::~Counter()
{
}


void Counter::run()
{
	this->available = true;
	
	//"Endlosschleife" bis available in main geaendert wird
	while (this->available) 
	{
		Sleep(5);
	}

	//Beenden Thread, threadStatus wird in main gesetzt
	if (!threadStatus) return;

	//std::cout << "\nstd::this_thread::get_id()\n";
	std::cout << "\nINTERRUPTED!!!\n";

	this->count();

	//rekursiver aufruf
	this->run();
}


void Counter::count()
{
	for (int i = 0; i < 10; i++) std::cout << i << std::endl;
}


bool Counter::getStatus()
{
	return this->available;
}


void Counter::setStatus(bool status)
{
	this->available = status;
}


void Counter::stopThread()
{
	this->threadStatus = false;
}