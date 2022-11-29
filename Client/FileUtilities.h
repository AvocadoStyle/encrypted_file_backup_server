#ifndef __FILEUTILITIES_H__
#define __FILEUTILITIES_H__
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>

class FileUtilities {
public:
	std::ofstream file_w;
	std::ifstream file_r;
	FileUtilities();
	~FileUtilities();
	
	uint8_t* file_read(uint8_t* file_name);
	bool file_write(uint8_t* file_name);
	bool create_directory(std::string path);
	bool is_file_exists(char* file_name);
	//char* string_to_char(std::string st);
	//uint8_t* string_to_uint8_t(std::string st);
};


#endif