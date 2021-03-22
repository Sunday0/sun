#pragma once

#include <cstdint> 

// ����Э����� ��װ

class sun_protocol
{
public:
	static uint16_t sun_crc16(const int8_t* data, int32_t len);
public:
	static int32_t analyze(int8_t * buff, uint16_t len);
};
