
#include "sun_socket_env.h"
#include <cstdint>

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <atomic>

static std::atomic<bool> init_count{ 0 };

int32_t sun_socket_env::env_start(void)
{
	int			ret{ 0 };
	bool old_value = false;
	if (init_count.compare_exchange_weak(old_value, true)) {
		WSADATA		wsa_data;
		ret = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
	}

	return ret == 0 ? ret : 1;
}

int32_t sun_socket_env::env_clean(void)
{
	int			ret{ 0 };
	bool old_value = true;
	if (init_count.compare_exchange_weak(old_value, false)) {
		ret = ::WSACleanup();
	}

	return ret;
}
