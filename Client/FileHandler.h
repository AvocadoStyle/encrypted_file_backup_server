#ifndef __FILEHANDLER_H__
#define __FILEHANDLER_H__


#include "FileUtilities.h"

#define CLIENT_INSTRUCTION	"transfer.info"
#define CLIENT_INFO			"me.info"

class FileHandler {
public:
	FileUtilities *file_utilities;
	
	FileHandler();
	~FileHandler();

	std::string read_address_from_file();
	std::string read_port_from_file();

private:
	uint8_t* __read_client_instruction_file();




};






#endif