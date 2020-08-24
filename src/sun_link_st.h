#pragma once

// ����IOCP ��صĽṹ��

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
////std::list<sun_send_block*> head;								/*	���Ͷ���*/
////unsigned int			lmt_size;								/*	���ƴ�С*/
////unsigned int			cur_size;								/*	��ǰ�Ĵ�С*/
//
//struct sun_tx {
//	char					buffer[MAX_SOCK_BUFFER];				/*	���ݻ�����*/
//	unsigned short			bufsz;									/*	��������С*/
//	char			 		padding[6];								/*	����*/
//	time_t					mtime;									/*	���һ���޸�ʱ��*/
//};

struct sun_link {
	OVERLAPPED				iocp_arg;								/*	����*/
	/*
	 *	���� ��־			0:��������  1:���� 2:���� 
	 */
	uint32_t				lnk_flgs;								/*	��־*/

	/*
	 *  ���ղ�ʹ��
	 *	sending��־			0:������, 1:���ڷ���
	 */
	uint32_t				sending;								/*	��־*/

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
	/*
	 * link_no = seq<<16 | key
	 */
	uint32_t				link_no;								/*	���Ӻ�*/
	/*
	 * key ��Դ���
	 */
	uint16_t				key;									/*	KEYֵ*/
	/*
	 * ��Դʹ�ô���
	 */
	uint16_t				seq;									/*	���*/
	/*
	*	��رձ�־			0:���� 1�ر�
	*/
	uint16_t				slt_flgs;								/*	��־*/
	int8_t					padding[6];								/*	����*/
};