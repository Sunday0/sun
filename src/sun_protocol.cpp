#include "sun_protocol.h"
#include "sun_pack.h"

uint16_t sun_protocol::sun_crc16(const int8_t* data, int32_t len)
{
	uint16_t	ret		= 0;

	auto p16 = (int16_t*)(data);
	auto len16 = len >> 1;			// ���� int8_t ת���� uint16_t ������
	while (len16 > 0)
	{
		ret += *p16;
		p16++;
		len16--;
	}

	if (len & 1)					// len Ϊ ����
	{
		uint16_t value = data[len-1];
		ret += value;
	}

	return ret;
}


int32_t sun_protocol::analyze(int8_t * buff, uint16_t len)
{
	auto ptr = (sun_pack*)buff;
	if (len < sizeof(sun_pack))
	{
		return 0;
	}

	if ('$' != ptr->magic)
	{
		// Э�����
		return -1;
	}
		
	auto crc = sun_crc16(buff, len);
	if (crc != ptr->crc16)
	{
		// Э�����
		return -1;
	}

	if (len < ptr->lenth)
	{
		// ���ݲ���
		return 0;
	}

	auto ret = (int32_t)(sizeof(sun_pack) + ptr->lenth);

	return ret;
}