#include "Counter.h"
#include <thread>
#include <vector>


void staticObjectThreads()
{
	Counter countA;
	Counter countB;
	Counter countC;

	//std::thread *Threadname*(*function*, *object*)
	//& nur bei Nicht-Pointern! -> Durch & wird Addresse übergeben, bei Pointern wird ohne & automatisch Addresse übergeben
	std::thread tA(&Counter::count, &countA);
	std::thread tB(&Counter::count, &countB);
	std::thread tC(&Counter::count, &countC);
	
	//Starten der Threads
	tA.join();
	tB.join();
	tC.join();
}

void dynamicObjectThreads(int numberOfThreads)
{
	std::vector<std::thread*> threads;

	//Erstellen der Objekte+Threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		//Bei dynamischer Objekt-Erzeugung immer Pointer(*) benutzen in C++!!..nicht wie oben in staticObjectThreads()!
		Counter *count = new Counter();
		std::thread *t = new std::thread(&Counter::count, count);
		threads.push_back(t);
	}

	//Starten der Threads
	for (int n = 0; n < threads.size(); n++)
	{
		//Bei Pointern "->"
		threads[n]->join();
	}

}

void staticThreads()
{
	//std::thread *Threadname*(*function*)
	//Threads fuer lokale Funktionen-> kein &!
	std::thread tA(count);
	std::thread tB(count);
	std::thread tC(count);

	//Starten der Threads
	tA.join();
	tB.join();
	tC.join();
}

void dynamicThreads(int numberOfThreads)
{
	std::vector<std::thread*> threads;

	//Erstellen der Threads
	for (int i = 0; i < numberOfThreads; i++)
	{
		//Bei dynamischer Objekt-Erzeugung immer Pointer(*) benutzen in C++!!..nicht wie oben in staticThreads()!
		std::thread *t = new std::thread(count);
		threads.push_back(t);
	}

	//Starten der Threads
	for (int n = 0; n < threads.size(); n++)
	{
		//Bei Pointern "->"
		threads[n]->join();
	}
}

void count()
{
	for (int i = 0; i < 10; i++) std::cout << i << std::endl;
}

int main()
{
	staticThreads();
	//dynamicThreads(3);
	//staticObjectThreads();
	//dynamicObjectThreads(3);

	std::cin.get();

	return 0;
}