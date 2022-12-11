#ifndef __CRC_H__
#define __CRC_H__

#include <cstdint>
#include <string>

class CRC {
private:
	uint32_t crc;
	uint32_t nchar;

public:
	CRC();
	void update(unsigned char*, uint32_t);
	uint32_t digest();
};

#endif