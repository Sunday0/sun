#pragma once

#include <cstdint>
#include <mutex>
#include <list>
#include <map>

/*
* 内存池一次分配大块，再按T切成小块，同时将指针 放入list
* 当程序内存过大时，通过查询页指针对应的空闲内存大小是否等于m_page_size 
*   来确定释放该内存
* 还差一个算法，标记
* 后续再实现释放功能
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
	int32_t				m_idle_size;				// 空闲块数量
	int32_t				m_num_of_page;				// 每页的块数量
	int32_t				m_max_size;					// 总的页数量
	mutex				m_mutex;					// 内存锁
	list<T*>			m_idle_pool;				// 空闲块列表	
	pool_map			m_page_map;					// 页指针对象
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