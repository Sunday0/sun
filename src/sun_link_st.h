#pragma once

// ����IOCP ��صĽṹ��

#include <WinSock2.h>

constexpr auto MAX_SOCK_BUFFER = 8192;

/*
 *	���� ��־
 */
enum class OLAD_FLAG : uint32_t
{
	ACCEPT, 
	RECV, 
	SEND
};

/*
 *	���� ��־
 */
enum class SEND_FLAG : uint32_t
{
	IDLE,
	BUSY,
};

/*
 *	��ر� ��־
 */
enum class SOFT_FLAG : uint16_t
{
	USING,
	SHUT,
};

struct sun_link {
	OVERLAPPED				iocp_arg;								/*	����*/
	OLAD_FLAG				ol_flgs;								/*	��־*/
	SEND_FLAG				sending;								/*	��־*/
	int8_t					buffer[MAX_SOCK_BUFFER];				/*	���ݻ�����*/
	uint16_t				bufsz;									/*	��������С*/
	int8_t			 		padding[6];								/*	����*/
	time_t					mtime;									/*	���һ���޸�ʱ��*/
};

struct sun_socket_st {
	struct sun_link			rx_head;								/*	��������ͷ*/
	struct sun_link			tx_head;								/*	��������ͷ*/
	int32_t					sock;									/*	SOCKET������*/
	uint32_t				session_id;								/*	��ǰ�ỰID*/
	uint32_t				link_no;								/*	��Ϊ iocp �� key. link_no = seq<<16 | idx */
	uint16_t				idx;									/*	idxֵ*/
	uint16_t				seq;									/*	���(��Դʹ�ô���)*/
	SOFT_FLAG				slt_flgs;								/*	��־ */
	int8_t					padding[6];								/*	����*/
};