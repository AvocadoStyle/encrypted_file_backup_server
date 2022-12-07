#include "FileUtilities.h"
#include <iostream>

FileUtilities::FileUtilities() {

}

FileUtilities::~FileUtilities() {

}
uint8_t* FileUtilities::file_read(uint8_t* file_name) {
	try {
		char* file_name_char = reinterpret_cast<char*>(file_name);
		this->file_r.open(file_name_char);
		std::string line;
		std::string file_content;
		while (std::getline(this->file_r, line)) {
			file_content += line;
			file_content += '\n';
		}
		this->file_r.close();
		uint8_t* content = (uint8_t*)malloc(sizeof(uint8_t) * file_content.length());
		memcpy(content, file_content.c_str(), file_content.length());
		content[file_content.length()] = '\0';
		return content;
	} catch(const std::exception& e){
			throw std::exception(e.what());
	}
}

bool FileUtilities::file_write(uint8_t* file_name) {
	char* dst = reinterpret_cast<char*>(file_name);
	return true;
}

bool FileUtilities::is_file_exists(char* file_name) {
	if (FILE* file = fopen(file_name, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}


	return true;
}

uint8_t* FileUtilities::get_file_name_seperated_from_path(uint8_t* file_path_name) {
	uint8_t file_path_name_cpy[255];
	memcpy(file_path_name_cpy, file_path_name, 255);
	char tk[2] = "\\";
	char* token;
	uint8_t last_file_name[255];
	token = strtok((char*)file_path_name_cpy, tk);
	while (token != NULL) {
		memcpy(last_file_name, token, 255);
		token = strtok(NULL, tk);
	}
	return last_file_name;
}

//char* FileUtilities::string_to_char(std::string st) {
//	char* c = (char*)malloc(sizeof(char) * st.length());
//	memcpy(c, st.c_str(), st.length());
//	c[st.length()] = '\0';
//	return c;
//}
//
//uint8_t* string_to_uint8_t(std::string st) {
//	int len = st.length();
//	uint8_t* c = (uint8_t*)malloc(sizeof(uint8_t) * len);
//	memcpy(c, st.c_str(), len);
//	c[len] = '\0';
//	return c;
//}


