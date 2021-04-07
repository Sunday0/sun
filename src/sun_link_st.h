#pragma once

// 定义IOCP 相关的结构体

#include <WinSock2.h>
#include <cstdint>

#define GET_IDX(link_no) (link_no & 0x0000FFFF)

constexpr auto MAX_SOCK_BUFFER = 8192;

using overlapped = OVERLAPPED;

/*
 *	操作 标志
 */
enum class olad_flag : uint32_t
{
	accept, 
	recv, 
	send
};

/*
 *	发送中 标志
 */
enum class send_flag : uint32_t
{
	idle,
	busy,
};

/*
 *	软关闭 标志
 */
enum class soft_flag : uint16_t
{
	used,
	shut,
};

struct sun_olad {
	overlapped				iocp_arg;								/*	参数*/
	olad_flag				ol_flgs;								/*	标志*/
	send_flag				sending;								/*	标志*/
	int8_t					buffer[MAX_SOCK_BUFFER];				/*	数据缓冲区*/
	uint16_t				bufsz;									/*	缓冲区大小*/
	int8_t			 		padding[6];								/*	对齐*/
	time_t					mtime;									/*	最后一次修改时间*/
};

struct sun_link {
	sun_olad				rx;										/*	接收连接头*/
	sun_olad				tx;										/*	发送连接头*/
	int32_t					sock;									/*	SOCKET描述符*/
	uint32_t				link_no;								/*	作为 iocp 的 key. link_no = seq<<16 | idx */
	uint16_t				idx;									/*	idx值*/
	uint16_t				seq;									/*	序号(资源使用次数)*/
	soft_flag				slt_flgs;								/*	标志 */
	int8_t					padding[2];								/*	对齐*/
};