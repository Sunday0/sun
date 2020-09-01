
#include "sun_iocp_mgr.h"

#include <WinSock2.h>
#include <cstdio>
#include <thread>
#include <array>

#include "sun_link_mgr.h"
#include "sun_protocol.h"

#pragma warning(disable:4312)

using namespace std;
using namespace std::chrono;


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
		std::printf("start_service CreateIoCompletionPort 调用失败");
		return -1;
	}
		
	m_run_flag = true;

	create_thread_work();

	return 0;
}

int32_t sun_iocp_mgr::stop_service()
{
	m_run_flag = false;

	for (auto &var : m_list_th_work)
	{
		var.join();
	}
	m_list_th_work.clear();

	return 0;
}

int32_t sun_iocp_mgr::iocp_bind(int32_t idx)
{
	sun_socket_st* p_link = m_p_link->get_link_ptr(idx);

	// 从资源管理获取link资源信息
	// 绑定到完成端口
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

	wsabuf.len = MAX_BUFFER - p_socket->rx.bufsz;
	wsabuf.buf = (char*)p_socket->rx.buffer;
	memset(&(p_socket->rx.iocp_arg), 0, sizeof(p_socket->rx.iocp_arg));

	if (WSARecv(p_socket->sock, &wsabuf, 1, &size, &flags, &p_socket->rx.iocp_arg, 0)
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
		m_list_th_work.push_back(thread(fun));
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
		if (0 != GetQueuedCompletionStatus(m_h_iocp, &size, &key, (overlapped**)(&link), 1000))
		{
			// 连接处理
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

// 归还一个连接资源
void sun_iocp_mgr::free_link(uint64_t key)
{

}
// 软关闭
void sun_iocp_mgr::close_link(uint64_t key)
{

}

int32_t sun_iocp_mgr::send_done(uint64_t link_no, uint64_t size)
{
	return 0;
}

int32_t sun_iocp_mgr::recv_done(uint64_t link_no, uint64_t size)
{
	uint16_t idx = link_no & 0x0000FFFF;
	auto link = m_p_link->get_link_ptr(idx);

	link->rx.mtime = system_clock::to_time_t(system_clock::now());;
	link->rx.bufsz += (uint16_t)(size);

	uint16_t off = 0;
	do
	{
		int16_t ret = sun_protocol::analyze(link->rx.buffer + off, link->rx.bufsz);
		if (0 > ret)
		{
			// 校验失败
			// 需要关闭sock, 释放资源
			return -1;
		}
		else if(0 < ret)
		{

			int32_t m_len = ret;
			int8_t buf[8192] = { 0 };

			memcpy(buf, link->rx.buffer, ret);
			off += ret;
			link->rx.bufsz -= (uint16_t)ret;
		}
		else
		{
			break;
		}
	} while (link->rx.bufsz > 0);
	


	// 解析完毕。继续接收
	if (0 > iocp_recv(link))
	{
		// 需要关闭sock, 释放资源
	}

	return 0;
}

int32_t sun_iocp_mgr::data_analyze(uint64_t link_no, sun_link* p_rx)
{
	uint16_t off = 0;
	do
	{
		int16_t ret = sun_protocol::analyze(p_rx->buffer + off, p_rx->bufsz);
		if (0 > ret)
		{
			// 校验失败
			// 需要关闭sock, 释放资源
			return -1;
		}
		else if (0 < ret)
		{
			int8_t buf[8192] = { 0 };

			*((uint64_t*)buf) = link_no;
			memcpy(buf + sizeof(uint64_t), p_rx->buffer + off, ret);

			p_rx->bufsz -= (uint16_t)ret;
			off			+= ret;
		}
		else
		{
			break;
		}
	} while (p_rx->bufsz > 0);

	return 0;
}