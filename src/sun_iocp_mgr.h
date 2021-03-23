#pragma once

#include <cstdint>
#include <thread>
#include <list>

#include "sun_conf.h"
#include "sun_link_st.h"

/*
iocp 处理类
功能 ,iocp 启动, gqcs, 连接接收, 链接绑定，接收(粘包分离)，发送，接收 功能
 
*/
class sun_link_mgr;
class sun_iocp_mgr
{
private:
	bool					m_run_flag{ false };						// 服务标志
	HANDLE					m_h_iocp{ nullptr };						// 完成端口句柄
	sun_link_mgr*			m_p_link{ nullptr };
	
	std::list<std::thread>	m_list_th_work;								// 工作线程list
public:
	sun_iocp_mgr();
	~sun_iocp_mgr();

	int32_t start_service(sun_link_mgr* p_link);
	int32_t stop_service();

	int32_t iocp_bind(int32_t idx);
	int32_t iocp_send(uint32_t link_no, int8_t* buff, int32_t len);

private:
	int32_t iocp_recv(sun_socket_st * p_socket);
	int32_t iocp_send(sun_socket_st* p_socket);
private:

	int32_t do_iocp_work(void);

	int32_t create_thread_work(void);
	
	int32_t accept_link(uint32_t link_no);
	// 关闭 
	void free_link(uint32_t link_no);
	// 软关闭
	void close_link(uint32_t link_no);

	int32_t send_done(uint32_t link_no, uint64_t size);

	int32_t recv_done(uint32_t link_no, uint64_t size);

	int32_t data_analyze(uint32_t link_no, sun_link* p_rx);
};
