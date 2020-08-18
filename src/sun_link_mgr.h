#pragma once

#include <WinSock2.h>
#include <cstdint>
#include <mutex>
#include "sun_link_st.h"

constexpr auto MAX_BUFFER = 8192;

class sun_iocp;

class sun_link_mgr
{
private:
	std::mutex						m_lock_mgr;

	std::mutex*						m_lock_arr;
	sun_socket_st*					m_link_arr;
	sun_tx*							m_tx_arr;
	sun_rx*							m_rx_arr;
	uint16_t*						m_link_no_arr;

	uint16_t*						m_link_no_r;
	uint16_t*						m_link_no_w;
public:
	sun_link_mgr();
	~sun_link_mgr();

	// ���� ���� socket
	int create_listen_socket(void);

	// �󶨼���
	int bind_and_listen(int sock);

	// ������Դ����
	sun_socket_st * alloc_link();

	// �ͷ���Դ����
	void free_link(int link_no);

	// �ж���Դ����
	bool is_invalid_link(int link_no);
private:
	int initialize(void);
	int destroy(void);
};
