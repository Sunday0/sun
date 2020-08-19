// sun.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include "sun_link_mgr.h"
#include "sun_iocp_mgr.h"
#include "sun_socket.h"

class sun_server 
{
private:
	sun_link_mgr						m_link_mgr;
	sun_iocp_mgr						m_iocp_mgr;

public:
	sun_server();
	~sun_server();
	int32_t run(void);
};
