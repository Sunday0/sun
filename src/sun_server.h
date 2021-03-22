#pragma once
#include <iostream>

#include "sun_socket.h"
#include "sun_link_mgr.h"
#include "sun_iocp_mgr.h"
#include "sun_listen.h"

class sun_server
{
private:
	sun_listen							m_lsn_mgr;
	sun_link_mgr						m_link_mgr;
	sun_iocp_mgr						m_iocp_mgr;
public:
	sun_server();
	~sun_server();
	int32_t start(void);
	int32_t stop(void);
};