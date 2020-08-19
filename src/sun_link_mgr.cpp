#include "sun_link_mgr.h"
#include "sun_iocp.h"

sun_link_mgr::sun_link_mgr()
{
	initialize();
}

sun_link_mgr::~sun_link_mgr()
{
}

int32_t sun_link_mgr::create_listen_socket(void)
{
	// 创建监听socket
	int32_t sock = (int32_t)WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	if (sock == INVALID_SOCKET)
	{
		std::printf("start_service WSASocket 调用失败");
		return -1;
	}

	return 0;
}

int32_t sun_link_mgr::bind_and_listen(int32_t sock)
{
	// 设置协议类型
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

	// 绑定监听端口
	if (SOCKET_ERROR == bind(sock, (const sockaddr*)(&addr), sizeof(addr)))
	{
		std::printf("start_service bind 调用失败");
		return -1;
	}

	// 启用监听功能
	if (SOCKET_ERROR == listen(sock, 100))
	{
		std::printf("start_service listen 调用失败");
		return -1;
	}

	return 0;
}

// 申请资源对象
sun_socket_st* sun_link_mgr::alloc_link()
{
	sun_socket_st* ptr = nullptr;

	std::lock_guard<std::mutex> lck(m_lock_mgr);
	if (m_res_idles != 0)
	{
		ptr = &m_link_arr[m_res_r];
		
		m_res_idles--;
		m_res_r++;
		if (m_res_r == MAX_LINKS)
		{
			m_res_r = 0;
		}
	}

	return ptr ;
}

// 释放资源对象
void sun_link_mgr::free_link(uint32_t link_no)
{
	uint16_t idx = link_no & 0x0000FFFF;
	if (idx >= MAX_LINKS)
	{
		return ;
	}

	{
		std::lock_guard<std::mutex> lck(m_lock_mgr);
		
		if (m_res_idles < MAX_LINKS)
		{
			m_res_arr[m_res_w] = idx;

			m_res_idles++;
			m_res_w++;
			if (m_res_w == MAX_LINKS)
			{
				m_res_w = 0;
			}
		}
	}
}

// 判断资源对象
bool sun_link_mgr::is_invalid_link(uint32_t link_no)
{
	uint16_t idx = link_no & 0x0000FFFF;
	if (idx >= MAX_LINKS)
	{
		return false;
	}
	else
	{
		auto ptr = &m_link_arr[idx];
		std::lock_guard<std::mutex> lck(m_lock_arr[idx]);
		if (ptr->link_no != link_no)
		{
			return false;
		}

		if (ptr->slt_flgs == 1)
		{
			return false;
		}
	}

	return true;
}

int32_t sun_link_mgr::initialize(void)
{
	std::lock_guard<std::mutex> lck(m_lock_mgr);

	m_lock_arr = new std::mutex[MAX_LINKS];
	m_link_arr = new sun_socket_st[MAX_LINKS];
	m_res_arr = new uint16_t[MAX_LINKS];

	m_res_r = 0;
	m_res_r = 0;

	m_res_idles = MAX_LINKS;

	memset(m_link_arr, 0, MAX_LINKS * sizeof(m_link_arr[0]));

	for (uint16_t i = 0; i < MAX_LINKS; i++)
	{
		m_link_arr[i].rx_head.lnk_flgs = 1;

		m_link_arr[i].tx_head.lnk_flgs = 0;
		m_link_arr[i].tx_head.sending = 0;

		m_link_arr[i].sock = -1;
		m_link_arr[i].session_id = 0;

		m_link_arr[i].key = i;
		m_link_arr[i].seq = 0;
		m_link_arr[i].link_no = ((uint32_t)i) << 16 | m_link_arr[i].seq;

		m_link_arr[i].slt_flgs = 1;


		m_res_arr[i] = i;
	}

	return 0;
}

int32_t sun_link_mgr::destroy(void)
{

}