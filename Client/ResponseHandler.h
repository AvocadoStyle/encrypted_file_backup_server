#ifndef __RESPONSEHANDLER_H__
#define __RESPONSEHANDLER_H__
#include <iostream>

/*                  HEADERS                                */
#define __HEADER_SIZE__					23 
#define __CLIENT_ID_SIZE__				16
#define __VERSION_SIZE__				1
#define __CODE_SIZE__					2
#define __PAYLOAD_SIZE_SIZE__			4
/*                  PAYLOAD                                */
#define __NAME_SIZE__					255
#define __PK_SIZE__						160


class ResponseHandler {
public:
	uint8_t		version[__VERSION_SIZE__];
	uint8_t		code[__CODE_SIZE__];
	uint8_t		payload_size[__PAYLOAD_SIZE_SIZE__];
	uint8_t*	payload;
	int			total_size;
	int			header_size;
	int*		int_payload_size;




	ResponseHandler();
	~ResponseHandler();


	void registration_response_handle();
	void init_response_header(uint8_t* header);
private:
	void __set_message(int* start, uint8_t* insert_to, uint8_t* insert_from, int size_to_add);
};







#endif
