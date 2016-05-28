#include <cstdlib>
#include <iostream>
#include <exception>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;

class tcp_server
{
public:
typedef boost::shared_ptr<tcp::socket> socket_ptr;
    
tcp_server();
~tcp_server();

void server(boost::asio::io_service&);
    
private:
    
const static short port = 8888;						//Port auf den der Server hoert
const static int max_length = 1024;

void datahandler(char*)
void session(socket_ptr);
    
};