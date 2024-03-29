#include "ClientController.h"


ClientController::ClientController() {
	this->port = {0};
	this->address = {0};
	this->s = new SocketHandler();
	this->req_handler = new RequestsHandler();
	this->res_handler = new ResponseHandler(this->req_handler);
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
	this->address      = this->file_handler->read_address_from_file_info();
	this->port	       = this->file_handler->read_port_from_file_info();
	this->file_name    = this->file_handler->read_file_path_from_file_info();

	// will read from the registration info file and initialize all of the Request Handler object 
	if (this->file_handler->is_client_register_info_exists()){
		this->name = this->file_handler->read_name_from_register_file_info();
		this->client_id = this->file_handler->read_client_id_from_register_file_info();
		this->private_key_base64 = this->file_handler->read_private_key_64_from_register_file_info();
		this->req_handler->initialize_fields_from_register_file_info();
	} else {
		this->name = this->file_handler->read_name_from_file_info();
	}
}

/* registration controll
 * if the client already registered it will not register again
 * if the client is not registered it will nagotiate with the server to register the client accordingly to the 
 * info file that the client own.
 * :return boolean: will return true if the client is registered in any way or if the client is registered in the past.
 * will return false if the registration not succed or throw an exception.
 * :exception: will be throwned if the client registration not succed due to any issue in the action of registration.
 */
bool ClientController::registration(){
	/* check if there is past registration of the clientand will return true if does. */
	if (this->file_handler->is_client_register_info_exists()) {
		return true;
	}

	/*                 send message with name                      */
	s->connect(this->address, this->port);
	// build the protocol payload
	this->req_handler->registration_request_handle(this->name);
	// will send header size 23
	s->send_msg(this->req_handler->build_message, this->req_handler->header_size); 
	// will send all the message
	s->send_msg(this->req_handler->build_message, this->req_handler->total_size);


	/*                 receive response message of client_id                      */ 
	//receive response message header
	uint8_t* receive_buffer_header_response;
	size_t header_response_size = this->res_handler->HEADER_SIZE_PATTERN;
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * header_response_size);
	s->recv_msg(receive_buffer_header_response,
		header_response_size);
	// parse the response header message
	this->res_handler->init_response_header(receive_buffer_header_response);
	// receive response message header+payload
	//uint8_t* receive_buffer_payload_response;
	size_t payload_response_size = *this->res_handler->int_payload_size; // the size of the response payload
	// initialize the buffer to be the header + the payload size
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * (header_response_size + payload_response_size));
	// gets the response header+payload from the server
	s->recv_msg(receive_buffer_header_response,
		(header_response_size + payload_response_size));
	// parse the response header+payload message from the buffer that received from the server
	this->res_handler->response_code_handler(receive_buffer_header_response);

	/*         creation private key                           */		
	this->req_handler->generate_rsa_private_key();

	/*  create client info file including name, client_id ascii hex presentation, and private key in base64    */
	//name from the file info, 
	// will set the client_id bytes to string hex to insert it in the future to the registration info file
	this->res_handler->set_client_id_str_hex();
	std::string client_id_st = this->res_handler->client_id_st_hex;
	std::string private_key_st = this->req_handler->private_key;
	this->file_handler->write_registration_info_file(this->name, client_id_st, private_key_st);
	s->disconnect();
	return true;
}

/* authentication controll
 * public_key: generate and send the generated public key in the payload (after generated in the register stage the private key and plented it inside the info registration file).
 * name: send the name in the payload.
 */
