#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void send_message() {
	const int max_len = 1024;
	char address[] = "127.0.0.1";
	char port[] = "8085";
	boost::asio::io_context io_context;
	tcp::socket s(io_context);
	tcp::resolver resolver(io_context);
	boost::asio::connect(s, resolver.resolve(address, port));
	char request[max_len];
	std::cout << "Enter a message: ";
	std::cin.getline(request, max_len);
	boost::asio::write(s, boost::asio::buffer(request, max_len));






}