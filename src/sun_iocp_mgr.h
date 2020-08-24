#pragma once

#include <cstdint>
#include <thread>
#include <list>

#include "sun_define.h"
#include "sun_link_st.h"
/*
iocp 处理类
功能 ,iocp 启动, gqcs, 连接接收, 链接绑定，接收(粘包分离)，发送，接收 功能
 
*/
class sun_iocp_mgr
{
private:
	bool					m_run_flag{ false };						// 服务标志
	HANDLE					m_h_iocp{ nullptr };						// 完成端口句柄

	std::list<std::thread*>	m_list_th_work;								// 工作线程list

public:
	sun_iocp_mgr();
	~sun_iocp_mgr();

	int32_t start_service();
	int32_t stop_service();

	int32_t iocp_bind(sun_socket_st* p_link);
private:

	int32_t do_iocp_work(void);

	// 创建
	// 绑定
	// 监听
	int32_t get_thread_work_num(void);
	
	int32_t create_thread_work(void);

	
	int32_t accept_link(uint64_t key);
	// 关闭 
	void free_link(uint64_t key);
	// 软关闭
	void close_link(uint64_t key);

	int32_t send_done(uint64_t key, uint64_t size);

	int32_t recv_done(uint64_t key, uint64_t size);
};
