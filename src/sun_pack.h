#pragma once
#include <cstdint>
#pragma warning(disable:4200)

#pragma pack(1)

/* Э��˵��
* 	sun_pack    Э��ͷ���ṹ, У�飬ѹ�������� ��ʶ ��ָ msg_buff
*
*   ���ݰ��ֶ�˵��:
*  
* 	magic		Э��ǩ��: $
* 	zip_flag	ѹ����־: 0 δѹ�� 1 ѹ��  LZW8192
* 	cmp_flag    ���ܱ�־: 0 δ���� 1 ����  �������� openssl �ӿ� ���ݲ�ʵ��
* 	err_flag	�����־: 0 ����	  errno ��Ч��frames �� number��Ч
*						  1 ����  �������� errno   ����
* 	paddingf    λ����
* 	crc16		crc16У��
* 	main		��Э���	
* 	child		��Э���
* 	seqno		������� ����
* 	errno		�������
* 	frames		��֡��
* 	number		��ǰ֡���
*   msg_len		��Ϣ����
* 	padding;	����
* 	msg_buff[0] ��Ϣ����
*/

struct sun_pack
{
	uint8_t  magic;
	uint8_t  zip_flag : 1;
	uint8_t  cmp_flag : 1;
	uint8_t  err_flag : 1;
	uint8_t  paddingf : 5;
	uint16_t crc16;
	uint16_t main;
	uint16_t child;
	uint16_t seqno;
	union {
		uint16_t errno;
		struct {
			uint8_t frames;
			uint8_t number;
		};
	};
	uint16_t msg_len;
	uint16_t padding;
	int8_t	 msg_buff[0];
};

#pragma pack()