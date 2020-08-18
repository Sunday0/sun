#include "sun_link_mgr.h"
#include "sun_iocp.h"

sun_link_mgr::sun_link_mgr()
{
}

sun_link_mgr::~sun_link_mgr()
{
}

int sun_link_mgr::create_listen_socket(void)
{
	// 创建监听socket
	int sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	if (sock == INVALID_SOCKET)
	{
		std::printf("start_service WSASocket 调用失败");
		return -1;
	}

	return 0;
}

int sun_link_mgr::bind_and_listen(int sock)
{
	// 设置协议类型
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);

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
	return nullptr;
}

// 释放资源对象
void sun_link_mgr::free_link(int link_no)
{

}

// 判断资源对象
bool sun_link_mgr::is_invalid_link(int link_no)
{
	return false;
}

int sun_link_mgr::initialize(void)
{
	std::lock_guard<std::mutex> lck(m_lock);

	m_lock_arr = new std::mutex[MAX_LINKS];
	m_link_tx = new sun_tx[MAX_LINKS];
	m_link_rx = new sun_rx[MAX_LINKS];
	m_link_arr = new sun_socket_st[MAX_LINKS];
	m_link_no_arr = new uint16_t[MAX_LINKS];

	m_link_no_r = m_link_no_arr;
	m_link_no_w = m_link_no_arr;

	for (uint16_t i = 0; i < MAX_LINKS; i++)
	{
		m_link_arr[i].rx_head.lnk_flgs = 1;
		m_link_arr[i].rx_head.rx = m_link_rx[i];

		m_link_arr[i].tx_head.lnk_flgs = 0;
		m_link_arr[i].tx_head.sending = 0;
		m_link_arr[i].tx_head.tx = m_link_tx[i];

		m_link_arr[i].sock = -1;
		m_link_arr[i].session_id = 0;

		m_link_arr[i].key = i;
		m_link_arr[i].seq = 0;
		m_link_arr[i].link_no = m_link_arr[i].key << 16 | m_link_arr[i].seq;

		m_link_arr[i].slt_flgs = 1;

		memset(m_link_arr[i].padding, 0, sizeof(m_link_arr[i].padding));
	}

	return 0;
}

int sun_link_mgr::destroy(void)
{

}