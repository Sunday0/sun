#pragma once

#include "sun_pack.h"


#pragma pack(1)

/*
* 内部消息封装
* 存储消息的连接信息
*/

struct sun_msg
{
	uint64_t link_no;
	sun_pack pack;
};

#pragma pack()