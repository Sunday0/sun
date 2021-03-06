#pragma once
#include <iostream>

#include "sun_socket_env.h"
#include "sun_link_mgr.h"
#include "sun_iocp.h"
#include "sun_listen.h"

class sun_server
{
public:
	sun_server();
	~sun_server();
	int32_t start(void);
	int32_t stop(void);
};