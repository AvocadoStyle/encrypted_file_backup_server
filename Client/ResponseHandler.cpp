#include "ResponseHandler.h"



ResponseHandler::ResponseHandler() {

}
ResponseHandler::~ResponseHandler() {

}

void ResponseHandler::registration_response_handle(uint8_t* buffer_header_and_payload) {
	int* start;
	start = (int*)malloc(sizeof(int));
	// payload starting from
	*start = this->payload_start;
	// setting the client_id from the buffer
	this->__set_message(start, this->client_id, buffer_header_and_payload, __CLIENT_ID_SIZE__);
}

void ResponseHandler::response_code_handler(uint8_t* buffer_header_and_payload) {
	int CODE = *this->int_code;
	if (CODE == __REGISTRATION_RES__) {
		this->registration_response_handle(buffer_header_and_payload);
	} else if(CODE == __REGISTRATION_FAILED_RES__){

	} else if(CODE == __PRIMARY_KEY_ACCEPTED_RES__){

	} else if(CODE == __CRC_VALID_RES__){

	} else if(CODE == __ACCEPT_MSG_RES__){

	}

}

void ResponseHandler::init_response_header(uint8_t* header) {
	int* start;
	start = (int*)malloc(sizeof(int));
	*start = 0;
	this->header_size = __VERSION_SIZE__ + __CODE_SIZE__ + __PAYLOAD_SIZE_SIZE__;
	this->payload_start = this->header_size + 1;
	this->__set_message(start, this->version, header, __VERSION_SIZE__);
	this->__set_message(start, this->code, header, __CODE_SIZE__);
	this->__set_message(start, this->payload_size, header, __PAYLOAD_SIZE_SIZE__);
	int_payload_size = (int*)malloc(sizeof(int));
	int_code = (unsigned int*)calloc(sizeof(unsigned int), 0);
	memcpy(int_payload_size, (int*)&this->payload_size, __PAYLOAD_SIZE_SIZE__);
	memcpy(int_code, (unsigned int*)&this->code, __CODE_SIZE__);

}

void ResponseHandler::__set_message(int* start, uint8_t* insert_to, uint8_t* insert_from, int size_to_add) {
	int start_val = *start;
	for (int i = start_val, j=0; i < size_to_add + start_val; i++, j++) {
		insert_to[j] = insert_from[i];
	}
	*start = *start + size_to_add;

}