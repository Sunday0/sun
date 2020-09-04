
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
	auto p_link = m_p_link->get_link_ptr(idx);

	// 从资源管理获取link资源信息
	// 绑定到完成端口
	if (!CreateIoCompletionPort((HANDLE)(p_link->sock), m_h_iocp, (ULONG_PTR)(p_link->link_no), 0))
	{
		return -1;
	}

	return iocp_recv(p_link);
}

int32_t sun_iocp_mgr::iocp_send(uint32_t link_no, int8_t* buff, int32_t len)
{
	sun_socket_st*				p_link;
	mutex*						p_mutx;
	std::list<sun_buff*>*		p_list;
	sun_men_pool<sun_buff>*		p_pool;
	int32_t						idx = GET_IDX(link_no);

	std::tie(p_link, p_mutx, p_list, p_pool) = m_p_link->get_link_tuple(idx);
	
	{
		std::lock_guard<std::mutex> lck(*p_mutx);

		if (p_link->link_no != link_no)
		{
			return -1;
		}

		if (p_link->slt_flgs == soft_flag::shut)
		{
			return -1;
		}

		if (p_link->tx.sending == send_flag::busy)
		{
			if (p_list->size() > 0)
			{
				auto data = p_list->back();
				if (MAX_BUFFER - data->len >= len)
				{
					memcpy(data->mem + data->len, buff, len);
					data->len += len;
				}
				else
				{
					// 放入发送队列
					auto data = p_pool->alloc();
					data->len = len;
					memcpy(data->mem, buff, len);
					p_list->push_back(data);
				}
			}
			else
			{
				// 放入发送队列
				auto data = p_pool->alloc();
				data->len = len;
				memcpy(data->mem, buff, len);
				p_list->push_back(data);
			}
		}
	}
	
	// 发送标志

	/*if (0 > iocp_send(p_link))
	{
		close_link(link_no);
	}*/

	return 0;
}

int32_t sun_iocp_mgr::iocp_recv(sun_socket_st* p_socket)
{
	WSABUF	wsabuf;
	DWORD	size = 0;
	DWORD	flags = 0;

	wsabuf.len = MAX_BUFFER - p_socket->rx.bufsz;
	wsabuf.buf = (char*)(p_socket->rx.buffer + p_socket->rx.bufsz);
	memset(&(p_socket->rx.iocp_arg), 0, sizeof(p_socket->rx.iocp_arg));

	if (WSARecv(p_socket->sock, &wsabuf, 1, &size, &flags, &p_socket->rx.iocp_arg, 0)
		&& GetLastError() != WSA_IO_PENDING)
	{
		return -1;
	}

	return 0;
}

int32_t sun_iocp_mgr::iocp_send(sun_socket_st* p_socket)
{
	WSABUF	wsabuf;
	DWORD	size = 0;

	wsabuf.buf = (char*)(p_socket->tx.buffer);
	wsabuf.len = p_socket->tx.bufsz;

	p_socket->tx.mtime = system_clock::to_time_t(system_clock::now());

	memset(&(p_socket->tx.iocp_arg), 0, sizeof(overlapped));
	if (WSASend(p_socket->sock, &wsabuf, 1, &size, 0,
		&(p_socket->tx.iocp_arg), 0) && GetLastError() != WSA_IO_PENDING)
		return -1;
	else
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
	uint32_t			link_no;

	while (m_run_flag)
	{
		link = nullptr;
		if (0 != GetQueuedCompletionStatus(m_h_iocp, &size, &key, (overlapped**)(&link), 1000))
		{
			link_no = (uint32_t)key;
			// 连接处理
			switch (link->ol_flgs)
			{
			case olad_flag::accept:
				accept_link(link_no);
				break;
			case olad_flag::recv:
				recv_done(link_no, size);
				break;
			case olad_flag::send:
				send_done(link_no, size);
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
				link_no = (uint32_t)key;
				switch (link->ol_flgs)
				{
				case olad_flag::accept:
					break;
				case olad_flag::recv:
					free_link(link_no);
					break;
				case olad_flag::send:
					close_link(link_no);
					break;
				default:
					break;
				}
			}
		}
	}

	return 0;
}

int32_t sun_iocp_mgr::accept_link(uint32_t link_no)
{

	//WSAAccept();

	return 0;
}

// 归还一个连接资源
void sun_iocp_mgr::free_link(uint32_t link_no)
{

}
// 软关闭
void sun_iocp_mgr::close_link(uint32_t link_no)
{

}

int32_t sun_iocp_mgr::send_done(uint32_t link_no, uint64_t size)
{
	return 0;
}

int32_t sun_iocp_mgr::recv_done(uint32_t link_no, uint64_t size)
{
	auto idx = GET_IDX(link_no);
	auto link = m_p_link->get_link_ptr(idx);

	link->rx.mtime = system_clock::to_time_t(system_clock::now());;
	link->rx.bufsz += (uint16_t)(size);

	auto off = data_analyze(link_no, &link->rx);
	if (0 < off)
	{
		link->rx.bufsz -= (uint16_t)off;
		memmove(link->rx.buffer, link->rx.buffer + off, link->rx.bufsz);
	}
	else if(0 == off)
	{
		off = iocp_recv(link);
	}

	if (0 > off)
	{
		free_link(link_no);
	}

	return 0;
}

int32_t sun_iocp_mgr::data_analyze(uint32_t link_no, sun_link* p_rx)
{
	int16_t off = 0;
	auto	len = p_rx->bufsz;
	do
	{
		auto ret = sun_protocol::analyze(p_rx->buffer + off, len);
		if (0 > ret)
		{
			return -1;
		}
		else if (0 == ret)
		{
			break;
		}
		else if (0 < ret)
		{
			/*****************/
			/** 拷贝数据到消息队列 **/
			int8_t buf[8192] = { 0 };

			*((uint64_t*)buf) = link_no;
			memcpy(buf + sizeof(uint64_t), p_rx->buffer + off, ret);

			/*****************/
			len -= (uint16_t)ret;
			off	+= ret;
		}
	} while (len > 0);

	return off;
}