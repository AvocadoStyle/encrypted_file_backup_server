#include "ResponseHandler.h"



ResponseHandler::ResponseHandler() {

}
ResponseHandler::~ResponseHandler() {

}


void ResponseHandler::registration_response_handle() {

}

void ResponseHandler::init_response_header(uint8_t* header) {
	int* start;
	start = (int*)malloc(sizeof(int));
	*start = 0;
	this->header_size = __VERSION_SIZE__ + __CODE_SIZE__ + __PAYLOAD_SIZE_SIZE__;
	this->__set_message(start, this->version, header, __VERSION_SIZE__);
	this->__set_message(start, this->code, header, __CODE_SIZE__);
	this->__set_message(start, this->payload_size, header, __PAYLOAD_SIZE_SIZE__);
	int_payload_size = (int*)malloc(sizeof(int));
	memcpy(int_payload_size, (int*)&this->payload_size, __PAYLOAD_SIZE_SIZE__);
}

void ResponseHandler::__set_message(int* start, uint8_t* insert_to, uint8_t* insert_from, int size_to_add) {
	int start_val = *start;
	for (int i = start_val, j=0; i < size_to_add + start_val; i++, j++) {
		insert_to[j] = insert_from[i];
	}
	*start = *start + size_to_add;

}