#ifndef __REQUESTHANDLER_H__
#define __REQUESTHANDLER_H__

#include <iostream>
#include <boost/algorithm/hex.hpp>
#include "FileHandler.h"

#define __REGISTRATION__				1100
#define __SEND_PK__						1101
#define __SEND_FILE__					1103
#define __CRC_VALID__					1104
#define __CRC_NOT_VALID_SEND_AGAIN__	1105
#define __CRC_NOT_VALID_LAST_X__		1106

/*                  HEADERS                                */
#define __HEADER_SIZE__					23 
#define __CLIENT_ID_SIZE__				16
#define __VERSION_SIZE__				1
#define __CODE_SIZE__					2
#define __PAYLOAD_SIZE_SIZE__			4
/*                  PAYLOAD                                */
#define __NAME_SIZE__					255
#define __PK_SIZE__						160




class RequestsHandler {
public:
	FileHandler* file_handler;
	uint8_t			client_id[__CLIENT_ID_SIZE__];
	uint8_t			version[__VERSION_SIZE__];
	uint8_t			code[__CODE_SIZE__];
	uint8_t			payload_size[__PAYLOAD_SIZE_SIZE__];
	uint8_t			name[__NAME_SIZE__];
	uint8_t*		header_message;
	uint8_t*		payload;
	uint8_t*		build_message;
	uint8_t			public_key_bytes[__PK_SIZE__];
	int				total_size;
	int				header_size;
	std::string		public_key;
	std::string		private_key;
	std::string		client_id_st_hex;
	std::string		name_st;

	RequestsHandler();
	~RequestsHandler();

	void registration_request_handle(std::string name);
	void authentication_request_handle();
	void generate_rsa_public_key();
	void generate_rsa_private_key();
	void initialize_fields_from_register_file_info();

private:
	void __set_build_message(int* start, uint8_t* section, int size_to_add);
	void __set_client_id_default();
	void __set_version_id_default();
	void __set_name_from_str_name(std::string name_str);
	void __set_client_id_from_str_hex_client_id(std::string client_id_str_hex);
	void __set_private_key_from_str_base64_private_key(std::string private_key_str_base64);
	void __set_payload_fixed_size(int size_of_payload);

};







#endif
