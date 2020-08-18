
#include "sun_iocp.h"

#include <WinSock2.h>
#include <cstdio>
#include <thread>
#include <array>

#include "sun_link_st.h"

using namespace std;


sun_iocp::sun_iocp()
{
}

sun_iocp::~sun_iocp()
{
}

int sun_iocp::start_service()
{
	// 创建完成端口
	m_h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (NULL == m_h_iocp)
	{
		std::printf("start_service CreateIoCompletionPort 调用失败");
		return -1;
	}
		
	m_run_flag = true;

	create_thread_work();

	return 0;
}

int sun_iocp::stop_service()
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

int sun_iocp::bind_iocp(uint64_t key)
{
	// 从资源管理获取link资源信息
	// 绑定到完成端口
	// CreateIoCompletionPort((HANDLE)sock, m_h_iocp, (ULONG_PTR)1, 0);
	return 0;
}



int sun_iocp::get_thread_work_num(void)
{
	SYSTEM_INFO sysTemInfo;
	GetSystemInfo(&sysTemInfo);
	return (int)(sysTemInfo.dwNumberOfProcessors << 1);
}

int sun_iocp::create_thread_work(void)
{
	auto num = get_thread_work_num();
	for (auto i = 0; i < num; i++)
	{
		m_list_th_work.push_back(new thread(th_iocp_work, this));
	}
	return 0;
}

int sun_iocp::th_iocp_work(sun_iocp* ptr)
{
	return ptr->do_iocp_work();
}

int sun_iocp::do_iocp_work(void)
{
	DWORD				size;
	ULONG_PTR			key;
	sun_link*			link;

	while (m_run_flag)
	{
		if (GetQueuedCompletionStatus(m_h_iocp, &size, &key,
			(OVERLAPPED**)&link, 1000) != 0) {
			if (size == 0)
			{
				if (link->lnk_flgs)
					close_link(key);
				else
					free_link(key);
			}
			else
			{
				if (link->lnk_flgs)
					send_done(key, size);
				else
					recv_done(key, size);
			}
		}
		else 
		{
			auto rc = GetLastError();
			if (rc != WAIT_TIMEOUT && rc != WSA_IO_PENDING && link != NULL) 
			{
				if (link->lnk_flgs)
					free_link(key);
				else
					close_link(key);
			}
		}
	}

	return 0;
}

// 归还一个连接资源
void sun_iocp::free_link(uint64_t key)
{

}
// 软关闭
void sun_iocp::close_link(uint64_t key)
{

}

int sun_iocp::send_done(uint64_t key, uint64_t size)
{
	return 0;
}

int sun_iocp::recv_done(uint64_t key, uint64_t size)
{
	return 0;
}