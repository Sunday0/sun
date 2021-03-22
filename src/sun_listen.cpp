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

	if (m_ip_type & IPV4)
	{
		create_listen(AF_INET);
	}

	if (m_ip_type & IPV6)
	{
		create_listen(AF_INET6);
	}

	// 启动监听线程
	m_thread = std::move(std::thread([this]() { this->do_listen_work(); }));

	return 0;
}

int32_t sun_listen::stop_listen()
{
	// 关闭监听socket
	for (auto s : m_lsn_sock)
	{
		closesocket(s);
	}

	// 推出监听线程
	m_thread.join();

	return 0;

}

int32_t sun_listen::create_listen(int32_t af)
{
	auto s = (int32_t)socket(af, SOCK_STREAM, 0);
	if (s < 0)
	{
		std::printf("create fail");
		return -1;
	}

	if (0 > sun_bind(AF_INET, s, PORT))
	{
		std::printf("bind fail\n");
		closesocket(s);
		return -1;
	}

	if (0 > listen(s, MAX_LINKS))
	{
		std::printf("listen fail\n");
		closesocket(s);
		return -1;
	}

	m_lsn_sock.push_back(s);
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

	// 剩下的代码应该交给连接管理

	auto ptr = m_p_link->alloc_link();
	if (nullptr == ptr)
	{
		// 连接数已达上限
		closesocket(client);
		return -1;
	}

	auto tmp_time = system_clock::to_time_t(system_clock::now());

	ptr->rx.mtime = tmp_time;
	ptr->tx.mtime = tmp_time;
	ptr->sock = client;
	ptr->link_no = ((uint32_t)ptr->seq) << 16 | ptr->idx;
	ptr->seq++;
	ptr->slt_flgs = soft_flag::used;
	return ptr->idx;
}


int32_t sun_listen::sun_bind(int32_t af, int32_t s, uint16_t port)
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

int32_t sun_listen::do_listen_work(void)
{
	fd_set				fds;
	struct timeval		t_out { 1, 0 };
	while (0)
	{
		FD_ZERO(&fds);

		std::for_each(m_lsn_sock.cbegin(), m_lsn_sock.cend(), [&fds](const int32_t& var) {
			FD_SET(var, &fds);
			});

		if (0 >= select(0, &fds, NULL, NULL, &t_out))
		{
			continue;
		}

		for (auto var : m_lsn_sock)
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