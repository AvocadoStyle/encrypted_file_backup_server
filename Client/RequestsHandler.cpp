#include "RequestsHandler.h"

RequestsHandler::RequestsHandler() {

}

RequestsHandler::~RequestsHandler() {

}
/*
 * take care of the registration request - build a message to send to the server
 * taking care about header request
 * client id: will initialize it, the server will give it as response
 * version: will not take care in the registration request
 * code: will take care
 * payload size: will contain the size of the payload NAME
 * payload: NAME
 */
void RequestsHandler::registration_request_handle(std::string name) {
	// set client id default
	this->__set_client_id_default();

	// set version
	this->__set_version_id_default();

	// set code for registration
	int reg_request_number_code = __REGISTRATION__;
	memcpy(this->code, (uint8_t*)&reg_request_number_code, sizeof(this->code));

	// set payload size
	int name_size_number = __NAME_SIZE__;
	memcpy(this->payload_size, (uint8_t*)&name_size_number, sizeof(this->payload_size));
	this->payload = (uint8_t*)malloc(255);
	int* v = (int*)malloc(sizeof(int) * 20);
	//const char* name_c = name.c_str();
	char* ss = const_cast<char*>(name.c_str());
	//memcpy(this->payload, (uint8_t*)&(ss), 255);
	for (int i = 0; i < 255; i++) {
		this->payload[i] = (uint8_t)ss[i];
	}

	// merge together the message
	this->total_size = __CLIENT_ID_SIZE__ + __VERSION_SIZE__ + __CODE_SIZE__ + __PAYLOAD_SIZE_SIZE__ + __NAME_SIZE__;
	this->header_size = __HEADER_SIZE__;
	this->header_message = (uint8_t*)malloc(sizeof(uint8_t) * __HEADER_SIZE__);
	this->build_message = (uint8_t*)malloc(this->total_size);
	int *start;
	start = (int*)malloc(sizeof(int));
	*start = 0;
	this->__set_build_message(start, this->client_id, __CLIENT_ID_SIZE__);
	this->__set_build_message(start, this->version, __VERSION_SIZE__);
	this->__set_build_message(start, this->code, __CODE_SIZE__);
	this->__set_build_message(start, this->payload_size, __PAYLOAD_SIZE_SIZE__);
	this->__set_build_message(start, this->payload, __NAME_SIZE__);
}

/* handle the authentication request and build entire message to send to the server 
 * take of the header request and for the payload including:
 * header:
 * client_id:	will be placed from the previous registration request.
 * version:		will set to default value.
 * code:		will set the code value as expected for the authentication request.
 * payload size: will contain the payload size - from the payload: name, public_key
 * payload:
 * name:		contain the name of the user that previously registered.
 * public_key	contain the public_key that previous registered and will be generated from the private key 
 * that will supplied as argument.
 */
void RequestsHandler::authentication_request_handle(uint8_t client_id[], uint8_t name[], uint8_t* private_key) {

}

/* generates the rsa key public.
 *
 */
void RequestsHandler::generate_rsa_public_key() {
	this->public_key = "abcd";
}
/* generates the rsa key private.
 *
 */
void RequestsHandler::generate_rsa_private_key() {
	this->private_key = "1234";
}

/*
 * set the final build_meesage to be send to the server
 */
void RequestsHandler::__set_build_message(int* start, uint8_t* section, int size_to_add) {
	int start_val = *start;
	for (int i = start_val, j=0; i < size_to_add + start_val; i++, j++) {
		build_message[i] = section[j];
	}
	*start = *start + size_to_add;
}

/*
 * set the client_id to be a default value to send to the server, the server will generate the client id.
 */
void RequestsHandler::__set_client_id_default() {
	for (int i = 0; i < __CLIENT_ID_SIZE__; i++) {
		this->client_id[i] = '\x00';
	}
}
/*
 * set the version to be a default value to send to the server, the server will generate the client id.
 */
void RequestsHandler::__set_version_id_default() {
	for (int i = 0; i < __VERSION_SIZE__; i++) {
		this->version[i] = '\x00';
	}
}