#pragma once

#include <cstdint>
#include <mutex>
#include <list>
#include <map>

/*
* �ڴ��һ�η����飬�ٰ�T�г�С�飬ͬʱ��ָ�� ����list
* �������ڴ����ʱ��ͨ����ѯҳָ���Ӧ�Ŀ����ڴ��С�Ƿ����m_page_size 
*   ��ȷ���ͷŸ��ڴ�
* ����һ���㷨�����
* ������ʵ���ͷŹ���
*
*/

using std::mutex;
using std::list;

template <class T>
class sun_men_pool
{
	using pool_guard = std::lock_guard<std::mutex>;
	using pool_map = std::map<void*, int32_t>;
private:
	int32_t				m_idle_size;				// ���п�����
	int32_t				m_num_of_page;				// ÿҳ�Ŀ�����
	int32_t				m_max_size;					// �ܵ�ҳ����
	mutex				m_mutex;					// �ڴ���
	list<T*>			m_idle_pool;				// ���п��б�	
	pool_map			m_page_map;					// ҳָ�����
public:
	sun_men_pool()
	{
		pool_guard lck(m_mutex);
		m_num_of_page = 1024;
		new_page();
	}
	~sun_men_pool()
	{
		pool_guard lck(m_mutex);
		for (auto it = m_page_map.begin(); it != m_page_map.end(); it++)
		{
			delete[] it->first;
		}
		m_page_map.clear();
	}

	T* alloc()
	{
		T* value = nullptr;
		pool_guard lck(m_lock_mgr);

		if (0 == m_idle_pool.size())
		{
			new_page();
		}

		value = m_idle_pool.front();
		m_idle_pool.pop_front();

		return value;
	}

	void free(T* value)
	{
		pool_guard lck(m_lock_mgr);
		m_idle_pool.push_back(value);
	}

	
private:
	void new_page()
	{
		auto page = new T[m_num_of_page];
		m_page_map[page] = m_num_of_page;

		for (size_t i = 0; i < m_num_of_page; i++)
		{
			m_idle_pool.push_back(&page[i]);
		}
	}
};