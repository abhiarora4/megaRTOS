#ifndef _HTTP_H_
#define _HTTP_H_

#include <stdint.h>

/* HTTP Version Number */
#define HTTP_1_1				11
#define HTTP_1_0				10
#define HTTP_2_0				20
/* End */

#define HTTP_MESSAGE_MAX_LEN	512
#define HTTP_MAX_CONTENT_LEN	256

#define STATUS_NUM_DIGITS 	3

#define DEFAULT 0X01

#define HTTP_METHOD_MAP(XX)		\
	XX(0, GET, GET)				\
	XX(1, HEAD, HEAD)			\
	XX(2, POST, POST)			\
	XX(3, PUT, PUT)				\
	XX(4, DELETE, DELETE)		\
	XX(5, CONNECT, CONNECT)		\
	XX(6, TRACE, TRACE)


#define MAX_HEADER_NUM 		10
#define MAX_METHODS 		HTTP_METHODS_SENTIMEL

#define isURLchr(c)			(((c >= '&') && (c <= ';')) || (c == '=') || ((c >= '?') && (c <= '[')) || \
 							(c == ']') || (c == '_') || islower(c) || (c == '~')  || \
 							(c == '!') || (c == '#') || (c == '$'))

#define IS_HEADER_CHAR(ch)                                                     \
  (ch == CR || ch == LF || ch == 9 || ((unsigned char)ch > 31 && ch != 127))


enum http_status_codes_e {
	
	CONTINUE=100,
	SWITCHING_PROTOCOL,

	INFORMATIONAL_SENTINEL,

	OK=200,
	CREATED,
	ACCEPTED,
	NON_AUTHORITATIVE_INFO,
	NO_CONTENT,
	RESET_COUNTER,
	PARTIAL_CONTENT,

	SUCCESSFUL_SENTINEL,

	MULTIPLE_CHOICES=300,
	MOVED_PERMANENTLY,
	FOUND,
	SEE_OTHER,
	NOT_MODIFIED,
	USE_PROXY,
	UNUSED,
	TEMPORARY_REDIRECT,

	REDIRECTION_SENTINEL,

	BAD_REQUEST=400,
	UNAUTHORIZED,
	PAYMENT_REQUIRED,
	FORBIDDEN,
	NOT_FOUND,
	METHOD_NOT_ALLOWED,
	NOT_ACCEPTABLE,
	PROXY_AUTHENTICATION_REQUIRED,
	REQUEST_TIMEOUT,
	CONFLICT,
	GONE,
	LENGTH_REQUIRED,
	PRECONDITION_FAILED,
	ENTITY_TOO_LARGE,
	URL_TOO_LONG,
	USUPPORTED_MEDIA,
	RANGE_NOT_SATISFIABLE,
	EXPECTATION_FAILED,

	CLIENT_SENTINEL,

	SERVER_ERROR=500,
	NOT_IMPLEMENTED,
	BAD_GATEWAY,
	SERVICE_UNAVAILABLE,
	GATEWAY_TIMEOUT,
	VERSION_NOT_SUPPORTED,

	SERVER_SENTINEL
};


struct http_header_s {
	const char *const name;

	uint16_t start_ndx;
	uint16_t end_ndx;
	int (*func)(void);
};

extern struct http_header_s http_header[];
extern const char *const http_method[];

enum http_methods_e {
#define XX(num,name,cstr)	name,
	HTTP_METHOD_MAP(XX)
	HTTP_METHODS_SENTINEL
#undef XX
};

enum http_headers_e {
	USER_AGENT=0,
	HOST,

	CONTENT_TYPE,
	CONTENT_LENGTH,
	CONNECTION,
	DATE,
	TRANSFER_ENCODING,
	AUTHORIZATION,
	LOCATION,
	RETRY_AFTER,
	HEADER_SENTINEL
};

enum http_message_e {
	HTTP_INVALID=0, HTTP_REQUEST, HTTP_RESPONSE
};

enum http_conn_e {
	CLOSE, KEEP_ALIVE, UPGRADE
};

int http_former_init(char *cstr, uint8_t version, uint8_t message);

int http_print_request_line(char method, char *url);
int http_print_status_line(int status_code, char *status_mess);

int http_print_header(char header, int value);

int http_print_header(char header, char *value);
int http_header_end();
int http_verify();
int http_append_message(char *mess);


#endif