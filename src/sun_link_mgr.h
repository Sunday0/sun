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
	uint16_t*						m_res_arr;
	uint16_t						m_res_idles; 
	uint16_t						m_res_r;
	uint16_t						m_res_w;
public:
	sun_link_mgr();
	~sun_link_mgr();

	// 申请资源对象
	sun_socket_st * alloc_link();

	// 释放资源对象
	void free_link(uint32_t link_no);

	// 判断资源对象
	bool is_invalid_link(uint32_t link_no);

public:
	int32_t initialize(void);
	int32_t destroy(void);
};