bool ClientController::authentication() {
	// if the info file is not exists in this stage we'll throw an error or return false and handle it in another scenario
	if (!this->file_handler->is_client_register_info_exists()) {
		return false;
	}
	
	/*                 send message with name and public key                      */
	 //initialize will set the values as represented in the file with string:
	// name, client_id, private_key 
	this->initialize();
	s->connect(this->address, this->port);

	// handle the data from the registration file
	this->req_handler->authentication_request_handle();

	// send data
	// will send header size
	s->send_msg(this->req_handler->build_message, this->req_handler->header_size);
	// will send all of the message
	s->send_msg(this->req_handler->build_message, this->req_handler->total_size);

	/*                 receive response message of client NAME and AES encrypt key                      */
//receive response message header
	uint8_t* receive_buffer_header_response;
	size_t header_response_size = this->res_handler->HEADER_SIZE_PATTERN;
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * header_response_size);
	s->recv_msg(receive_buffer_header_response,
		header_response_size);
	// parse the response header message
	this->res_handler->init_response_header(receive_buffer_header_response);
	// receive response message header+payload
	//uint8_t* receive_buffer_payload_response;
	size_t payload_response_size = *this->res_handler->int_payload_size; // the size of the response payload
	// initialize the buffer to be the header + the payload size
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * (header_response_size + payload_response_size));
	// gets the response header+payload from the server
	s->recv_msg(receive_buffer_header_response,
		(header_response_size + payload_response_size));
	// parse the response header+payload message from the buffer that received from the server
	this->res_handler->response_code_handler(receive_buffer_header_response);
	s->disconnect();
	return true;
}

bool ClientController::send_encrypted_file() {

	// if the info file is not exists in this stage we'll throw an error or return false and handle it in another scenario
	if (!this->file_handler->is_client_register_info_exists()) {
		return false;
	}

	/*                 send message with name and public key                      */
	 //initialize will set the values as represented in the file with string:
	// name, client_id, private_key 
	this->initialize();
	s->connect(this->address, this->port);

	// handle the data from the registration file
	this->req_handler->send_file_request_handle();

	// send data
	// will send header size
	s->send_msg(this->req_handler->build_message, this->req_handler->header_size);
	// will send all of the message
	s->send_msg(this->req_handler->build_message, this->req_handler->total_size);






	/*                 receive response message of client NAME and AES encrypt key                      */
//receive response message header
	uint8_t* receive_buffer_header_response;
	size_t header_response_size = this->res_handler->HEADER_SIZE_PATTERN;
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * header_response_size);
	s->recv_msg(receive_buffer_header_response,
		header_response_size);
	// parse the response header message
	this->res_handler->init_response_header(receive_buffer_header_response);
	// receive response message header+payload
	//uint8_t* receive_buffer_payload_response;
	size_t payload_response_size = *this->res_handler->int_payload_size; // the size of the response payload
	// initialize the buffer to be the header + the payload size
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * (header_response_size + payload_response_size));
	// gets the response header+payload from the server
	s->recv_msg(receive_buffer_header_response,
		(payload_response_size));


	s->disconnect();
	/************ parse the response header + payload message from the buffer that received from the server ***********/
	bool crc_check_server_and_client = this->res_handler->response_code_handler(receive_buffer_header_response);
	return crc_check_server_and_client;
}

bool ClientController::crc_valid() {
	// if the info file is not exists in this stage we'll throw an error or return false and handle it in another scenario
	if (!this->file_handler->is_client_register_info_exists()) {
		return false;
	}

	/*                 send message with name and public key                      */
	 //initialize will set the values as represented in the file with string:
	// name, client_id, private_key 
	this->initialize();
	s->connect(this->address, this->port);

	// handle the data from the registration file
	this->req_handler->crc_valid();

	// send data
	// will send header size
	s->send_msg(this->req_handler->build_message, this->req_handler->header_size);
	// will send all of the message
	s->send_msg(this->req_handler->build_message, this->req_handler->total_size);

	/*                 receive response message of client NAME and AES encrypt key                      */
	//receive response message header
	uint8_t* receive_buffer_header_response;
	size_t header_response_size = this->res_handler->HEADER_SIZE_PATTERN;
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * header_response_size);
	s->recv_msg(receive_buffer_header_response,
		header_response_size);
	// parse the response header message
	this->res_handler->init_response_header(receive_buffer_header_response);
	// receive response message header+payload
	//uint8_t* receive_buffer_payload_response;
	size_t payload_response_size = *this->res_handler->int_payload_size; // the size of the response payload
	// initialize the buffer to be the header + the payload size
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * (header_response_size + payload_response_size));
	// gets the response header+payload from the server
	s->recv_msg(receive_buffer_header_response,
		(payload_response_size));
	// parse the response header+payload message from the buffer that received from the server
	this->res_handler->response_code_handler(receive_buffer_header_response);
	s->disconnect();
	return true;
}

