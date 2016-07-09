#include "http.h"
#include "utils.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define isHeaderDef(i)		(http_header[i].start_ndx && http_header[i].end_ndx)

unsigned _loop_var=0;
#define SAFE_WHILE(x)		for (_loop_var=(parser.curr_ptr - parser.cstr); (_loop_var < parser.len) && (x); _loop_var++)
#define IS_EXP_RAISED()		(_loop_var >= parser.len)	

/* Strings for commonly used words */
const char HTTP[] = "HTTP";
const char http[] = "http";
const char keep_alive[] = "Keep-Alive";
const char Close[] = "Close";
const char absURLHttp[] = "http://";
const char absURLHttps[] = "https://";
const char chuncked[] = "chunked";
const char upgrade[] = "upgrade";

int on_content_length();
int on_connection();
int on_date();
int on_transfer_encoding();


struct http_header_s http_header[HEADER_SENTINEL] = {
	[USER_AGENT] 		= 	{	"User-Agent", 			 0, 	0	,	NULL},
	[HOST]		 		= 	{	"Host", 				 0, 	0	,	NULL},
	[CONTENT_TYPE] 		= 	{	"Content-Type", 		 0, 	0	, 	NULL},
	[CONTENT_LENGTH] 	= 	{	"Content-Length", 		 0, 	0 	,	on_content_length},
	[CONNECTION] 		= 	{	"Connection", 			 0, 	0	,	on_connection},
	[DATE] 				= 	{	"Date", 				 0, 	0	,	on_date},
	[TRANSFER_ENCODING] =   {	"Transfer-Encoding",	 0,		0	,	on_transfer_encoding},
	[AUTHORIZATION]		=	{	"Authorization",		 0,		0	,	NULL},
	[LOCATION]			=	{	"Location",				 0, 	0   ,	NULL},
	[RETRY_AFTER]		=	{	"Retry-After",			 0,		0	,	NULL},
};


const char *const http_method[HTTP_METHODS_SENTINEL] = {
#define XX(num,name,cstr) [num] = #cstr,	\
	HTTP_METHOS_MAP(XX)
#undef XX
};

struct http_parser_s {
	char *cstr;
	int len;

	char *curr_ptr;

	uint8_t state;
	uint8_t settings;

	bool isParsed;
	bool isError;
};

struct http_message_s {
	uint8_t type;

	/* Initial Line (Request or Response) */
	char version;
	union {
		uint16_t status_code; 	//For Response
		uint8_t method; 		//For Request
	};
	union {
		uint16_t indexes;
		struct { //For Request
			uint8_t url_start;
			uint8_t url_end;
		};
		struct { //For Response
			uint8_t status_start;
			uint8_t status_end;
		};
	};
	/* Initial Line End */

	/* Headers Values Converted */
	uint8_t content_length;
	uint8_t connection;
	uint32_t date;
	/* End */

	/* Entity Body */
	void *data;
	uint8_t len;
	/* End */
};

static struct http_parser_s parser;
static struct http_message_s message;

#define DEFAULT
enum http_state_e {
	HTTP_INIT,

	HTTP_INITIAL_LINE,
	HTTP_HEADER,
	HTTP_BODY,

	HTTP_PARSED
};


int on_content_length()
{	
	message.content_length = 0x00;
	int i;
	for (i=http_header[CONTENT_LENGTH].start_ndx; (i <= http_header[CONTENT_LENGTH].end_ndx) && isdigit(parser.cstr[i]); i++)
		message.content_length = message.content_length*10 + (parser.cstr[i] - '0');
	if (i < http_header[CONTENT_LENGTH].end_ndx)
		return -1;
	return 0;
}

int on_connection()
{
	if (!strncmp(parser.cstr + http_header[CONNECTION].start_ndx, keep_alive, strlen(keep_alive)))
		message.connection = KEEP_ALIVE;
	else if (!strncmp(parser.cstr + http_header[CONNECTION].start_ndx, upgrade, strlen(upgrade)))
		message.connection = UPGRADE;
	else
		message.connection = CLOSE;
	return 0;
}

int on_date()
{
	return 0;
}

int on_transfer_encoding()
{
	return 0;
}

int http_parser_init(char *http_cstr, int len)
{
	memset(&parser, 0, sizeof(parser));
	memset(&message, 0, sizeof(message));
	parser.cstr = http_cstr;
	parser.curr_ptr = http_cstr;
	parser.len = (len < 0) ? HTTP_MESSAGE_MAX_LEN : len;

	parser.state = HTTP_INIT;
	//parser.settings = DEFAULT;

	int i;
	for (i=0; i<HEADER_SENTINEL; i++)
		http_header[i].start_ndx = http_header[i].end_ndx = 0;
	return 0;
}

