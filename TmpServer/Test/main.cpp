#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;
using namespace std;

int main(int argc, char* argv[]) 
{
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::acceptor acceptor(io_context);  
	cout << "asdf" << endl;
}