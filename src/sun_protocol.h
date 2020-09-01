#pragma once

#include <cstdint> 

// 负责协议解析 组装

class sun_protocol
{
public:
	static int analyze(int8_t * buff, uint16_t len);
	// static int build();
};
