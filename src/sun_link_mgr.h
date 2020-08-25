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
	/*
	 * 资源锁
	 */
	std::mutex						m_lock_mgr;
	
	std::mutex*						m_lock_arr;
	sun_socket_st*					m_link_arr;
	uint16_t*						m_res_arr;

	//发送队列, 后面用chunk 管理 固定长度
	//list<>						

	// 资源 空闲数量 资源读写偏移
	uint16_t						m_res_idles;
	uint16_t						m_res_r;
	uint16_t						m_res_w;

	// 页管理池
public:
	sun_link_mgr();
	~sun_link_mgr();

	// 申请资源对象
	sun_socket_st * alloc_link();

	// 释放资源对象
	void free_link(uint32_t link_no);

	// 判断资源对象
	bool is_invalid_link(uint32_t link_no);
	
	// 获取资源对象地址
	sun_socket_st* get_link_ptr(int32_t idx);

public:
	int32_t initialize(void);
	int32_t destroy(void);
};
