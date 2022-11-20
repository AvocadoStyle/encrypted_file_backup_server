#include "ClientController.h"


ClientController::ClientController() {
	this->port = {0};
	this->address = {0};
	this->s = new SocketHandler();
	this->req_handler = new RequestsHandler();
	this->res_handler = new ResponseHandler();
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


	// will send header size 23
	s->send_msg(this->req_handler->build_message, this->req_handler->header_size); 
	// will send all the message
	s->send_msg(this->req_handler->build_message, this->req_handler->total_size);


	// receive message
	uint8_t* receive_buffer_header_response;
	size_t header_response_size = 7;
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * header_response_size);
	s->recv_msg(receive_buffer_header_response,
		header_response_size);

	this->res_handler->init_response_header(receive_buffer_header_response);
	uint8_t* receive_buffer_payload_response;
	size_t receive_buffer_payload_response = this->res_handler->int_payload_size;
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * header_response_size);
	s->recv_msg()
	this->res_handler->registration_response_handle();




	//s->send_msg(request, 1024);
	return true;
}











