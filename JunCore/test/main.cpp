#include <boost/asio.hpp>
#include <iostream>
#include "packet.pb.h"

using namespace std;

int main()
{
	boost::asio::io_context io_context;

	cout << "111" << endl;

	PacketLib::UG_ECHO_REQ a;
	a.set_echo("222");
	cout << a.echo() << endl;
}