#include "RequestsHandler.h"

RequestsHandler::RequestsHandler() {

}

RequestsHandler::~RequestsHandler() {

}
/*
 * take care of the registration request
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
	this->build_message = (uint8_t*)malloc(this->total_size);
	int start = 0;
	for (int i = start; i < __CLIENT_ID_SIZE__; i++) {
		build_message[i] = this->client_id[i];
	}
	start = start + __CLIENT_ID_SIZE__;
	for (int i = start, j=0; i < __VERSION_SIZE__+start; i++, j++) {
		build_message[i] = this->version[j];
	}
	start = start + __VERSION_SIZE__;
	for (int i = start, j = 0; i < __CODE_SIZE__ + start; i++, j++) {
		build_message[i] = this->code[j];
	}
	start = start + __CODE_SIZE__;
	for (int i = start, j = 0; i < __PAYLOAD_SIZE_SIZE__ + start; i++, j++) {
		build_message[i] = this->payload_size[j];
	}
	start = start + __PAYLOAD_SIZE_SIZE__;
	for (int i = start, j = 0; i < __NAME_SIZE__ + start; i++, j++) {
		build_message[i] = this->payload[j];
	}





}
/*
 * set the client_id to be a default value to send to the server, the server will generate the client id.
 */
void RequestsHandler::__set_client_id_default() {
	for (int i = 0; i < __CLIENT_ID_SIZE__; i++) {
		this->client_id[i] = '\x00';
	}
}

void RequestsHandler::__set_version_id_default() {
	for (int i = 0; i < __VERSION_SIZE__; i++) {
		this->version[i] = '\x00';
	}
}
