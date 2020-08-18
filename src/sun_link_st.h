#pragma once

// 定义IOCP 相关的结构体

#include <WinSock2.h>
#include <mutex>
#include <list>
#include "sun_define.h"







struct sun_send_block
{
	char					buffer[MAX_SOCK_BUFFER];
	size_t					size;
};

/// 接收描述
struct sun_rx {
	char					buffer[MAX_SOCK_BUFFER];				/*	数据缓冲区*/
	unsigned short			bufsz;									/*	缓冲区大小*/
	char			 		padding[6];								/*	对齐*/
	time_t					mtime;									/*	最后一次修改时间*/
};

struct sun_tx {
	char					buffer[MAX_SOCK_BUFFER];				/*	数据缓冲区*/
	unsigned short			bufsz;									/*	缓冲区大小*/
	char			 		padding[6];								/*	对齐*/
	time_t					mtime;									/*	最后一次修改时间*/
	std::list<sun_send_block*> head;								/*	发送队列*/
	unsigned int			lmt_size;								/*	限制大小*/
	unsigned int			cur_size;								/*	当前的大小*/
};

struct sun_link {
	OVERLAPPED				iocp_arg;								/*	参数*/
	
	/*
	 *	发送/接收 标志		0:接收 1:发送
	 */
	unsigned int			lnk_flgs;								/*	标志*/

	/*
	 *  接收不使用
	 *	sending标志			0:空闲中, 1:正在发送
	 */
	unsigned int			sending;								/*	标志*/
	union {
		struct sun_rx*		rx;										/*	接收*/
		struct sun_tx*		tx;										/*	发送*/
	};
};


struct sun_socket_st {
	struct sun_link			rx_head;								/*	接收连接头*/
	struct sun_link			tx_head;								/*	发送连接头*/
	int						sock;									/*	SOCKET描述符*/
	unsigned int			session_id;								/*	当前会话ID*/
	/*
	 * link_no = seq<<16 | key
	 */
	unsigned int			link_no;								/*	连接号*/
	/*
	 * key 资源编号
	 */
	unsigned short			key;									/*	KEY值*/
	/*
	 * 资源使用次数
	 */
	unsigned short			seq;									/*	序号*/
	/*
	*	软关闭标志			0:启用 1关闭
	*/
	unsigned short			slt_flgs;								/*	标志*/
	char					padding[6];								/*	对齐*/
};