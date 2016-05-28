#include "tcp_server.h"


int main() {

	std::cout << "TestServer running...\n";

    tcp_server server;
    
    try
  {
    boost::asio::io_service io_service;
    server.server(io_service);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

