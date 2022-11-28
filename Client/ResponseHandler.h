#ifndef __RESPONSEHANDLER_H__
#define __RESPONSEHANDLER_H__
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#define __REGISTRATION_RES__				2100
#define __REGISTRATION_FAILED_RES__			2101
#define __PRIMARY_KEY_ACCEPTED_RES__		2102
#define __CRC_VALID_RES__					2103
#define __ACCEPT_MSG_RES__					2104

/*                  HEADERS                                */
#define __HEADER_SIZE__					7 
#define __CLIENT_ID_SIZE__				16
#define __VERSION_SIZE__				1
#define __CODE_SIZE__					2
#define __PAYLOAD_SIZE_SIZE__			4
/*                  PAYLOAD                                */
#define __NAME_SIZE__					255
#define __PK_SIZE__						160
#define __CLIENT_ID_SIZE__				16



class ResponseHandler {
public:
	size_t				HEADER_SIZE_PATTERN = __HEADER_SIZE__;
	uint8_t				version[__VERSION_SIZE__];
	uint8_t				code[__CODE_SIZE__];
	uint8_t				payload_size[__PAYLOAD_SIZE_SIZE__];
	uint8_t*			payload;
	uint8_t				name[__NAME_SIZE__];
	uint8_t				public_key[__PK_SIZE__];
	uint8_t				client_id[__CLIENT_ID_SIZE__];
	std::string			client_id_st_hex;
	unsigned int*		int_code;
	int					total_size;
	int					header_size;
	int					payload_start;
	int*				int_payload_size;




	ResponseHandler();
	~ResponseHandler();
	/*                           response handler                            */
	// header response handler
	void init_response_header(uint8_t* header);
	// payload response handler:
	void response_code_handler(uint8_t* buffer_header_and_payload);
	void registration_response_handle(uint8_t* buffer_header_and_payload);




private:
	void __set_message(int* start, uint8_t* insert_to, uint8_t* insert_from, int size_to_add);
	void __set_client_id_str_hex();
};







#endif
