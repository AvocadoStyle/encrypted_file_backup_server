#include "RequestsHandler.h"

RequestsHandler::RequestsHandler() {
	this->file_handler	= new FileHandler();
	this->crc_handler	= new CRC();
}

RequestsHandler::~RequestsHandler() {

}

/* initialize the Requests Handler object according to the registration info file.
 * will initialize the according fields:
 * name			- initialzie the name according to the registration info file.
 * ClientID		- initialize both the str hex and the byte client_id.
 * PrivateKey	- initialize the privatekey.
 */
void RequestsHandler::initialize_fields_from_register_file_info() {
	/* if the registration file exists it will read all the fields including name, client_id hex, private_key base64 */
	try {
		if (this->file_handler->is_client_register_info_exists()) {
			this->name_st = this->file_handler->read_name_from_register_file_info();
			this->client_id_st_hex = this->file_handler->read_client_id_from_register_file_info();
			this->private_key = this->file_handler->read_private_key_64_from_register_file_info();
		}

		/* convert the string values and hex values into bytes and initialize the bytes of the request object according
		 * to the values that loaded from the registration file
		 */

		/* convert client_id hex to bytes */
		this->__set_client_id_from_str_hex_client_id(this->client_id_st_hex);

		/* set name string to bytes */
		this->__set_name_from_str_name(this->name_st);
	} catch(...){
		throw "not good";
	}
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

	/* merge all to one message in bytes */
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
void RequestsHandler::authentication_request_handle() {
	try {
	// initialize the name, client_id, private_key
	this->initialize_fields_from_register_file_info();

	/* header */
	// set version default 
	this->__set_version_id_default();
	// set code for authentication
	int reg_request_number_code = __SEND_PK__;
	memcpy(this->code, (uint8_t*)&reg_request_number_code, __CODE_SIZE__);
	// set fixed payload size: name + public_key
	int payload_fixed_size = __NAME_SIZE__ + __PK_SIZE__;
	this->__set_payload_fixed_size(payload_fixed_size);

	/* payload */
	this->generate_rsa_public_key();
	this->payload = (uint8_t*)malloc(sizeof(uint8_t) * payload_fixed_size);
	int i = 0;
	for (; i < __NAME_SIZE__; i++) {
		this->payload[i] = this->name[i];
	}
	i = __NAME_SIZE__;
	for (int j=0;i < __NAME_SIZE__+__PK_SIZE__; i++, j++) {
		this->payload[i] = this->public_key_bytes[j];
	}


	/* merge all to one message in bytes */
	// merge together the message
	this->total_size = __CLIENT_ID_SIZE__ + __VERSION_SIZE__ + __CODE_SIZE__ + __PAYLOAD_SIZE_SIZE__ + __NAME_SIZE__ + __PK_SIZE__;
	this->header_size = __HEADER_SIZE__;
	this->build_message = (uint8_t*)malloc(sizeof(uint8_t) * total_size);
	int* start;
	start = (int*)malloc(sizeof(int));
	*start = 0;
	this->__set_build_message(start, this->client_id, __CLIENT_ID_SIZE__);
	this->__set_build_message(start, this->version, __VERSION_SIZE__);
	this->__set_build_message(start, this->code, __CODE_SIZE__);
	this->__set_build_message(start, this->payload_size, __PAYLOAD_SIZE_SIZE__);
	this->__set_build_message(start, this->payload, __NAME_SIZE__+__PK_SIZE__);

	}
	catch (...) {
		return;
	}
}

/* handle the file sending request and build entire message to send to the server
 * take of the header request and for the payload including:
 * header:
 * client_id		: will be placed from the previous registration request.
 * version			: will set to default value.
 * code				: will set the code value as expected for the file_send request.
 * payload size		: will contain the payload size - from the payload: client_id, content_size, file_name, message_content
 * payload:
 * client_id		: client id from the registration file
 * content_size		: the size of the message content, represents in 4 bytes
 * message_content	: changed size of each file content
 */
void RequestsHandler::send_file_request_handle() {
	// initialize the name, client_id, private_key
	this->initialize_fields_from_register_file_info();

	/* header */
	// set version default 
	this->__set_version_id_default();
	// set code for sending file
	int reg_request_number_code = __SEND_FILE__;
	memcpy(this->code, (uint8_t*)&reg_request_number_code, __CODE_SIZE__);



	/* parse file content */
	file_to_send_content		= this->file_handler->parse_file_to_send();
	size_t len_of_file_content	= this->__my_strlen(file_to_send_content) - 1; 
	file_to_send_content[len_of_file_content] = '\0';
	this->file_content_size = len_of_file_content;
	
	memcpy(this->file_content_size_bytes, (uint8_t*)&this->file_content_size, __CONTENT_SIZE__);
	memcpy(this->file_name, this->file_handler->file_name_after_parse, __FILE_NAME_SIZE__);




	// set fixed payload size:
	int payload_fixed_size = __CLIENT_ID_SIZE__ + __CONTENT_SIZE__  + __FILE_NAME_SIZE__ + this->file_content_size;
	this->__set_payload_fixed_size(payload_fixed_size);
	///* payload */
	//this->generate_rsa_public_key();
	this->payload = (uint8_t*)malloc(sizeof(uint8_t) * payload_fixed_size);
	int i = 0;
	int len = 0;
	for (; i < __CLIENT_ID_SIZE__; i++) {
		this->payload[i] = this->client_id[i];
	}
	i = __CLIENT_ID_SIZE__;
	len = i + __CONTENT_SIZE__;
	for (int j = 0; i < len; i++, j++) {
		this->payload[i] = this->file_content_size_bytes[j];
	}
	i = __CLIENT_ID_SIZE__ + __CONTENT_SIZE__;
	len = i + __FILE_NAME_SIZE__;
	for (int j = 0; i < len; i++, j++) {
		this->payload[i] = this->file_name[j];
	}
	i = __CLIENT_ID_SIZE__ + __CONTENT_SIZE__ + __FILE_NAME_SIZE__;
	len = i + this->file_content_size;
	for (int j = 0; i < len; i++, j++) {
		this->payload[i] = this->file_to_send_content[j];
	}


	/* merge all to one message in bytes */
	// merge together the message
	this->total_size = __CLIENT_ID_SIZE__ + __VERSION_SIZE__ + __CODE_SIZE__ + __PAYLOAD_SIZE_SIZE__ + payload_fixed_size;
	this->header_size = __HEADER_SIZE__;
	this->build_message = (uint8_t*)malloc(sizeof(uint8_t) * total_size);
	int* start;
	start = (int*)malloc(sizeof(int));
	*start = 0;
	this->__set_build_message(start, this->client_id, __CLIENT_ID_SIZE__);
	this->__set_build_message(start, this->version, __VERSION_SIZE__);
	this->__set_build_message(start, this->code, __CODE_SIZE__);
	this->__set_build_message(start, this->payload_size, __PAYLOAD_SIZE_SIZE__);
	this->__set_build_message(start, this->payload, payload_fixed_size);

	this->crc_handler->update(this->file_to_send_content, file_content_size);
	this->cksum = this->crc_handler->digest();
}

void RequestsHandler::crc_valid() {
	this->__send_requests_crc(__CRC_VALID__);
}

void RequestsHandler::crc_not_valid() {
	this->__send_requests_crc(__CRC_NOT_VALID_SEND_AGAIN__);
}

void RequestsHandler::crc_not_valid_final() {
	this->__send_requests_crc(__CRC_NOT_VALID_LAST_X__);
}





/*********************** helpers ****************************/

/* generates the rsa key public.
 *
 */
void RequestsHandler::generate_rsa_public_key() {
	std::string pkk = "abcdefg";
	for (int i = 0; i < pkk.length(); i++) {
		this->public_key_bytes[i] = pkk.c_str()[i];
	}
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
 * set payload fixed size
 */
void RequestsHandler::__set_payload_fixed_size(int size_of_payload) {
	// set payload size
	memcpy(this->payload_size, (uint8_t*)&size_of_payload, sizeof(this->payload_size));
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

/*
 * set the name from the str name include null terminator 
 */
void RequestsHandler::__set_name_from_str_name(std::string name_str) {
	for (int i = 0; i < __NAME_SIZE__; i++) {
		this->name[i] = (uint8_t)(name_str.c_str()[i]);
	}
}

/*
 * client_id from hex str to bytes
 */
void RequestsHandler::__set_client_id_from_str_hex_client_id(std::string client_id_str_hex) {
	std::string hash = boost::algorithm::unhex(client_id_str_hex);
	std::copy(hash.begin(), hash.end(), this->client_id);
}

/*
 * @TODO
 */
void RequestsHandler::__set_private_key_from_str_base64_private_key(std::string private_key_str_base64) {
	for (int i = 0; i < __NAME_SIZE__; i++) {
		this->version[i] = '\x00';
	}
}

size_t RequestsHandler::__my_strlen(uint8_t* str) {
	size_t i;
	for (i = 0; str[i]; i++);
	return i;
}

void RequestsHandler::__send_requests_crc(int REQUEST) {
	// initialize the name, client_id, private_key
	this->initialize_fields_from_register_file_info();

	/* header */
	// set version default 
	this->__set_version_id_default();
	// set code for sending file
	int reg_request_number_code = REQUEST;
	memcpy(this->code, (uint8_t*)&reg_request_number_code, __CODE_SIZE__);
	memcpy(this->file_name, this->file_handler->file_name_after_parse, __FILE_NAME_SIZE__);

	int payload_fixed_size = __CLIENT_ID_SIZE__ + __FILE_NAME_SIZE__;
	this->__set_payload_fixed_size(payload_fixed_size);
	///* payload */
	//this->generate_rsa_public_key();
	this->payload = (uint8_t*)malloc(sizeof(uint8_t) * payload_fixed_size);
	int i = 0;
	int len = 0;
	for (; i < __CLIENT_ID_SIZE__; i++) {
		this->payload[i] = this->client_id[i];
	}
	i = __CLIENT_ID_SIZE__;
	len = i + __FILE_NAME_SIZE__;
	for (int j = 0; i < len; i++, j++) {
		this->payload[i] = this->file_name[j];
	}
	/* merge all to one message in bytes */
	// merge together the message
	this->total_size = __CLIENT_ID_SIZE__ + __VERSION_SIZE__ + __CODE_SIZE__ + __PAYLOAD_SIZE_SIZE__ + payload_fixed_size;
	this->header_size = __HEADER_SIZE__;
	this->build_message = (uint8_t*)malloc(sizeof(uint8_t) * total_size);
	int* start;
	start = (int*)malloc(sizeof(int));
	*start = 0;
	this->__set_build_message(start, this->client_id, __CLIENT_ID_SIZE__);
	this->__set_build_message(start, this->version, __VERSION_SIZE__);
	this->__set_build_message(start, this->code, __CODE_SIZE__);
	this->__set_build_message(start, this->payload_size, __PAYLOAD_SIZE_SIZE__);
	this->__set_build_message(start, this->payload, payload_fixed_size);
}