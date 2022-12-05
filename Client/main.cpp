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
	//client_controller->registration();
	//client_controller->authentication();
	client_controller->send_encrypted_file();

}