#include "ClientController.h"


ClientController::ClientController() {
	this->port = {0};
	this->address = {0};
	this->s = new SocketHandler();
	this->req_handler = new RequestHandler();
	this->file_handler = new FileHandler();
	this->file_utilities = new FileUtilities();
	initialize();
}

ClientController::~ClientController() {
	delete s;
	delete req_handler;
	delete file_handler;
	this->address =		nullptr;
	this->port	  =		nullptr;
}

void ClientController::initialize() {
	this->address      = this->file_handler->read_address_from_file();
	this->port	       = this->file_handler->read_port_from_file();
	this->name		   = this->file_handler->read_name_from_file();
	this->file_name    = this->file_handler->read_file_path_from_file();
}


bool ClientController::registration(char *name){
	s->connect(this->address, this->port);
	uint8_t request[1024] = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x01\x04\x4C\x00\x00\x00\x05\EDEN\x00";
	//uint8_t request[1024] = "hey";
	s->send_msg(request, 1024);
	return true;
}











