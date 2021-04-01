
#include "sun_socket.h"
#include <cstdint>

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <cstdio>

int32_t sun_socket::sock_env_start(void)
{
	int			ret{ 0 };
	WSADATA		data;
	if (0 != WSAStartup(MAKEWORD(2, 2), &data))
	{
		std::printf("�����ʼ��ʧ��");
		ret = 1;
	}

	return ret;
}

int32_t sun_socket::sock_env_clean(void)
{
	return WSACleanup();
}
