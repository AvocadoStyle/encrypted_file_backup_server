#ifndef __CLIENTCONTROLLER_H__
#define __CLIENTCONTROLLER_H__
#include <iostream>
#include <string>
#include "SocketHandler.h"
#include "RequestsHandler.h"
#include "ResponseHandler.h"
#include "FileHandler.h"
#include "FileUtilities.h"


class ClientController {
public:
	std::string port;
	std::string address;
	std::string name;
	std::string client_id;
	std::string private_key_base64;
	std::string file_name;
	SocketHandler *s;
	RequestsHandler *req_handler;
	ResponseHandler *res_handler;
	FileHandler* file_handler;
	FileUtilities* file_utilities;

	ClientController();
	~ClientController();

	void initialize();
	bool registration();
	bool authentication();
	bool send_encrypted_file();
};










#endif