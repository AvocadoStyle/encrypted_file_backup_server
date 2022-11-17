#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "FileUtilities.h"
#include "FileHandler.h"
#include <string>
#include "ClientController.h"

using boost::asio::ip::tcp;
int main() {
	ClientController* client_controller = new ClientController();
	client_controller->registration();





	//FileUtilities *file_utilities = new FileUtilities();
	//FileHandler* file_handler = new FileHandler();
	//std::string address = file_handler->read_address_from_file();
	//std::string port = file_handler->read_address_from_file();
	//std::cout <<"address" << address << std::endl;
	//std::cout << "port" << port << std::endl;



	//std::cout << st << std::endl;



	//std::cout << "[start the client...]" << std::endl;
	//const int max_len = 1024;
	//char address[] = "127.0.0.1";
	//char port[] = "8085";
	//boost::asio::io_context io_context;
	//tcp::socket s(io_context);
	//tcp::resolver resolver(io_context);
	//boost::asio::connect(s, resolver.resolve(address, port));
	////char request[max_len] = "hey";
	////char request[max_len] = "hey\x00";
	//char request[max_len] = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x01\x04\x4C\x00\x00\x00\x05\EDEN\x00";
	//////std::cout << "Enter a message: ";
	//////std::cin.getline(request, max_len);
	//boost::asio::write(s, boost::asio::buffer(request, max_len));
}