#include "ClientController.h"


ClientController::ClientController() {
	this->port = {0};
	this->address = {0};
	this->s = new SocketHandler();
	this->req_handler = new RequestsHandler();
	this->file_handler = new FileHandler();
	this->file_utilities = new FileUtilities();
	initialize();
}

ClientController::~ClientController() {
	delete s;
	delete req_handler;
	delete file_handler;
	this->address = nullptr;
	this->port	  =	nullptr;
}

void ClientController::initialize() {
	this->address      = this->file_handler->read_address_from_file();
	this->port	       = this->file_handler->read_port_from_file();
	this->name		   = this->file_handler->read_name_from_file();
	this->file_name    = this->file_handler->read_file_path_from_file();
}


bool ClientController::registration(){
	s->connect(this->address, this->port);
	// build the protocol payload
	this->req_handler->registration_request_handle(this->name);




	//s->send_msg(request, 1024);
	return true;
}











