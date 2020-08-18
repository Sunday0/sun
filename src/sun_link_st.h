#pragma once

// ����IOCP ��صĽṹ��

#include <WinSock2.h>
#include <mutex>
#include <list>
#include "sun_define.h"







struct sun_send_block
{
	char					buffer[MAX_SOCK_BUFFER];
	size_t					size;
};

/// ��������
struct sun_rx {
	char					buffer[MAX_SOCK_BUFFER];				/*	���ݻ�����*/
	unsigned short			bufsz;									/*	��������С*/
	char			 		padding[6];								/*	����*/
	time_t					mtime;									/*	���һ���޸�ʱ��*/
};

struct sun_tx {
	char					buffer[MAX_SOCK_BUFFER];				/*	���ݻ�����*/
	unsigned short			bufsz;									/*	��������С*/
	char			 		padding[6];								/*	����*/
	time_t					mtime;									/*	���һ���޸�ʱ��*/
	std::list<sun_send_block*> head;								/*	���Ͷ���*/
	unsigned int			lmt_size;								/*	���ƴ�С*/
	unsigned int			cur_size;								/*	��ǰ�Ĵ�С*/
};

struct sun_link {
	OVERLAPPED				iocp_arg;								/*	����*/
	
	/*
	 *	����/���� ��־		0:���� 1:����
	 */
	unsigned int			lnk_flgs;								/*	��־*/

	/*
	 *  ���ղ�ʹ��
	 *	sending��־			0:������, 1:���ڷ���
	 */
	unsigned int			sending;								/*	��־*/
	union {
		struct sun_rx*		rx;										/*	����*/
		struct sun_tx*		tx;										/*	����*/
	};
};


struct sun_socket_st {
	struct sun_link			rx_head;								/*	��������ͷ*/
	struct sun_link			tx_head;								/*	��������ͷ*/
	int						sock;									/*	SOCKET������*/
	unsigned int			session_id;								/*	��ǰ�ỰID*/
	/*
	 * link_no = seq<<16 | key
	 */
	unsigned int			link_no;								/*	���Ӻ�*/
	/*
	 * key ��Դ���
	 */
	unsigned short			key;									/*	KEYֵ*/
	/*
	 * ��Դʹ�ô���
	 */
	unsigned short			seq;									/*	���*/
	/*
	*	��رձ�־			0:���� 1�ر�
	*/
	unsigned short			slt_flgs;								/*	��־*/
	char					padding[6];								/*	����*/
};