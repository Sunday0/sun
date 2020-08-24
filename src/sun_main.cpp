// sun.cpp : Defines the entry point for the application.
//

#include "sun_main.h"
#include <chrono>
#include <thread>
using namespace std;

static void sun_sleep(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

sun_server::sun_server()
{
	sun_socket::sock_env_start();
}

sun_server::~sun_server()
{
	stop();
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
		sun_sleep(1000);
		// 统计，处理命令等
	}

	return 0;
}

int32_t sun_server::stop(void)
{
	m_link_mgr.destroy();
	m_iocp_mgr.stop_service();
	return 0;
}


int main()
{
	cout << "Hello CMake SUN ." << endl;

	auto sun = new sun_server;
	sun->run();
	delete sun;

	cout << "Goodbye CMake SUN ." << endl;
	return 0;
}
