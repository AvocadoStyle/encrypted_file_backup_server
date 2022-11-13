#include "FileHandler.h"
#include <iostream>



FileHandler::FileHandler() {
	this->file_utilities = new FileUtilities();
}

FileHandler::~FileHandler() {

}
std::string FileHandler::read_address_from_file() {
	uint8_t* content = this->__read_client_instruction_file();
	char* con = reinterpret_cast<char*>(content);
	std::string cntnt = con;
	std::string del = ":";
	size_t addr_end = cntnt.find(del);
	std::string token_address = cntnt.substr(0, addr_end);
	return token_address;
	//return this->file_utilities->string_to_uint8_t(token_address);
}
std::string FileHandler::read_port_from_file() {
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

uint8_t* FileHandler::__read_client_instruction_file() {
	std::string client_instruction = CLIENT_INSTRUCTION;
	size_t len = client_instruction.length();
	uint8_t* client_instruction_file_name = (uint8_t*)malloc(sizeof(uint8_t) * len);
	memcpy(client_instruction_file_name, client_instruction.c_str(), len);
	client_instruction_file_name[len] = '\0';
	return file_utilities->file_read(client_instruction_file_name);
}