int parse_response();
int parse_request();
int parse_body();
int parse_headers();


int http_parse(void)
{
	SAFE_WHILE (isblank(*parser.cstr)) parser.cstr++;
	if (IS_EXP_RAISED())
		return -1;
	parser.curr_ptr = parser.cstr;
	if (!strncmp(parser.cstr, HTTP, strlen(HTTP)))
		return parse_response();
	return parse_request();
}

int checkURL(int start, int end)
{
	bool absOrRel=false;
	bool default_port=true;

	char *ptr = &parser.cstr[start];

	if (strncmp(absURLHttp, ptr, strlen(absURLHttp)))
		goto RELATIVE;
	ptr += strlen(absURLHttp);
	absOrRel = true;

	ptr = strchr(ptr, '/');
	RELATIVE:
	if (*ptr != '/')
		return -1;

	if ((ptr = strchr(ptr, ':')))
		default_port = true;
	return 0;
}

int parse_url()
{
	SAFE_WHILE (isblank(*parser.curr_ptr)) parser.curr_ptr++;
	if (IS_EXP_RAISED())
		return -1;

	message.url_start = parser.curr_ptr - parser.cstr;
	SAFE_WHILE (isURLchr(*parser.curr_ptr)) parser.curr_ptr++;
	if (IS_EXP_RAISED())
		return -1;
	message.status_end = (parser.curr_ptr - parser.cstr) - 1;

	if (!isspace(*parser.curr_ptr))
		return -1;
	return 0;	
}

int parse_http_version()
{
	SAFE_WHILE (isblank(*parser.curr_ptr)) parser.curr_ptr++;

	if (IS_EXP_RAISED())
		return -1;

	if (strncmp(HTTP, parser.curr_ptr, strlen(HTTP)))
		return -1;
	parser.curr_ptr += strlen(HTTP);

	if (!isbacksplash(*parser.curr_ptr++))
		return -1;

	char major = *parser.curr_ptr++ - '0';
	if (*parser.curr_ptr++ != '.')
		return -1;
	char minor = *parser.curr_ptr++ - '0';

	message.version = (char)(major*10 + minor);
	return 0;
}

int parse_method()
{
	SAFE_WHILE (isblank(*parser.curr_ptr)) parser.curr_ptr++;
	if (IS_EXP_RAISED())
		return -1;

	int i;
	for (i=0; i<HTTP_METHODS_SENTINEL; i++) 
		if (strncmp(http_method[i], parser.curr_ptr, strlen(http_method[i])))
			break;
	if (i >= HTTP_METHODS_SENTINEL)
		return -1;
	parser.curr_ptr += strlen(http_method[i]);
	if (!isblank(*parser.curr_ptr))
		return -1;
	message.method = i & (0b1111);
	return 0;
}

int parse_response()
{
	if (parse_http_version())
		goto ERROR;
	SAFE_WHILE (parser.curr_ptr) parser.curr_ptr++;
	if (IS_EXP_RAISED())
		goto ERROR;

	int i;
	for (i = 0, message.status_code = 0; (i < STATUS_NUM_DIGITS) && isdigit(*parser.curr_ptr); i++)
		message.status_code = (message.status_code * 10) + (*parser.curr_ptr++ - '0');

	if (i != STATUS_NUM_DIGITS)
		goto ERROR;

	if (IS_EXP_RAISED())
		goto ERROR;

	SAFE_WHILE (isblank(*parser.curr_ptr)) parser.curr_ptr++;
	if (IS_EXP_RAISED())
		goto ERROR;
	message.status_start = parser.curr_ptr - parser.cstr;
	SAFE_WHILE (!isCR(*parser.curr_ptr)) parser.curr_ptr++;
	if (IS_EXP_RAISED())
		goto ERROR;
	message.status_end = (parser.curr_ptr - parser.cstr) - 1;
	while (isspace(parser.cstr[message.status_end])) message.status_end--;

	if (isLF(*++parser.curr_ptr))
		parser.curr_ptr++;
	if (parse_headers())
		goto ERROR;
	if (parse_body())
		goto ERROR;
	message.type = HTTP_RESPONSE;
	return 0;

ERROR:
	parser.isError = true;
	return -1;	
}

