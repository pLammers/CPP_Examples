#include "tcp_server.h"


tcp_server::tcp_server()
{
	
}

tcp_server::~tcp_server()
{
    
}


void tcp_server::session(socket_ptr sock)
{
try
  {
    for (;;)
    {
      char data[max_length];

      boost::system::error_code error;
      size_t length = sock->read_some(boost::asio::buffer(data), error);
      

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.
	
	  datahandler(data);		//Handler wird mit den erhaltenen Daten aufgerufen, datahandler schreibt in data!
	  
	  //Antwort wird gesendet
      boost::asio::write(*sock, boost::asio::buffer(data, length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}


void tcp_server::server(boost::asio::io_service& io_service)
{
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    boost::thread t(boost::bind(&tcp_server::session,this , sock));
  }
 }
 
void tcp_server::datahandler(char* data)
{
    //Verabeite erhaltene Daten
	//...
	memset(data, '\0', sizeof(data));
	data[0] = 'H';
	data[1] = 'i';
	data[2] = '\0';
	
	//Inhalt von data wird nach Ende der Funktion als Antwort zurueckgesendet
}
