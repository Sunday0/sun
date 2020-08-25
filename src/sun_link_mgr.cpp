#include "sun_link_mgr.h"
#include "sun_iocp_mgr.h"

sun_link_mgr::sun_link_mgr()
{
	m_lock_arr = nullptr;
	m_link_arr = nullptr;
	m_res_arr = nullptr;

	//发送队列, 后面用chunk 管理 固定长度
	//list<>						

	// 资源 空闲数量 资源读写偏移
	m_res_idles = 0;
	m_res_r = 0;
	m_res_w = 0;
}

sun_link_mgr::~sun_link_mgr()
{
}

// 申请资源对象
sun_socket_st* sun_link_mgr::alloc_link()
{
	sun_socket_st* ptr = nullptr;

	std::lock_guard<std::mutex> lck(m_lock_mgr);
	if (m_res_idles != 0)
	{
		ptr = &m_link_arr[m_res_r];
		ptr->link_no = ((uint32_t)ptr->seq) << 16 | ptr->idx;
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

sun_socket_st* sun_link_mgr::get_link_ptr(int32_t idx)
{
	return m_link_arr + idx;
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

		m_link_arr[i].idx = i;
		m_link_arr[i].seq = 0;
		m_link_arr[i].link_no = ((uint32_t)m_link_arr[i].seq) << 16 | i;

		m_link_arr[i].slt_flgs = 1;


		m_res_arr[i] = i;
	}

	return 0;
}

int32_t sun_link_mgr::destroy(void)
{
	return 0;
}