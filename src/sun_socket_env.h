#pragma once#include <mutex>
#include <atomic>

/*
���紦����
*/
class sun_socket_env
{
	
public:
	static int32_t env_start(void);
	static int32_t env_clean(void);
};
