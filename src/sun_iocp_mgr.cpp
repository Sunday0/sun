
#include "sun_iocp_mgr.h"

#include <WinSock2.h>
#include <cstdio>
#include <thread>
#include <array>

#include "sun_link_mgr.h"

using namespace std;
using namespace std::chrono;

#pragma warning(disable:4312)

sun_iocp_mgr::sun_iocp_mgr()
{
}

sun_iocp_mgr::~sun_iocp_mgr()
{
}

int32_t sun_iocp_mgr::start_service(sun_link_mgr* p_link)
{
	m_p_link = p_link;

	m_h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (nullptr == m_h_iocp)
	{
		std::printf("start_service CreateIoCompletionPort ����ʧ��");
		return -1;
	}
		
	m_run_flag = true;

	create_thread_work();

	return 0;
}

int32_t sun_iocp_mgr::stop_service()
{
	m_run_flag = false;

	for each (auto var in m_list_th_work)
	{
		var->join();
		delete var;
	}
	m_list_th_work.clear();

	return 0;
}

int32_t sun_iocp_mgr::iocp_bind(int32_t idx)
{
	sun_socket_st* p_link = m_p_link->get_link_ptr(idx);

	// ����Դ�����ȡlink��Դ��Ϣ
	// �󶨵���ɶ˿�
	if (!CreateIoCompletionPort((HANDLE)(p_link->sock), m_h_iocp, (ULONG_PTR)(p_link->link_no), 0))
	{
		return -1;
	}

	return iocp_recv(p_link);
}

int32_t sun_iocp_mgr::iocp_recv(sun_socket_st* p_socket)
{
	WSABUF	wsabuf;
	DWORD	size = 0;
	DWORD	flags = 0;

	wsabuf.len = MAX_BUFFER - p_socket->rx_head.bufsz;
	wsabuf.buf = (char*)p_socket->rx_head.buffer;
	memset(&(p_socket->rx_head.iocp_arg), 0, sizeof(p_socket->rx_head.iocp_arg));

	if (WSARecv(p_socket->sock, &wsabuf, 1, &size, &flags, &p_socket->rx_head.iocp_arg, 0)
		&& GetLastError() != WSA_IO_PENDING)
	{
		return -1;
	}

	return 0;
}

int32_t sun_iocp_mgr::iocp_send(int32_t idx)
{
	return 0;
}

int32_t sun_iocp_mgr::get_thread_work_num(void)
{
	SYSTEM_INFO sysTemInfo;
	GetSystemInfo(&sysTemInfo);
	return (int32_t)(sysTemInfo.dwNumberOfProcessors << 1);
}

int32_t sun_iocp_mgr::create_thread_work(void)
{
	auto fun = [this]() {
		do_iocp_work();
	};

	auto num = get_thread_work_num();
	for (auto i = 0; i < num; i++)
	{
		m_list_th_work.push_back(new thread(fun));
	}
	return 0;
}

int32_t sun_iocp_mgr::do_iocp_work(void)
{
	DWORD				size;
	ULONG_PTR			key;
	sun_link*			link;

	while (m_run_flag)
	{
		link = nullptr;
		if (0 != GetQueuedCompletionStatus(m_h_iocp, &size, &key, (OVERLAPPED **)(&link), 1000))
		{
			// ���Ӵ���
			switch (link->ol_flgs)
			{
			case olad_flag::accept:
				accept_link(key);
				break;
			case olad_flag::recv:
				recv_done(key, size);
				break;
			case olad_flag::send:
				send_done(key, size);
				break;
			default:
				break;
			}
		}
		else 
		{
			auto rc = GetLastError();
			if (rc != WAIT_TIMEOUT && rc != WSA_IO_PENDING && link != NULL) 
			{
				switch (link->ol_flgs)
				{
				case olad_flag::accept:
					break;
				case olad_flag::recv:
					free_link(key);
					break;
				case olad_flag::send:
					close_link(key);
					break;
				default:
					break;
				}
			}
		}
	}

	return 0;
}

int32_t sun_iocp_mgr::accept_link(uint64_t key)
{

	//WSAAccept();

	return 0;
}

// �黹һ��������Դ
void sun_iocp_mgr::free_link(uint64_t key)
{

}
// ��ر�
void sun_iocp_mgr::close_link(uint64_t key)
{

}

int32_t sun_iocp_mgr::send_done(uint64_t key, uint64_t size)
{
	return 0;
}

int32_t sun_iocp_mgr::recv_done(uint64_t key, uint64_t size)
{
	uint16_t idx = key & 0x0000FFFF;
	auto link = m_p_link->get_link_ptr(idx);

	link->rx_head.mtime = system_clock::to_time_t(system_clock::now());;
	link->rx_head.bufsz += (uint16_t)(size);

	// ���ݰ���, �ŵ�������Ϣ����
	// ʧ�ܵĻ�, ��Ҫ�Ͽ�����

	// ������������
	if (0 > iocp_recv(link))
	{
		// ��Ҫ�ر�sock, �ͷ���Դ
	}

	return 0;
}