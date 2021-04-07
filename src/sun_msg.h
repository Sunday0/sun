#pragma once

#include <cstdint>

/*
* 内部消息封装
* 存储消息的连接信息
*/

struct sun_msg
{
	uint64_t link_no;
	uint32_t lenth;
	int8_t	 data[0];
};
