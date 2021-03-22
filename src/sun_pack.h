#pragma once
#include <cstdint>
#pragma warning(disable:4200)

#pragma pack(1)

/* 协议说明
 *
 * 	magic		协议签名: $
 *	crc16		校验码   
 *  padding		补齐   
 *	lenth		消息长度
 * 	msg_buff[0] 消息内容
 */

struct sun_pack
{
	uint8_t  magic;
	uint16_t crc16;
	uint8_t  padding;
	uint32_t lenth;
	int8_t	 data[0];
};

#pragma pack()