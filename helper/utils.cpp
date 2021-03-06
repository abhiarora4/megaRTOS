#include "utils.h"

#include <stdio.h>

#define MAX_IP_STR_LEN 		20

struct _ui_ipv4_s _temp = {0, 0, 0, 0};
char ip_cstr[MAX_IP_STR_LEN];

void convToCompactIP(struct ipv4_s *ip)
{
	ip->byte[0] = (uint8_t)_temp.ui_octet[0];
	ip->byte[1] = (uint8_t)_temp.ui_octet[1];
	ip->byte[2] = (uint8_t)_temp.ui_octet[2];
	ip->byte[3] = (uint8_t)_temp.ui_octet[3];
}

void cpyToTempIP(struct ipv4_s *ip)
{
	_temp.ui_octet[0] = ip->byte[0];
	_temp.ui_octet[1] = ip->byte[1];
	_temp.ui_octet[2] = ip->byte[2];
	_temp.ui_octet[3] = ip->byte[3];
}

// TODO move this somewhere nice
uint16_t util::crc16(void *buffer, uint16_t len)
{
	// http://www.mycal.net/?cpath=/Archive/&id=567&action=9/
	uint8_t *buff = (uint8_t *)buffer;
	uint16_t crcvalue = 0xFFFF; // Initial crc
	uint16_t data = 0;
	int i;
	for (i=0; i<len; i++) {
		data = (crcvalue ^ buff[i]) & 0xFF;
		data = (data ^ (data << 4)) & 0xFF;
		data = (data << 8) ^ (data << 3) ^ (data >> 4);
		crcvalue=((crcvalue >> 8) ^ data);
	}
	return crcvalue;
}

char* util::ipToCStr(char *dest, struct ipv4_s *ip)
{
	sprintf(dest, "%d.%d.%d.%d", (int)ip->byte[0], (int)ip->byte[1], (int)ip->byte[2], (int)ip->byte[3]);
	return dest;
}

void util::cstrToIP(struct ipv4_s *ip, const char *src)
{
	sscanf(src, "%u.%u.%u.%u", &_temp.ui_octet[0], &_temp.ui_octet[1], &_temp.ui_octet[2], &_temp.ui_octet[3]);
	convToCompactIP(ip);
	return;
}

int util::printIP(struct ipv4_s *ip)
{
	cpyToTempIP(ip);
	return printf("%u.%u.%u.%u", _temp.ui_octet[0], _temp.ui_octet[1], _temp.ui_octet[2], _temp.ui_octet[3]);
}

void util::setIP(struct ipv4_s *ip, uint8_t byte3, uint8_t byte2, uint8_t byte1, uint8_t byte0)
{
	ip->byte[0] = byte3;
	ip->byte[1] = byte2;
	ip->byte[2] = byte1;
	ip->byte[3] = byte0;
}

uint32_t flip_byte_order_ui32(uint32_t ui32)
{
	uint32_t _ui32R = (ui32 >> 24) & 0x000000FF;
	_ui32R |= (ui32 >> 8) & 0x0000FF00;
	_ui32R |= (ui32 << 8) & 0x00FF0000;
	_ui32R |= (ui32 << 24) & 0xFF000000;
	return _ui32R;
}

uint16_t flip_byte_order_ui16(uint16_t ui16)
{
	uint16_t _ui16R = (ui16 >> 8) & 0x00FF;
	_ui16R |= (ui16 << 8) & 0xFF00;
	return _ui16R;
}


int util::inet_aton(const char *cp, struct in_addr_s *inp) 
{
	setIP(inp, cp[0], cp[1], cp[2], cp[3]);
	return 1;
}

char* util::inet_ntoa(struct in_addr_s in) 
{
	ipToCStr(ip_cstr, &in);
	return ip_cstr;
}

in_addr_t util::inet_network(const char *cp)
{
	struct ipv4_s ip;
	cstrToIP(&ip, cp);
	ip.s_addr = ntoh_ui32(ip.s_addr);
	return ip.s_addr;
}

uint16_t hton_ui16(uint16_t ui16_host)
{
	return flip_byte_order_ui16(ui16_host);
}

uint16_t ntoh_ui16(uint16_t ui16_net)
{
	return flip_byte_order_ui16(ui16_net);
}

uint32_t hton_ui32(uint32_t ui32_host)
{
	return flip_byte_order_ui32(ui32_host);
}


uint32_t ntoh_ui32(uint32_t ui32_net)
{
	return flip_byte_order_ui32(ui32_net);
}

int firstSetBit(uint32_t d)
{
	int i;
	for (i=0; !(d & (0x01)); i++, d >>= 1);
	return i;
}

uint32_t maskTime(uint8_t ui8, const uint32_t mask)
{
	int shift = firstSetBit(mask);
	return ((ui8 << shift) & mask);
}


//#define __MAIN__

#ifdef __MAIN__
using namespace util;
int main()
{
	struct ipv4_s ip = {192,168,2,1};

	uint16_t host = 0x1234;
	uint16_t net = ntoh_ui16(host);
	printf(" Con - 0x%02x\n", net);
	printIP(&ip);

	char str[20];
	ipToCStr(str, &ip);	

	struct ipv4_s i;
	cstrToIP(&i, str);

	printIP(&i);

	setIP(&i, 172, 182, 6, 8);
	printIP(&i);

	return 0;
}
#endif