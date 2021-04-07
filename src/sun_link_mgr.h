#pragma once

#include <WinSock2.h>
#include <tuple>
#include "sun_conf.h"
#include "sun_link_st.h"
#include "sun_pool.hpp"

#include "sun_singleton.hpp"

constexpr auto MAX_BUFFER = 8192;

struct sun_buff
{
	int32_t len;
	int8_t mem[MAX_BUFFER];
};

using sun_link_tuple = std::tuple<sun_link*, std::mutex*, std::list<sun_buff*>*, sun_men_pool<sun_buff>*>;

class sun_iocp;

class sun_link_mgr : public sun_singleton<sun_link_mgr>
{
private:
	/*
	 * ��Դ��
	 */
	std::mutex						m_lock_mgr;
	
	std::mutex*						m_lock_arr;
	sun_link*						m_link_arr;
	uint16_t*						m_res_arr;

	// ���Ͷ��й����
	// ÿһ�����Ӷ�Ӧһ��pool
	std::list<sun_buff*>*			m_data_list;				// ���ݶ��б�
	sun_men_pool<sun_buff>*			m_link_pool;

	// ��Դ �������� ��Դ��дƫ��
	uint16_t						m_res_idles;
	uint16_t						m_res_r;
	uint16_t						m_res_w;

public:
	sun_link_mgr();
	~sun_link_mgr();

	// ������Դ����
	sun_link* alloc_link();

	// �ͷ���Դ����
	void free_link(uint32_t link_no);

	// ��ر���Դ����
	void close_link(uint32_t link_no);

	// �ж���Դ����
	bool is_invalid_link(uint32_t link_no);
	
	// ��ȡ��Դ�����ַ
	sun_link* get_link_ptr(uint32_t idx);


	sun_men_pool<sun_buff>* get_link_pool(uint32_t idx);

	sun_link_tuple get_link_tuple(uint32_t idx);

public:
	int32_t initialize(void);
	int32_t destroy(void);

	void free_res(uint32_t idx);
	void close_link(uint32_t link_no, uint32_t idx);
	bool is_invalid_link(uint32_t link_no, uint32_t idx);
};
