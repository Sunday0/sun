#pragma once

#include <cstdint>
#include <thread>
#include <list>

#include "sun_define.h"
#include "sun_link_st.h"
/*
iocp ������
���� ,iocp ����, gqcs, ���ӽ���, ���Ӱ󶨣�����(ճ������)�����ͣ����� ����
 
*/
class sun_link_mgr;
class sun_iocp_mgr
{
private:
	bool					m_run_flag{ false };						// �����־
	HANDLE					m_h_iocp{ nullptr };						// ��ɶ˿ھ��
	sun_link_mgr*			m_p_link{ nullptr };
	
	std::list<std::thread*>	m_list_th_work;								// �����߳�list
public:
	sun_iocp_mgr();
	~sun_iocp_mgr();

	int32_t start_service(sun_link_mgr* p_link);
	int32_t stop_service();

	int32_t iocp_bind(int32_t idx);
	int32_t iocp_recv(sun_socket_st * p_socket);
	int32_t iocp_send(int32_t idx);
private:

	int32_t do_iocp_work(void);

	// ����
	// ��
	// ����
	int32_t get_thread_work_num(void);
	
	int32_t create_thread_work(void);

	
	int32_t accept_link(uint64_t key);
	// �ر� 
	void free_link(uint64_t key);
	// ��ر�
	void close_link(uint64_t key);

	int32_t send_done(uint64_t key, uint64_t size);

	int32_t recv_done(uint64_t key, uint64_t size);
};
