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
class sun_iocp_mgr
{
private:
	bool					m_run_flag{ false };						// �����־
	HANDLE					m_h_iocp{ nullptr };						// ��ɶ˿ھ��

//	uint8_t					m_ip_type{ IPV4 };							// ֧�ֵ�IP���ͣ�v4 v6
//	uint16_t				m_port{ 20000 };							// �����˿�
//	int						m_max_client{ 10000 };						// ���ͻ�������

	std::list<std::thread*>	m_list_th_work;								// �����߳�list

public:
	sun_iocp_mgr();
	~sun_iocp_mgr();

	int32_t start_service();
	int32_t stop_service();

	int32_t iocp_bind(sun_socket_st* p_link);
private:
	static int32_t th_iocp_work(sun_iocp_mgr* ptr);

	int32_t do_iocp_work(void);

	// ����
	// ��
	// ����
	int32_t get_thread_work_num(void);
	
	int32_t create_thread_work(void);

	

	// �ر� 
	void free_link(uint64_t key);
	// ��ر�
	void close_link(uint64_t key);

	int32_t send_done(uint64_t key, uint64_t size);

	int32_t recv_done(uint64_t key, uint64_t size);
};
