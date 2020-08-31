#include "sun_listen.h"

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <algorithm>
#include <chrono>
#include "sun_link_mgr.h"
#include "sun_iocp_mgr.h"

using namespace std::chrono;

sun_listen::sun_listen()
{

}

sun_listen::~sun_listen()
{
	
}

int32_t sun_listen::start_listen(sun_iocp_mgr* p_iocp, sun_link_mgr* p_link)
{
	m_p_iocp = p_iocp;
	m_p_link = p_link;

	// 创建监听socket
	if (0 != init_listen())
	{
		return -1;
	}


	// 启动监听线程
	//m_th_lsn = std::move(std::thread([this() { do_listen_work ()}]));

	return 0;
}

int32_t sun_listen::stop_listen()
{
	// 关闭监听socket

	// 推出监听线程
	//m_th_lsn.join();

	return 0;

}

int32_t sun_listen::init_listen(void)
{
	// 创建监听socket
	auto s = sk_create(AF_INET);

	if (s < 0)
	{
		std::printf("sk_create 调用失败");
		return -1;
	}

	if (0 > sk_bind(AF_INET, s, PORT))
	{
		std::printf("sk_bind fail\n");
		closesocket(s);
		return -1;
	}
	
	if (0 > sk_listen(s))
	{
		std::printf("sk_listen fail\n");
		closesocket(s);
		return -1;
	}

	m_lsn.push_back(s);
	return 0;
}

int32_t sun_listen::do_listen_work(void)
{
	fd_set				fds;
	struct timeval		t_out{1, 0};
	while (0)
	{
		FD_ZERO(&fds);

		std::for_each(m_lsn.cbegin(), m_lsn.cend(), [&fds](const int32_t& var) {
				FD_SET(var, &fds);
		});

		if (0 >= select(0, &fds, NULL, NULL, &t_out))
		{
			continue;
		}

		for (auto var : m_lsn)
		{
			if (!FD_ISSET(var, &fds))
			{
				continue;
			}

			auto idx = do_accept((int32_t)var);
			if (0 < idx)
			{
				// join iocp
				m_p_iocp->iocp_bind(idx);
			}
		}
	}

	return 0;
}

int32_t sun_listen::do_accept(int32_t s)
{
	struct sockaddr_storage	addr;
	int32_t 				addr_len = sizeof(addr);

	auto client = (int32_t)accept(s, (struct sockaddr*)&addr, &addr_len);

	if (client < 0)
	{
		return -1;
	}

	auto ptr = m_p_link->alloc_link();
	if (nullptr == ptr)
	{
		// 连接数已达上限
		closesocket(client);
		return -1;
	}

	auto tmp_time = system_clock::to_time_t(system_clock::now());

	ptr->rx_head.mtime = tmp_time;
	ptr->tx_head.mtime = tmp_time;
	ptr->sock = client;
	ptr->link_no = ((uint32_t)ptr->seq) << 16 | ptr->idx;
	ptr->seq++;
	ptr->slt_flgs = soft_flag::used;
	return ptr->idx;
}

int32_t sun_listen::sk_create(int32_t af)
{
	return (int32_t)WSASocket(af, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
}

int32_t sun_listen::sk_bind(int32_t af, int32_t s, uint16_t port)
{
	struct sockaddr_storage	addr = { 0 };
	struct sockaddr_in*		sa_in4;
	struct sockaddr_in6*	sa_in6;
	int						nlen;

	switch (af)
	{
	case AF_INET:
		sa_in4 = (struct sockaddr_in*)(&addr);
		sa_in4->sin_family = AF_INET;
		sa_in4->sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		sa_in4->sin_port = htons(port);
		nlen = sizeof(struct sockaddr_in);
		break;
	case AF_INET6:
		sa_in6 = (struct sockaddr_in6*)(&addr);
		sa_in6->sin6_family = AF_INET6;
		sa_in6->sin6_addr = in6addr_any;
		sa_in6->sin6_port = htons(port);
		nlen = sizeof(struct sockaddr_in6);
		break;
	default:
		SetLastError(WSAEAFNOSUPPORT);
		return -1;
	}
	
	return bind(s, (struct sockaddr*)&addr, nlen);
}

int32_t sun_listen::sk_listen(int32_t s)
{
	return listen(s, SOMAXCONN);
}
