#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

void sendMessage(char* ip, char* port, char* message)
{
	boost::asio::io_service io_service;
    
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), ip, port);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);
    
	//Senden message an Server.. 20 = length, ggf. dynamisch setzen
    boost::asio::write(s, boost::asio::buffer(message, 20));
    
	//Lesen Antwort
    boost::asio::read(s, boost::asio::buffer(message, 20));
}

int main(int argc, char* argv[])
{
	if (argc != 3)
    {
      std::cerr << "Wrong usage, run <Server-IP> <Server-Port>\n";
      return 1;
    }
	
	char message[20];
	
	std::cout << "Bitte geben Sie Ihre Nachricht ein:\n";
	std::cin >> message;
	
	//Nachricht senden
	sendMessage(argv[1], argv[2], message);
	
	//Antwort ausgeben
	std::cout << "Antwort: " << message << std::endl;

}