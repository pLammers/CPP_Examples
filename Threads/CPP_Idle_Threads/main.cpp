#include "Counter.h"
#include <vector>

//Bereits in Counter.h included:
//Sleep for Windows
//#include <Windows.h>
//usleep for UNIX
//#include <unistd.h>


void GUIsingleThread(Counter *count)
{
	char check;

	do
	{
		std::cout << "Press 1 to Interrupt Idle Thread, 0 to close: ";
		std::cin >> check;
		//Unterbrechen Schleife im counter-objekt -> Leerlauf wird beendet
		if (check == '1') count->setStatus(false);
		Sleep(5);
		//usleep(5000);		//Fuer Linux, nicht getestet
	} while (check != '0');

	count->stopThread();
	count->setStatus(false);
}


void singleThread()
{
	Counter *countA = new Counter();;

	std::thread tA(&Counter::run, countA);
	std::thread tB(GUIsingleThread, countA);
	tA.join();
	tB.join();
}


void GUImultiThread(std::vector<Counter*> count)
{
	char check;

	do
	{
		std::cout << "Press 1 to Interrupt Idle Thread, 0 to close: ";
		std::cin >> check;
		if (check == '1')
		{
			//Unterbrechen Schleifen in allen Counter-Objekten -> Leerlauf wird beendet
			for (int n = 0; n < count.size(); n++)
			{
				count[n]->setStatus(false);
			}
		}
		Sleep(4*count.size());
		//usleep(0.04*count*size());		//Fuer Linux, nicht getestet
	} while (check != '0');

	//Stop all Threads
	for (int i = 0; i < count.size(); i++)
	{
		count[i]->stopThread();
		count[i]->setStatus(false);
	}
}


void multiThread(int numberOfThreads)
{
	std::vector<std::thread*> threads;
	std::vector<Counter*> counters;

	//Erstellen der Objekte+Threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		//Bei dynamischer Objekt-Erzeugung immer Pointer(*) benutzen in C++!!
		Counter *count = new Counter();
		counters.push_back(count);
		std::thread *t = new std::thread(&Counter::run, count);
		threads.push_back(t);
	}


	std::thread tB(GUImultiThread, counters);

	//Starten der Threads
	for (int n = 0; n < threads.size(); n++)
	{
		if (n == 0) tB.join();
		//Bei Pointern "->"
		threads[n]->join();
	}
}


int main()
{
	//Erstellt einen Thread, der idle startet und durch Benutzereingabe aus dem Leerlauf geht
	//singleThread();

	//Erstellt mehrere Threads, die alle idle starten und durch Benutzereigabe aus dem Leerlauf gehen
	multiThread(4);

	return 0;
}