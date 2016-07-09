#ifndef _UTILS_H_
#define _UTILS_H_

#include "type.h"

#include <ctype.h>
#include <stdbool.h>


#define min(x,y) 					((x)>(y) ? (y) : (x))
#define max(x,y) 					((x)>(y) ? (x) : (y))

#define iseven(x) 					((x) & 0x01)
#define isodd(x)					(!iseven(x))

/* Double Negation Normalize Boolean values i {0,1} */
#define bit_at(a,i)					!!((a)[(unsigned int)(i) >> 3] & (1 << ((unsigned int)(i) & 0x07)))
	
#define array_size(a)				(sizeof(a)/sizeof(a))

#define CR                  '\r'   //Carriage Return Character
#define LF                  '\n'   //Line Feed or New Line Character

#define isblank(c) 			((c == ' ') || (c == '\t') || (c == '\v'))
#define isbacksplash(c) 	(c == '/')
#define ishex(c)           	(isxdigit(c))

#define isCR(c)				(c == CR)
#define isLF(c)				(c == LF)

namespace util {
	uint16_t crc16(void * buff, uint16_t len);

	char * ipToCStr(char *dest, struct ipv4_s *ip);
	void cstrToIP(struct ipv4_s *ip, const char *src);

	int printIP(struct ipv4_s *ip);
	void setIP(struct ipv4_s *ip, uint8_t byte3, uint8_t byte2, uint8_t byte1, uint8_t byte0);

	int inet_aton(const char *cp, struct in_addr_s *inp);
	char *inet_ntoa(struct in_addr_s in);
	in_addr_t inet_network(const char *cp);

	struct in_addr inet_makeaddr(in_addr_t net, in_addr_t host);
	in_addr_t inet_lnaof(struct in_addr in);
	in_addr_t inet_netof(struct in_addr in);

}

uint16_t hton_ui16(uint16_t ui16_host);
uint16_t ntoh_ui16(uint16_t ui16_net);

uint32_t hton_ui32(uint32_t ui32_host);
uint32_t ntoh_ui32(uint32_t ui32_net);

int firstSetBit(uint32_t d);

uint32_t maskTime(uint8_t ui8, const uint32_t mask);

#endif