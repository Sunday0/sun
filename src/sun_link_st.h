#pragma once

// ����IOCP ��صĽṹ��

#include <WinSock2.h>
#include <cstdint>

#define GET_IDX(link_no) (link_no & 0x0000FFFF)

constexpr auto MAX_SOCK_BUFFER = 8192;

using overlapped = OVERLAPPED;

/*
 *	���� ��־
 */
enum class olad_flag : uint32_t
{
	accept, 
	recv, 
	send
};

/*
 *	������ ��־
 */
enum class send_flag : uint32_t
{
	idle,
	busy,
};

/*
 *	��ر� ��־
 */
enum class soft_flag : uint16_t
{
	used,
	shut,
};

struct sun_olad {
	overlapped				iocp_arg;								/*	����*/
	olad_flag				ol_flgs;								/*	��־*/
	send_flag				sending;								/*	��־*/
	int8_t					buffer[MAX_SOCK_BUFFER];				/*	���ݻ�����*/
	uint16_t				bufsz;									/*	��������С*/
	int8_t			 		padding[6];								/*	����*/
	time_t					mtime;									/*	���һ���޸�ʱ��*/
};

struct sun_link {
	sun_olad				rx;										/*	��������ͷ*/
	sun_olad				tx;										/*	��������ͷ*/
	int32_t					sock;									/*	SOCKET������*/
	uint32_t				link_no;								/*	��Ϊ iocp �� key. link_no = seq<<16 | idx */
	uint16_t				idx;									/*	idxֵ*/
	uint16_t				seq;									/*	���(��Դʹ�ô���)*/
	soft_flag				slt_flgs;								/*	��־ */
	int8_t					padding[2];								/*	����*/
};