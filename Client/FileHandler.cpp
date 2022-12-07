#include "FileHandler.h"
#include <iostream>



FileHandler::FileHandler() {
	this->file_utilities = new FileUtilities();
}

FileHandler::~FileHandler() {

}

/********************************* read file to send ***************************************/
/* parse the file that we want to send (the file path is taken from .info file).
 * we'll read the file path and than read the file content with uint8_t* array.
 */
uint8_t* FileHandler::parse_file_to_send() {
	std::string file_path_name = this->read_file_path_from_file_info();
	size_t len = file_path_name.length() * sizeof(uint8_t*);
	uint8_t* file_name = (uint8_t*)malloc(len);
	memcpy(file_name, file_path_name.c_str(), len);
	memcpy(this->file_name_path_from_register_file, file_path_name.c_str(), len);
	memcpy(this->file_name_after_parse,
		this->file_utilities->get_file_name_seperated_from_path(this->file_name_path_from_register_file),
		__FILENAME_SIZE__);
	return this->file_utilities->file_read(file_name);
}





/******************************** after register ******************************************/

/*                      after register write info                              */
bool FileHandler::write_registration_info_file(std::string name, std::string client_id_ascii_hex,
	std::string private_key_base_64) {
	try {
		std::ofstream file_w("me.info");
		file_w << name << '\n' << client_id_ascii_hex << '\n' << private_key_base_64;
		return true;
	}
	catch (const std::exception& e) {
		return false;
	}
}

/*                      after register read info                              */
bool FileHandler::is_client_register_info_exists() {
	char client_register_file[] = CLIENT_INFO;
	return this->file_utilities->is_file_exists(client_register_file);
}

/*
 * read name from registration file 
 */
std::string FileHandler::read_name_from_register_file_info() {
	uint8_t* content = this->__read_client_register_info_file();
	char* con = reinterpret_cast<char*>(content);
	std::string cntnt = con;

	std::string del = "\n";
	size_t name_start = 0;
	size_t name_end = cntnt.find(del);

	std::string name = cntnt.substr(name_start, name_end);
	return name;
}

/*
 * read client_id from registration file
 */
std::string FileHandler::read_client_id_from_register_file_info() {
	uint8_t* content = this->__read_client_register_info_file();
	char* con = reinterpret_cast<char*>(content);
	std::string cntnt = con;

	std::string del = "\n";
	size_t client_id_start = cntnt.find(del) + 1;
	std::string client_id_s = cntnt.substr(client_id_start, CLIENT_ID_HEX_SIZE);
	return client_id_s;
}

/*
 * read private key from registration file
 */
std::string FileHandler::read_private_key_64_from_register_file_info() {
	uint8_t* content = this->__read_client_register_info_file();
	char* con = reinterpret_cast<char*>(content);
	std::string cntnt = con;

	std::string del = "\n";
	size_t private_key_start = cntnt.find(del) + 1 + CLIENT_ID_HEX_SIZE + 1;

	std::string private_key_s = cntnt.substr(private_key_start);
	size_t private_key_end = private_key_s.find(del);
	return cntnt.substr(private_key_start, private_key_end);
}



/*                    before register                                                                */
/*                      before register read info from initial info file                             */

/*
 * reads the address from the initial file
 */
std::string FileHandler::read_address_from_file_info() {
	uint8_t* content = this->__read_client_instruction_file();
	char* con = reinterpret_cast<char*>(content);
	std::string cntnt = con;
	std::string del = ":";
	size_t addr_end = cntnt.find(del);
	std::string token_address = cntnt.substr(0, addr_end);
	return token_address;
	//return this->file_utilities->string_to_uint8_t(token_address);
}

/*
 * reads the port from the initial file
 */
std::string FileHandler::read_port_from_file_info() {
	uint8_t* content = this->__read_client_instruction_file();
	char* con = reinterpret_cast<char*>(content);
	std::string cntnt = con;
	std::string del = ":";
	size_t port_start = cntnt.find(del)+1;
	del = "\n";
	size_t port_end = cntnt.find(del);
	std::string token_port = cntnt.substr(port_start, port_end-port_start);
	return token_port;
}
/*
 * read the name of the user from the initial file
 */
std::string FileHandler::read_name_from_file_info() {
	uint8_t* content = this->__read_client_instruction_file();
	char* con = reinterpret_cast<char*>(content);
	std::string cntnt = con;
	std::string del = "\n";
	// take first token pos
	size_t name_start = cntnt.find(del) + 1;
	std::string name_s = cntnt.substr(name_start, 100);
	// take next token pos
	int name_end = name_s.find(del); // make sure it's 100 chars atleast
	std::string token_name = cntnt.substr(name_start, name_end);
	return token_name;
}

/*
 * reads the file path from the initial file
 */
std::string FileHandler::read_file_path_from_file_info() {
	uint8_t* content = this->__read_client_instruction_file();
	char* con = reinterpret_cast<char*>(content);
	std::string cntnt = con;
	std::string del = "\n";
	size_t file_start = cntnt.find(del) + 1;
	std::string new_st = cntnt.substr(file_start);
	size_t file_s = new_st.find(del) + 1;
	std::string file_name = new_st.substr(file_s);
	size_t fe = file_name.find(del);
	file_name = file_name.substr(0, fe);

	return file_name;
}

/* read client initial instruction file before registration
 * will read it from the file utilities
 * :return: uint8_t* value of the read file
 */
uint8_t* FileHandler::__read_client_instruction_file() {
	std::string client_instruction = CLIENT_INSTRUCTION;
	size_t len = client_instruction.length();
	uint8_t* client_instruction_file_name = (uint8_t*)malloc(sizeof(uint8_t) * len);
	memcpy(client_instruction_file_name, client_instruction.c_str(), len);
	client_instruction_file_name[len] = '\0';
	memcpy(this->file_name, client_instruction_file_name, __FILENAME_SIZE__);
	return file_utilities->file_read(client_instruction_file_name);
}

/* read the client details after registration.
 * there is the initial value of the registration of name and client id.
 * and there is the last value of the name client id and private key.
 */
uint8_t* FileHandler::__read_client_register_info_file() {
	std::string client_instruction = CLIENT_INFO;
	size_t len = client_instruction.length();
	uint8_t* client_instruction_file_name = (uint8_t*)malloc(sizeof(uint8_t) * len);
	memcpy(client_instruction_file_name, client_instruction.c_str(), len);
	client_instruction_file_name[len] = '\0';
	return file_utilities->file_read(client_instruction_file_name);
}