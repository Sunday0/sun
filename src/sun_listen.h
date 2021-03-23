#pragma once

#include <cstdint>
#include <thread>
#include <list>
#include <atomic> 
#include "sun_conf.h"

#include "sun_singleton.hpp"

/*
网络处理类
*/

class sun_listen : public sun_singleton<sun_listen>
{
private:
	std::atomic<bool>			m_quit{false};
	std::thread					m_thread;
	std::list<int32_t>			m_lsn_sock;
	uint8_t						m_ip_type{ IPV4 };	// 支持的IP类型，v4 v6
private:
	sun_listen();
public:
	~sun_listen();
	int32_t start();
	int32_t stop();
private:

	int32_t create_listen(int32_t af);
	int32_t do_accept(int32_t s);
	int32_t do_bind(int32_t af, int32_t s, uint16_t port);
	void	do_listen(void);
	int32_t run_thread(void);
};
