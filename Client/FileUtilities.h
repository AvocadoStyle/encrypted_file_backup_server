#ifndef __FILEUTILITIES_H__
#define __FILEUTILITIES_H__

#include <string>
#include <fstream>

class FileUtilities {
public:
	std::ofstream file_w;
	std::ifstream file_r;
	FileUtilities();
	~FileUtilities();
	
	uint8_t* file_read(uint8_t* file_name);
	bool file_write(uint8_t* file_name);
	bool create_directory(std::string path);
	bool is_file_exists(uint8_t* file_name);
	//char* string_to_char(std::string st);
	//uint8_t* string_to_uint8_t(std::string st);
};


#endif