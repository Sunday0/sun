
#include "sun_socket.h"

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include <cstdio>
#include <thread>
#include <array>

using namespace std;


int32_t sun_socket::sock_env_start(void)
{
	int			ret{ 0 };
	WSADATA		data;
	if (0 != WSAStartup(MAKEWORD(2, 2), &data))
	{
		std::printf("ÍøÂç³õÊ¼»¯Ê§°Ü");
		ret = 1;
	}

	return ret;
}

int32_t sun_socket::sock_env_clean(void)
{
	return WSACleanup();
}

static int32_t th_iocp_work(sun_socket * ptr)
{

}


sun_socket::sun_socket()
{
}

sun_socket::~sun_socket()
{
}
