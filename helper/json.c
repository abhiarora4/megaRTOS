#include "json.h"
#include "strings.h"
#include "../hal/serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


struct jasonParser_s{
	char *jsStr;
	int jsOffset;

	int numJSPair;
	bool parsed;
};

typedef struct {
	int start;
	int end;

	char type; //'i' => invalid, 's' => string, 'n' => primitive, 'o' => object, 'a' => array
}jasonTok_t;

typedef struct {
	jasonTok_t key;
	jasonTok_t value;
} jasonPair_t;

#define MAX_JSON_PAIR 200

struct jasonParser_s jsParser;
jasonPair_t jsPair[MAX_JSON_PAIR];

int json_init(char *jsonStr)
{
	if (!jsonStr)
		return -1;

	jsParser.parsed = false;
	jsParser.jsStr = jsonStr;

	jsParser.numJSPair = 0;
	jsParser.jsOffset = 0;
	return 0;
}

jasonPair_t *jsPair_alloc()
{
	if (jsParser.numJSPair >= MAX_JSON_PAIR)
		return NULL;

	return &(jsPair[jsParser.numJSPair++]);
}

jasonTok_t *keyTok_alloc(jasonPair_t *pair)
{
	if (!pair)
		return NULL;

	pair->key.start = pair->key.end = -1;
	pair->key.type = 'i';
	return &(pair->key);
}

jasonTok_t *valTok_alloc(jasonPair_t *pair)
{
	if (!pair)
			return NULL;

	pair->value.start = pair->value.end = -1;
	pair->value.type = 'i';
	return &(pair->value);
}

#define isBrackets(c) ((c == '[') || (c == ']') || (c == '{') || (c == '}'))
#define isQuotes(c) ((c == '\'') || (c == '\"'))
#define isTokValidChar(c) (isalnum(c) || isBrackets(c) || isQuotes(c))

int jsonParseArray()
{
	return -1;
}

int jsonParseObject()
{
	jasonPair_t *pair;
	jasonTok_t *tok;

	char c;
	char keyOrValue = 'u'; //'k' => key, 'v' => value, 'u' => unknown token!
//	tokType =>'s' => string, 'p' => primitive, 'a' => array, 'o' => object, 'i' => invalid

	do {
		switch (c = jsParser.jsStr[jsParser.jsOffset]) {
			case '{':
				pair = jsPair_alloc();
				if (!pair)
					goto ERROR;
				tok = keyTok_alloc(pair);
				keyOrValue = 'k';
				tok->type = 'i';
				break;

			case ',': //Should be here after processing 'value' tok
				pair = jsPair_alloc();
				if (!pair)
					goto ERROR;
				if (keyOrValue != 'v')
					goto ERROR;

				tok = keyTok_alloc(pair);
				keyOrValue = 'k';
				tok->type = 'i';
				break;
			case ':': //Should be here after processing 'key' token
				if (keyOrValue != 'k')
					goto ERROR;
				tok = valTok_alloc(pair);
				tok->start = jsParser.jsOffset + 1;
				keyOrValue = 'v';
				tok->type = 'i';
				break;

			case '}':
				if (keyOrValue != 'v')
					goto ERROR;
				goto EXIT;
				break;

			case '\"':
				if (tok->type == 'i') {
					tok->type = 's';
					tok->start = jsParser.jsOffset + 1;
					break;
				}
				if (tok->type != 's')
					goto ERROR;
				tok->end = jsParser.jsOffset - 1;
				tok->type = 'i';
				break;

			case '0': case '1': case '2': case '3': case '4': case '5':
			case '6': case '7': case '8': case '9': case '.':
			{
				static bool decimal = false;
				if (tok->type == 'i') {
					tok->type = 'n';
					decimal = false;
				}
				if (decimal && c == '.')
					goto ERROR;
				if (c == '.')
					decimal = true;
				tok->end = jsParser.jsOffset;
				break;
			}
			case '\n': case '\r': case '\t':
				break;

			default: //Alphabets!!
				if (keyOrValue == 'u')
					goto ERROR;
				if (!tok)
					goto ERROR;
				tok->end = jsParser.jsOffset;
				break;
		}
		jsParser.jsOffset++;
	}  while (1);

	EXIT:
	serialPrintln("exit\n");
	jsParser.parsed = 1;
	return 0;

	ERROR:
	serialPrintln("Error at - %c and pos - %d\n", c, jsParser.jsOffset);
	jsParser.parsed = false;
	jsParser.jsOffset = 0;
	jsParser.numJSPair = 0;
	return -1;
}

int jsonParse()
{
	return jsonParseObject();
}

int getValueAtJSKey(char *key, char *val)
{
	if (!jsParser.parsed)
		return -1;

	char *js = jsParser.jsStr;
	int i;
	for (i = 0; i < jsParser.numJSPair; i++) {
		if (strncmp(key, &js[jsPair[i].key.start], jsPair[i].key.end - jsPair[i].key.start + 1))
			continue;
		strlcpy(val, &js[jsPair[i].value.start], jsPair[i].value.end - jsPair[i].value.start + 1);
		printf("Val type - %c\n", jsPair[i].value.type);
		return 0;
	}
	return 1;
}

#define __MAIN__

#ifdef __MAIN__

int main()
{
	char js[100] = "\t{\"abhi\":\t2.5, \"anu\":\"best\", \"papa\":\"angry\", \"mommy\":\"sweet\"}";
	json_init(js);
	serialPrintln("Jason String - %s\n", js);
	jsonParse();
	int i;
	for (i = 0; i < jsParser.numJSPair; i++) {
		char token[100];
		strlcpy(token, &jsParser.jsStr[jsPair[i].key.start], jsPair[i].key.end - jsPair[i].key.start + 1);
		serialPrintln("Key[%d] - %1.10s \t", i, token);
		strlcpy(token, &jsParser.jsStr[jsPair[i].value.start], jsPair[i].value.end - jsPair[i].value.start + 1);
		serialPrintln("Value[%d] - %1.10s\n", i, token);
	}
	char val[10];
	getValueAtJSKey("abhi", val);
	serialPrintln("val - %s\n", val);

	serialPrintln("Jason String - %s\n", js);

	return 0;
}
#endif
