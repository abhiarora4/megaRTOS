#include "string.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


String::String(char *str, int size)
{
	buffer = str;
	length = 0;
	capacity = size;
}

String::String(const String &str)
{
	buffer = str.buffer;
	length = str.length;
	capacity = str.capacity;
}

int String::checkAndResize(size_t s)
{
	if ((s <= capacity) && buffer)
		return 0;
	//Handle Dynamic Memory Allocation for the String!
	return -1;
}

void String::invalidate(void)
{
	buffer[0] = '\0';
	length = 0;
	//capacity = 0;
}

int String::concat(const char *cstr, unsigned int len)
{
	if (!cstr)
		return -1;
	if (!len)
		return -1;
	if (checkAndResize(len+length))
		return -1;
	strcpy(buffer+length, cstr);

	length += len;
	return 0;

}

int String::concat(const String &str)
{
	return concat(str.buffer, str.length);
}

int String::concat(const char *cstr)
{
	return concat(cstr, strlen(cstr));

}

int String::concat(char c)
{
	return concat(&c, 1);
}

void String::trim(void)
{
	if (!buffer)
		return;
	if (!length)
		return;
	char *begin = buffer;
	char *end = buffer + length - 1;

	while (isspace(*begin) && end > begin) begin++;
	while (isspace(*end) && end > begin) end--;

	length = end + 1 - begin;
	if (begin > buffer) memcpy(buffer, begin, length);
	buffer[length] = 0;
}

int String::cmp(String &str)
{
	return strncmp(buffer, str.buffer, (length < str.length) ? length : str.length);
}

char String::operator[](int ndx) const
{
	if (ndx > length)
		return '\0';
	return buffer[ndx];
}

char& String::operator[](int ndx)
{
	static char dummy_char;
	dummy_char = 0;
	if (ndx > length || !buffer)
		return dummy_char;
	return buffer[ndx];
}

String& String::operator=(const char *c)
{
	int i;
	for (i = 0, length=0; i<capacity && *c; i++, length++)
		buffer[i] = c[i];
	return *this;
}

String& String::operator=(const String &str)
{
	int i;
	if (checkAndResize(str.length)) {
		invalidate();
		return *this;
	}
	for (i = 0, length=0; i<str.length; i++)
		buffer[i] = str.buffer[i];

	length = str.length;
	return *this;
}

String& String::operator+=(const String &str)
{
	concat(str);
	return *this;
}

String& String::operator+=(const char *cstr)
{
	concat(cstr);
	return *this;

}

long String :: toInt() const
{
	if (buffer)
		return atol(buffer);
	return 0;
}

float String::toFloat() const
{
	if (buffer)
		return float(atof(buffer));
	return 0;
}

void String::toLowerCase(void)
{
	for (int i=0; i<length; i++) {
		if (isalpha(buffer[i]))
			buffer[i] = tolower(buffer[i]);
	}
}

void String::toUpperCase(void)
{
	for (int i=0; i<length; i++) {
		if (isalpha(buffer[i]))
			buffer[i] = toupper(buffer[i]);
	}
}

void String::replace(char find, char replace)
{
	if (!isValid())
		return;
	for (char *p=buffer; *p; p++)
		if (*p == find)
			*p = replace;

}


bool String::isValid(void)
{
	if (buffer && length > 0)
		return true;
	return false;
}







