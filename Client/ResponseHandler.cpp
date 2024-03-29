#include "ResponseHandler.h"



ResponseHandler::ResponseHandler(RequestsHandler* req_handler) {
	/* shallow copy */
	this->req_handler = req_handler; 
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
	this->client_id[__CLIENT_ID_SIZE__] = '\0';
}

void ResponseHandler::authentication_response_handle(uint8_t* buffer_header_and_payload) {
	int* start;
	start = (int*)malloc(sizeof(int));
	// payload starting from
	*start = this->payload_start;
	// setting the client_id from the buffer
	this->__set_message(start, this->client_id, buffer_header_and_payload, __CLIENT_ID_SIZE__);
	this->client_id[__CLIENT_ID_SIZE__] = '\0';
	this->__set_message(start, this->aes_key, buffer_header_and_payload, __AES_KEY_SIZE__);
}

bool ResponseHandler::receive_crc_response_handle(uint8_t* buffer_header_and_payload) {
	int* start;
	start	= (int*)malloc(sizeof(int));
	*start	= __CLIENT_ID_SIZE__ + __CONTENT_SIZE__ + __FILENAME_SIZE__;
	this->cksum_server = (uint8_t*)malloc(sizeof(uint8_t) * __CKSUM_SIZE__);
	this->__set_message(start, this->cksum_server, buffer_header_and_payload,
		__CKSUM_SIZE__);
	this->cksum_server_int = (uint32_t*)malloc(sizeof(uint32_t));
	this->__from_uint8_array_to_int(this->cksum_server_int, this->cksum_server, sizeof(uint32_t));
	return this->req_handler->cksum == *this->cksum_server_int;
}


bool ResponseHandler::response_code_handler(uint8_t* buffer_header_and_payload) {
	int CODE = *this->int_code;
	if (CODE == __REGISTRATION_RES__) {
		this->registration_response_handle(buffer_header_and_payload);
	} else if(CODE == __REGISTRATION_FAILED_RES__){



	} else if(CODE == __PRIMARY_KEY_ACCEPTED_RES__){
		this->authentication_response_handle(buffer_header_and_payload);
	} else if(CODE == __CRC_VALID_RES__){
		return this->receive_crc_response_handle(buffer_header_and_payload);
	} else if(CODE == __ACCEPT_MSG_RES__){
	}
	return true;
}

void ResponseHandler::init_response_header(uint8_t* header) {
	int* start;
	start = (int*)malloc(sizeof(int));
	*start = 0;
	this->header_size = __VERSION_SIZE__ + __CODE_SIZE__ + __PAYLOAD_SIZE_SIZE__;
	this->payload_start = this->header_size;
	this->__set_message(start, this->version, header, __VERSION_SIZE__);
	this->__set_message(start, this->code, header, __CODE_SIZE__);

	uint8_t temp = this->code[0];
	this->code[0] = this->code[1];
	this->code[1] = temp;

	this->__set_message(start, this->payload_size, header, __PAYLOAD_SIZE_SIZE__);
	int_payload_size = (int*)malloc(sizeof(int));
	int_code = (unsigned int*)calloc(sizeof(unsigned int), sizeof(unsigned int));
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

void ResponseHandler::set_client_id_str_hex() {
	std::stringstream ss;
	ss << std::hex;

	for (int i = 0; i < __CLIENT_ID_SIZE__; i++) {
		ss << std::setw(2) << std::setfill('0') << (int)this->client_id[i];
	}
	this->client_id_st_hex = ss.str();
}

void ResponseHandler::__from_uint8_array_to_int(uint32_t* to, uint8_t* from, size_t size) {
	memcpy(to, (uint32_t*)from, size);
}