#pragma once
#include <cstdint>
#pragma warning(disable:4200)

#pragma pack(1)

/* Э��˵��
 *
 * 	magic		Э��ǩ��: $
 *	crc16		У����   
 *  padding		����   
 *	lenth		��Ϣ����
 * 	msg_buff[0] ��Ϣ����
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