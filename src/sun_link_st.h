#pragma once

// 定义IOCP 相关的结构体

#include <WinSock2.h>

constexpr auto MAX_SOCK_BUFFER = 8192;

/*
 *	操作 标志
 */
enum class OLAD_FLAG : uint32_t
{
	ACCEPT, 
	RECV, 
	SEND
};

/*
 *	发送 标志
 */
enum class SEND_FLAG : uint32_t
{
	IDLE,
	BUSY,
};

/*
 *	软关闭 标志
 */
enum class SOFT_FLAG : uint16_t
{
	USING,
	SHUT,
};

struct sun_link {
	OVERLAPPED				iocp_arg;								/*	参数*/
	OLAD_FLAG				ol_flgs;								/*	标志*/
	SEND_FLAG				sending;								/*	标志*/
	int8_t					buffer[MAX_SOCK_BUFFER];				/*	数据缓冲区*/
	uint16_t				bufsz;									/*	缓冲区大小*/
	int8_t			 		padding[6];								/*	对齐*/
	time_t					mtime;									/*	最后一次修改时间*/
};

struct sun_socket_st {
	struct sun_link			rx_head;								/*	接收连接头*/
	struct sun_link			tx_head;								/*	发送连接头*/
	int32_t					sock;									/*	SOCKET描述符*/
	uint32_t				session_id;								/*	当前会话ID*/
	uint32_t				link_no;								/*	作为 iocp 的 key. link_no = seq<<16 | idx */
	uint16_t				idx;									/*	idx值*/
	uint16_t				seq;									/*	序号(资源使用次数)*/
	SOFT_FLAG				slt_flgs;								/*	标志 */
	int8_t					padding[6];								/*	对齐*/
};