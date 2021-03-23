
#include "sun_server.h"
#include "sun_listen.h"

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
	sun_listen::get_instance()->start_listen();
	return 0;
}

int32_t sun_server::stop(void)
{
	sun_listen::get_instance()->stop_listen();
	return 0;
}