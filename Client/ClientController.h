#ifndef __CLIENTCONTROLLER_H__
#define __CLIENTCONTROLLER_H__
#include <iostream>
#include <string>
#include "SocketHandler.h"
#include "RequestsHandler.h"
#include "FileHandler.h"
#include "FileUtilities.h"

class ClientController {
public:
	std::string port;
	std::string address;
	std::string name;
	std::string file_name;
	SocketHandler *s;
	RequestHandler *req_handler;
	FileHandler* file_handler;
	FileUtilities* file_utilities;

	ClientController();
	~ClientController();

	void initialize();
	bool registration(char* name);


};










#endif