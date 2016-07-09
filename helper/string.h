/*
 * string.h
 *
 *  Created on: 31-Mar-2016
 *      Author: abhiarora
 */

#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

#define STRING_DEF(n,s) char __str_##n[s]; \
	String n = String(__str_##n, s);

class String {
private:
	char *buffer; //Buffer to hold strings of characters
	unsigned int length; //Actual number of characters in the string excluding '\0'
	unsigned int capacity; //The maximum current allocated size for storing string

	String(const String &str); //Don't allow copy constructor!
							   //To pass arguments or return String from function, use references only!
	int checkAndResize(size_t n);

protected:
	int concat(const char *cstr, unsigned int len);
	void invalidate(void);
	bool isValid();

public:
	String(char *str, int size);
	String(char c) = delete;

	int len() { return length; }
	int max_size() { return capacity; }
	int cmp(String &str);

	char& first() { return *buffer; }
	char& last() { return *(buffer + length); }

	//Precaution Functions!

	//Helper Functions!!
	int concat(const String &str);
	int concat(const char *cstr);
	int concat(char c);



	//Operators!
	char operator[](int ndx) const;
	char& operator[](int ndx);

	String & operator=(const char *c);
	String & operator=(const String &str);
	String& operator+=(const String &str);
	String& operator+=(const char *cstr);

	bool operator == (String &str);
	bool operator != (String &str);

	//modification
	void replace(char find, char replace);
	void trim(void);
	void toUpperCase(void);
	void toLowerCase(void);


	//conversion
	long toInt() const;
	float toFloat() const;






	~String();


};


#endif /* HELPER_STRING_H_ */
