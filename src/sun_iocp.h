#pragma once

#include <cstdint>
#include <thread>
#include <list>

#include "sun_conf.h"
#include "sun_link_st.h"

#include "sun_singleton.hpp"

/*
iocp 处理类
功能 ,iocp 启动, gqcs, 连接接收, 链接绑定，接收(粘包分离)，发送，接收 功能
 
*/
class sun_link_mgr;
class sun_iocp : public sun_singleton<sun_iocp>
{
private:
	std::atomic<bool>		m_quit{ false };
	HANDLE					m_h_iocp{ nullptr };						// 完成端口句柄
	std::list<std::thread>	m_list_th_work;								// 工作线程list
public:
	sun_iocp();
	~sun_iocp();

	int32_t start_service(sun_link_mgr* p_link);
	int32_t stop_service();

	int32_t iocp_bind(int32_t idx);
	int32_t iocp_send(uint32_t link_no, int8_t* buff, int32_t len);

private:
	int32_t iocp_recv(sun_socket_st * p_socket);
	int32_t iocp_send(sun_socket_st* p_socket);
private:

	int32_t do_iocp_work(void);

	void create_work_threads(void);
	
	int32_t accept_link(uint32_t link_no);
	// 关闭 
	void free_link(uint32_t link_no);
	// 软关闭
	void close_link(uint32_t link_no);

	int32_t send_done(uint32_t link_no, uint64_t size);

	int32_t recv_done(uint32_t link_no, uint64_t size);

	int32_t data_analyze(uint32_t link_no, sun_link* p_rx);
};
