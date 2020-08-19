// sun.cpp : Defines the entry point for the application.
//

#include "sun_main.h"
using namespace std;


sun_server::sun_server()
{
	sun_socket::sock_env_start();
}

sun_server::~sun_server()
{
	sun_socket::sock_env_clean();
}

int32_t sun_server::run()
{
	m_iocp_mgr.start_service();

	auto link = m_link_mgr.alloc_link();
	link->sock = m_link_mgr.create_listen_socket();
	m_iocp_mgr.iocp_bind(link);

	
	while (1)
	{
		// 统计，处理命令等
	}

	return 0;
}




int main()
{
	cout << "Hello CMake." << endl;

	auto sun = new sun_server;
	sun->run();
	delete sun;
	return 0;
}
