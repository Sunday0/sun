#pragma once
#include <cstdint>
#pragma warning(disable:4200)

#pragma pack(1)

/* 协议说明
* 	sun_pack    协议头部结构, 校验，压缩，长度 标识 均指 msg_buff
*
*   数据包字段说明:
*  
* 	magic		协议签名: $
* 	zip_flag	压缩标志: 0 未压缩 1 压缩  LZW8192
* 	cmp_flag    加密标志: 0 未加密 1 加密  后续采用 openssl 接口 ，暂不实现
* 	err_flag	错误标志: 0 正常	  errno 无效，frames 和 number有效
*						  1 错误  错误码由 errno   解释
* 	paddingf    位域补齐
* 	crc16		crc16校验
* 	main		主协议号	
* 	child		子协议号
* 	seqno		请求序号 自增
* 	errno		错误编码
* 	frames		总帧数
* 	number		当前帧编号
*   msg_len		消息长度
* 	padding;	补齐
* 	msg_buff[0] 消息内容
*/

struct sun_pack
{
	uint8_t  magic;
	uint8_t  zip_flag : 1;
	uint8_t  cmp_flag : 1;
	uint8_t  err_flag : 1;
	uint8_t  paddingf : 5;
	uint16_t crc16;
	uint16_t main;
	uint16_t child;
	uint16_t seqno;
	union {
		uint16_t errno;
		struct {
			uint8_t frames;
			uint8_t number;
		};
	};
	uint16_t msg_len;
	uint16_t padding;
	int8_t	 msg_buff[0];
};

#pragma pack()