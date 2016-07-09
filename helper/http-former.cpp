#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char colon[] = ":";


struct http_former_s {
	char *cstr;
	char *curr_ptr;

	uint8_t version;
	uint8_t type;
};

static struct http_former_s former;	

int http_former_init(char *cstr, uint8_t version, uint8_t message)
{
	memset(&former, 0, sizeof(former));
	former.cstr = cstr;
	former.curr_ptr = cstr;
	*cstr = '\0';

	former.version = version;
	former.type = message;
	return 0;
}

int http_print_request_line(char method, char *url)
{
	if (former.type != HTTP_REQUEST)
		return -1;
	char version = former.version/10;
	char revision = former.version - version*10; 
	sprintf(former.cstr, "%s %s HTTP/%d.%d\r\n", http_method[method], url, version, revision);
	
	former.curr_ptr += strlen(former.curr_ptr);

	return 0;
}

int http_print_status_line(int status_code, char *status_mess)
{
	if (former.type != HTTP_RESPONSE)
		return -1;
	char version = former.version/10;
	char revision = former.version - version*10; 
	sprintf(former.cstr, "HTTP/%d.%d %d %s\r\n", version, revision, status_code, status_mess);
	
	former.curr_ptr += strlen(former.curr_ptr);

	return 0;

}

int http_print_header(char header, int value)
{
	sprintf(former.curr_ptr, "%s:%d\r\n", http_header[header].name, value);

	former.curr_ptr += strlen(former.curr_ptr);
	return 0;
}

int http_print_header(char header, char *value)
{
	sprintf(former.curr_ptr, "%s:%s\r\n", http_header[header].name, value);

	former.curr_ptr += strlen(former.curr_ptr);
	return 0;
}

int http_header_end()
{
	sprintf(former.curr_ptr, "\r\n");

	former.curr_ptr += strlen(former.curr_ptr);
	return 0;
}

int http_verify()
{

	return 0;
}

int http_append_message(char *mess)
{
	strcat(former.curr_ptr, mess);
	return 0;
}


//#define _MAIN_

#ifdef _MAIN_
#define HTTP_CSTR_LEN 1024
int main()
{

	char http[HTTP_CSTR_LEN];
	http_former_init(http, HTTP_1_0, HTTP_REQUEST);
	http_print_request_line(GET, "/abhi.html");
	http_print_header(CONTENT_TYPE, "text/html");
	char mess[1024] = "{\"lock\":\"on\",\"health\":\"good\"}";
	int len = strlen(mess);
	http_print_header(CONTENT_LENGTH, len);
	http_header_end();
	http_append_message(mess);
	printf("Http - %s\n", http);

	return 0;
}
#endif