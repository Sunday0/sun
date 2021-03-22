#pragma once

#include <cstdint>
#include <thread>
#include <list>

#include "sun_conf.h"

/*
网络处理类
*/

class sun_iocp_mgr;
class sun_link_mgr;
class sun_listen
{
private:
	std::thread					m_thread;
	std::list<int32_t>			m_lsn_sock;
	uint8_t						m_ip_type{ IPV4 };	// 支持的IP类型，v4 v6
	sun_iocp_mgr*				m_p_iocp{ nullptr };
	sun_link_mgr*				m_p_link{ nullptr };
public:
	sun_listen();
	~sun_listen();
	int32_t start_listen(sun_iocp_mgr* p_iocp, sun_link_mgr* p_link);
	int32_t stop_listen();
private:

	int32_t create_listen(int32_t af);
	int32_t do_accept(int32_t s);
	int32_t sun_bind(int32_t af, int32_t s, uint16_t port);
	int32_t do_listen_work(void);
};