bool ClientController::crc_not_valid() {
	// if the info file is not exists in this stage we'll throw an error or return false and handle it in another scenario
	if (!this->file_handler->is_client_register_info_exists()) {
		return false;
	}

	/*                 send message with name and public key                      */
	 //initialize will set the values as represented in the file with string:
	// name, client_id, private_key 
	this->initialize();
	s->connect(this->address, this->port);

	// handle the data from the registration file
	this->req_handler->crc_not_valid();

	// send data
	// will send header size
	s->send_msg(this->req_handler->build_message, this->req_handler->header_size);
	// will send all of the message
	s->send_msg(this->req_handler->build_message, this->req_handler->total_size);

	/*                 receive response message of client NAME and AES encrypt key                      */
	//receive response message header
	uint8_t* receive_buffer_header_response;
	size_t header_response_size = this->res_handler->HEADER_SIZE_PATTERN;
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * header_response_size);
	s->recv_msg(receive_buffer_header_response,
		header_response_size);
	// parse the response header message
	this->res_handler->init_response_header(receive_buffer_header_response);
	// receive response message header+payload
	//uint8_t* receive_buffer_payload_response;
	size_t payload_response_size = *this->res_handler->int_payload_size; // the size of the response payload
	// initialize the buffer to be the header + the payload size
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * (header_response_size + payload_response_size));
	// gets the response header+payload from the server
	s->recv_msg(receive_buffer_header_response,
		(payload_response_size));
	// parse the response header+payload message from the buffer that received from the server
	this->res_handler->response_code_handler(receive_buffer_header_response);
	s->disconnect();
	return true;
}

bool ClientController::crc_not_valid_at_all() {
	// if the info file is not exists in this stage we'll throw an error or return false and handle it in another scenario
	if (!this->file_handler->is_client_register_info_exists()) {
		return false;
	}

	/*                 send message with name and public key                      */
	 //initialize will set the values as represented in the file with string:
	// name, client_id, private_key 
	this->initialize();
	s->connect(this->address, this->port);

	// handle the data from the registration file
	this->req_handler->crc_not_valid_final();

	// send data
	// will send header size
	s->send_msg(this->req_handler->build_message, this->req_handler->header_size);
	// will send all of the message
	s->send_msg(this->req_handler->build_message, this->req_handler->total_size);

	/*                 receive response message of client NAME and AES encrypt key                      */
	//receive response message header
	uint8_t* receive_buffer_header_response;
	size_t header_response_size = this->res_handler->HEADER_SIZE_PATTERN;
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * header_response_size);
	s->recv_msg(receive_buffer_header_response,
		header_response_size);
	// parse the response header message
	this->res_handler->init_response_header(receive_buffer_header_response);
	// receive response message header+payload
	//uint8_t* receive_buffer_payload_response;
	size_t payload_response_size = *this->res_handler->int_payload_size; // the size of the response payload
	// initialize the buffer to be the header + the payload size
	receive_buffer_header_response = (uint8_t*)malloc(sizeof(uint8_t) * (header_response_size + payload_response_size));
	// gets the response header+payload from the server
	s->recv_msg(receive_buffer_header_response,
		(payload_response_size));
	// parse the response header+payload message from the buffer that received from the server
	this->res_handler->response_code_handler(receive_buffer_header_response);
	s->disconnect();
	return true;
}




