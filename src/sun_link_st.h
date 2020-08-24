#pragma once

// 定义IOCP 相关的结构体

#include <WinSock2.h>
#include <mutex>
#include <list>
#include "sun_define.h"







//struct sun_send_block
//{
//	char					buffer[MAX_SOCK_BUFFER];
//	size_t					size;
//};
//
////std::list<sun_send_block*> head;								/*	发送队列*/
////unsigned int			lmt_size;								/*	限制大小*/
////unsigned int			cur_size;								/*	当前的大小*/
//
//struct sun_tx {
//	char					buffer[MAX_SOCK_BUFFER];				/*	数据缓冲区*/
//	unsigned short			bufsz;									/*	缓冲区大小*/
//	char			 		padding[6];								/*	对齐*/
//	time_t					mtime;									/*	最后一次修改时间*/
//};

struct sun_link {
	OVERLAPPED				iocp_arg;								/*	参数*/
	/*
	 *	操作 标志			0:接收连接  1:接收 2:发送 
	 */
	uint32_t				lnk_flgs;								/*	标志*/

	/*
	 *  接收不使用
	 *	sending标志			0:空闲中, 1:正在发送
	 */
	uint32_t				sending;								/*	标志*/

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
	/*
	 * link_no = seq<<16 | key
	 */
	uint32_t				link_no;								/*	连接号*/
	/*
	 * key 资源编号
	 */
	uint16_t				key;									/*	KEY值*/
	/*
	 * 资源使用次数
	 */
	uint16_t				seq;									/*	序号*/
	/*
	*	软关闭标志			0:启用 1关闭
	*/
	uint16_t				slt_flgs;								/*	标志*/
	int8_t					padding[6];								/*	对齐*/
};