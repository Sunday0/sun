#pragma once

#include <cstdint>

#include "sun_define.h"

/*
���紦����
*/
class sun_socket
{
public:
	static int32_t sock_env_start(void);
	static int32_t sock_env_clean(void);
};
