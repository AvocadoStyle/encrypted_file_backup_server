#ifndef __REQUESTHANDLER_H__
#define __REQUESTHANDLER_H__

#include <iostream>

#define __REGISTRATION__				1100
#define __SEND_PK__						1101
#define __SEND_FILE__					1103
#define __CRC_VALID__					1104
#define __CRC_NOT_VALID_SEND_AGAIN__	1105
#define __CRC_NOT_VALID_LAST_X__		1106

/*                  HEADERS                                */
#define __CLIENT_ID_SIZE__				16
#define __VERSION_SIZE__				1
#define __CODE_SIZE__					2
#define __PAYLOAD_SIZE_SIZE__			4
/*                  PAYLOAD                                */
#define __NAME_SIZE__					255
#define __PK_SIZE__						160




class RequestsHandler {
public:
	uint8_t client_id[__CLIENT_ID_SIZE__];
	uint8_t version[__VERSION_SIZE__];
	uint8_t code[__CODE_SIZE__];
	uint8_t payload_size[__PAYLOAD_SIZE_SIZE__];
	uint8_t* payload;
	uint8_t* build_message;


	RequestsHandler();
	~RequestsHandler();

	void registration_request_handle(std::string name);

private:
	void __set_client_id_default();
	void __set_version_id_default();


};







#endif
