
#include "sun_server.h"
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono;
static void sun_sleep(int mseconds)
{
	this_thread::sleep_for(milliseconds(mseconds));
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

int32_t sun_server::start()
{
	m_link_mgr.initialize();
	m_iocp_mgr.start_service(&m_link_mgr);
	m_lsn_mgr.start_listen(&m_iocp_mgr, &m_link_mgr);

	m_lsn_mgr.do_listen_work();

	return 0;
}

int32_t sun_server::stop(void)
{
	m_lsn_mgr.stop_listen();
	m_link_mgr.destroy();
	m_iocp_mgr.stop_service();
	return 0;
}