int parse_request()
{
	SAFE_WHILE (isblank(*parser.curr_ptr)) parser.curr_ptr++;
	if (IS_EXP_RAISED())
		goto ERROR;

	int i;
	for (i=0; i<HTTP_METHODS_SENTINEL; i++)
		if (!strncmp(http_method[i], parser.curr_ptr, strlen(http_method[i])))
			break;
	if (i >= HTTP_METHODS_SENTINEL)
		goto ERROR;

	parser.curr_ptr += strlen(http_method[i]);
	if (parse_url())
		goto ERROR;
	if (parse_http_version())
		goto ERROR;
	
	parser.curr_ptr = strchr(parser.curr_ptr, CR) + 1;
	if (!isLF(*parser.curr_ptr++))
		goto ERROR;
	if (parse_headers())
		goto ERROR;
	if (parse_body())
		goto ERROR;
	return 0;

	ERROR:
	return -1;

}

int parse_headers(void)
{
	int i, j;
	bool header_find = false;

	for (i=0; i<MAX_HEADER_NUM; i++) { 
		if (isCR(*parser.curr_ptr)) { // End of Header Fields
			if (isLF(*(++parser.curr_ptr))) 
				parser.curr_ptr++;
			break;	
		}
		while (isblank(*parser.curr_ptr)) parser.curr_ptr++;
		for (j=0, header_find=false; j<HEADER_SENTINEL; j++)
			if (!strncmp(parser.curr_ptr, http_header[j].name, strlen(http_header[j].name)))
				break;

		if (j < HEADER_SENTINEL)
			header_find = true;
		parser.curr_ptr = strchr(parser.curr_ptr, ':');
		parser.curr_ptr++;

		while (isblank(*parser.curr_ptr)) parser.curr_ptr++;
		if (header_find)
			http_header[j].start_ndx = parser.curr_ptr - parser.cstr;

		while (!isCR(*parser.curr_ptr)) parser.curr_ptr++;
		if (header_find)
			http_header[j].end_ndx = parser.curr_ptr - parser.cstr - 1;

		if (isLF(*++parser.curr_ptr)) 
			parser.curr_ptr++;

		if (header_find && http_header[j].func)
			http_header[j].func();
	}
	if (i >= MAX_HEADER_NUM)
		return -1;
	return 0;
}

void print_headers()
{
	int i;
	for (i=0; i<HEADER_SENTINEL; i++)
		if (isHeaderDef(i)) printf("%s - %.*s\n", http_header[i].name, http_header[i].end_ndx - http_header[i].start_ndx + 1, &parser.cstr[http_header[i].start_ndx]);
}

#define MAX_MESSAGE_BODY_LEN 120
int parse_body(void)
{
	bool hasbody = isHeaderDef(CONTENT_LENGTH) && (message.method != HEAD);
	if (!hasbody)
		return 0;
	if (message.content_length >= HTTP_MAX_CONTENT_LEN)
		return -1;
	message.data = parser.curr_ptr;
	message.len = message.content_length;
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>


#define _MAIN_
#ifdef _MAIN_
int main(int argc, char *argv[])
{
	int sock_fd = 0, n = 0;
	char recvBuff[1024];
	struct sockaddr_in serv_addr;

	if (argc <= 2) {
		printf("Usage: <IP-Address> <Port-Number>\n");
		return -1;
	}

	memset(recvBuff, 0, sizeof(recvBuff));

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
			printf("Error: Socket cannot be openned\n");
			return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	int port_number;
	sscanf(argv[2], "%d", &port_number);
	serv_addr.sin_port = htons(port_number);

	if (inet_pton(AF_INET, 	argv[1], &serv_addr.sin_addr) <= 0) {
		printf("Error: Wrong IP Address\n");
		return -1;
	}

	if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
			printf(	"Error: Connection to Server (%s) Failed\n", argv[1]);
			return -1;
	}

	http_former_init(recvBuff, HTTP_1_1, HTTP_REQUEST);
	http_print_request_line(GET, "https://httpbin.org/ip");
	http_print_header(USER_AGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.63 Safari/537.36");
	http_header_end();
	http_parser_init(recvBuff, -1);
	if (http_parse())
		printf("Parsing Failed\n");
	print_headers();

	printf(" %s \n", recvBuff);
	write(sock_fd, recvBuff, strlen(recvBuff));
	
	while ((n = read(sock_fd, recvBuff, sizeof(recvBuff))) > 0) {
		recvBuff[n] = 0;
		http_parser_init(recvBuff, -1);
		if (http_parse())
			printf("Parsing Failed\n");
		print_headers();
		if (fputs(recvBuff, stdout) == EOF)
			printf("Error: Read Failed\n");
	}
	if (n < 0) 
		printf("Error: Read Failed\n");

	return 0;
}
#endif
