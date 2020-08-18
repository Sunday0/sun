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
class sun_iocp
{
private:
	bool					m_run_flag{ false };						// �����־
	HANDLE					m_h_iocp{ nullptr };						// ��ɶ˿ھ��

//	uint8_t					m_ip_type{ IPV4 };							// ֧�ֵ�IP���ͣ�v4 v6
//	uint16_t				m_port{ 20000 };							// �����˿�
//	int						m_max_client{ 10000 };						// ���ͻ�������

	std::list<std::thread*>	m_list_th_work;								// �����߳�list

public:
	sun_iocp();
	~sun_iocp();

	int start_service();
	int stop_service();

	int bind_iocp(uint64_t key);
private:
	static int th_iocp_work(sun_iocp* ptr);

	int do_iocp_work(void);

	// ����
	// ��
	// ����
	int get_thread_work_num(void);
	
	int create_thread_work(void);

	

	// �ر� 
	void free_link(uint64_t key);
	// ��ر�
	void close_link(uint64_t key);

	int send_done(uint64_t key, uint64_t size);

	int recv_done(uint64_t key, uint64_t size);
};
