#pragma once
#include <thread>
#include <iostream>

class Counter
{
public:
	Counter();
	~Counter();
	
	void run();

	void count();

	bool getStatus();
	void setStatus(bool);
	//Setzt threadStatus auf false
	void stopThread();

private:

	//Wenn available = true -> Thread im Leerlauf.. Wenn available = false -> Thread arbeitet an xyz
	volatile bool available;
	//Wird auf false gesetzt wenn Thread beendet werden soll
	volatile bool threadStatus;
};

