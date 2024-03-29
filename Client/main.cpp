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
	// 1100
	client_controller->registration();
	// 1101
	client_controller->authentication();
	// 1103
	int counter = 0;
	while (counter <= 3) {
		if (client_controller->send_encrypted_file()) {
			// 1104
			client_controller->crc_valid();
			std::cout << "finished, sent the file successfully" << std::endl;
			return 1;
		} else {
			// 1105
			std::cout << "cannot send the file properly - crc issue, try again" << std::endl;
			client_controller->crc_not_valid();
		}
		counter++;
	}
	// 1106
	std::cout << "aborting - file sent failure - crc" << std::endl;
	client_controller->crc_not_valid_at_all();
	return 0;
	
}