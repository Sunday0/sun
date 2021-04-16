#pragma once#include <mutex>
#include <atomic>

/*
ÍøÂç´¦ÀíÀà
*/
class sun_socket_env
{
	
public:
	static int32_t env_start(void);
	static int32_t env_clean(void);
};
