#ifndef __RESPONSEHANDLER_H__
#define __RESPONSEHANDLER_H__
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "RequestsHandler.h"




#define __REGISTRATION_RES__				2100
#define __REGISTRATION_FAILED_RES__			2101
#define __PRIMARY_KEY_ACCEPTED_RES__		2102
#define __CRC_VALID_RES__					2103
#define __ACCEPT_MSG_RES__					2104

/*                  HEADERS                                */
#define __HEADER_RES_SIZE__					7 
#define __CLIENT_ID_SIZE__					16
#define __VERSION_SIZE__					1
#define __CODE_SIZE__						2
#define __PAYLOAD_SIZE_SIZE__				4
/*                  PAYLOAD                                */
#define __NAME_SIZE__						255
#define __PK_SIZE__							160
#define __CLIENT_ID_SIZE__					16
#define __AES_KEY_SIZE__					16
#define __AES_KEY_ENCRYPTED_SIZE__			128
#define __CONTENT_SIZE__					4
#define __FILENAME_SIZE__					255
#define __CKSUM_SIZE__						4




class ResponseHandler {
public:
	RequestsHandler*	req_handler;
	size_t				HEADER_SIZE_PATTERN = __HEADER_RES_SIZE__;
	uint8_t				version[__VERSION_SIZE__];
	uint8_t				code[__CODE_SIZE__];
	uint8_t				payload_size[__PAYLOAD_SIZE_SIZE__];
	uint8_t*			payload;
	uint8_t				name[__NAME_SIZE__];
	uint8_t				public_key[__PK_SIZE__];
	std::string			private_key;
	std::string			priv_key_base64;
	uint8_t				aes_key[__AES_KEY_SIZE__];
	std::string			whatwhat;
	std::string			aes_key_st;
	uint8_t				aes_key_encrypted[__AES_KEY_ENCRYPTED_SIZE__];
	std::string			whatwhatwhat;
	std::string			aes_key_encrypted_st;
	uint8_t				client_id[__CLIENT_ID_SIZE__];
	std::string			client_id_st_hex;
	unsigned int*		int_code;
	int					total_size;
	int					header_size;
	int					payload_start;
	int*				int_payload_size;
	uint8_t*			cksum_client;
	uint8_t*			cksum_server;
	int*				cksum_client_int;
	uint32_t*			cksum_server_int;



	ResponseHandler(RequestsHandler* req_handler);
	~ResponseHandler();
	/*                           response handler                            */
	// header response handler
	void init_response_header(uint8_t* header);
	// payload response handler:
	bool response_code_handler(uint8_t* buffer_header_and_payload);
	void registration_response_handle(uint8_t* buffer_header_and_payload);
	void authentication_response_handle(uint8_t* buffer_header_and_payload);
	bool receive_crc_response_handle(uint8_t* buffer_header_and_payload);
	void set_client_id_str_hex();
	void set_private_key(std::string private_key);




private:
	void __set_message(int* start, uint8_t* insert_to, uint8_t* insert_from, int size_to_add);
	void __from_uint8_array_to_int(uint32_t* to, uint8_t* from, size_t size);
};







#endif
