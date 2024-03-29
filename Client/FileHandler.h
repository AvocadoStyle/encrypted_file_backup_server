#ifndef __FILEHANDLER_H__
#define __FILEHANDLER_H__


#include "FileUtilities.h"

#define CLIENT_INSTRUCTION		"transfer.info"
#define CLIENT_INFO				"me.info"
#define CLIENT_ID_HEX_SIZE		32

#define __FILENAME_SIZE__		255

class FileHandler {
public:
	FileUtilities *file_utilities;
	
	uint8_t file_name[__FILENAME_SIZE__];
	uint8_t file_name_path_from_register_file[__FILENAME_SIZE__];
	uint8_t file_name_after_parse[__FILENAME_SIZE__];
	FileHandler();
	~FileHandler();

	/*						file to send handle									  */
	uint8_t* parse_file_to_send();

	/*                      before register read info                             */
	std::string read_address_from_file_info();
	std::string read_port_from_file_info();
	std::string read_name_from_file_info();
	std::string read_file_path_from_file_info();


	/*                      after register read info                              */
	bool is_client_register_info_exists();
	std::string read_name_from_register_file_info();
	std::string read_client_id_from_register_file_info();
	std::string read_private_key_64_from_register_file_info();


	/*                      after register write info                              */
	bool write_registration_info_file(std::string name, std::string client_id_ascii_hex,
		std::string private_key_base_64);

private:
	uint8_t* __read_client_instruction_file();
	uint8_t* __read_client_register_info_file();


};






#endif