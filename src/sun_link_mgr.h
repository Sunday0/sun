#pragma once

#include <WinSock2.h>
#include <tuple>
#include "sun_conf.h"
#include "sun_link_st.h"
#include "sun_pool.hpp"

#include "sun_singleton.hpp"

constexpr auto MAX_BUFFER = 8192;

struct sun_buff
{
	int32_t len;
	int8_t mem[MAX_BUFFER];
};

using sun_link_tuple = std::tuple<sun_link*, std::mutex*, std::list<sun_buff*>*, sun_men_pool<sun_buff>*>;

class sun_iocp;

class sun_link_mgr : public sun_singleton<sun_link_mgr>
{
private:
	/*
	 * 资源锁
	 */
	std::mutex						m_lock_mgr;
	
	std::mutex*						m_lock_arr;
	sun_link*						m_link_arr;
	uint16_t*						m_res_arr;

	// 发送队列管理池
	// 每一个连接对应一个pool
	std::list<sun_buff*>*			m_data_list;				// 数据队列表
	sun_men_pool<sun_buff>*			m_link_pool;

	// 资源 空闲数量 资源读写偏移
	uint16_t						m_res_idles;
	uint16_t						m_res_r;
	uint16_t						m_res_w;

public:
	sun_link_mgr();
	~sun_link_mgr();

	// 申请资源对象
	sun_link* alloc_link();

	// 释放资源对象
	void free_link(uint32_t link_no);

	// 软关闭资源对象
	void close_link(uint32_t link_no);

	// 判断资源对象
	bool is_invalid_link(uint32_t link_no);
	
	// 获取资源对象地址
	sun_link* get_link_ptr(uint32_t idx);


	sun_men_pool<sun_buff>* get_link_pool(uint32_t idx);

	sun_link_tuple get_link_tuple(uint32_t idx);

public:
	int32_t initialize(void);
	int32_t destroy(void);

	void free_res(uint32_t idx);
	void close_link(uint32_t link_no, uint32_t idx);
	bool is_invalid_link(uint32_t link_no, uint32_t idx);
};
