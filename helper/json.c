#include "json.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


struct jasonParser_s{
	char *jsStr;
	int jsOffset;

	int numJSPair;
	bool parsed;
};

enum tokenType_e {
	TOKEN_INVALID, TOKEN_OBJECT, TOKEN_ARRAY, TOKEN_STRING, TOKEN_PRIMITIVE
};

typedef struct {
	int start;
	int end;

	enum tokenType_e type;
}jasonTok_t;

typedef struct {
	jasonTok_t key;
	jasonTok_t value;

	struct jasonPair_t *nextJSPair;
} jasonPair_t;

#define MAX_JSON_PAIR 200

struct jasonParser_s jsParser;
jasonPair_t jsPair[MAX_JSON_PAIR];

int jason_init(char *jsonStr)
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
	return &(pair->key);
}

jasonTok_t *valTok_alloc(jasonPair_t *pair)
{
	if (!pair)
			return NULL;

	pair->value.start = pair->value.end = -1;
	return &(pair->value);
}


#define isBrackets(c) ((c == '[') || (c == ']') || (c == '{') || (c == '}'))
#define isQuotes(c) ((c == '\'') || (c == '\"'))
#define isTokValidChar(c) (isalnum(c) || isBrackets(c) || isQuotes(c))

enum tokenType_e getTokenType(jasonTok_t *tok)
{
	char c = jsParser.jsStr[tok->start];

	enum tokenType_e type;
	char strEnclosingQuote;

	if (isdigit(c)) {
		type = TOKEN_PRIMITIVE;
	} else if (isQuotes(c)) {
		strEnclosingQuote = c;
		type = TOKEN_STRING;
		tok->start++;
	} else if (c == '[') {
		type = TOKEN_ARRAY;
		tok->start++;
	} else if (c == '{') {
		type = TOKEN_OBJECT;
		tok->start++;
	} else {
		type = TOKEN_INVALID;
	}

	c = jsParser.jsStr[tok->end];

	if (type == TOKEN_STRING) {
		if (c != strEnclosingQuote)
			type = TOKEN_INVALID;
		else
			tok->end--;
	} else if (type == TOKEN_ARRAY) {
		if (c != ']')
			type = TOKEN_INVALID;
		else
			tok->end--;
	} else if (type == TOKEN_OBJECT) {
		if (c != '}')
			type = TOKEN_INVALID;
		else
			tok->end--;
	} else {
		type = TOKEN_INVALID;
	}

	return type;
}

bool jsTokValidation(jasonPair_t *pair)
{
	char *js = jsParser.jsStr;
	jasonTok_t *key = &(pair->key);
	jasonTok_t *val = &(pair->value);

	for (; !isTokValidChar(js[key->start]); key->start++);
	for (; !isTokValidChar(js[key->end]); key->end--);
	if (key->start >= key->end)
		return false;

	key->type = getTokenType(key);

	if (key->type != TOKEN_STRING)
		return false;

	for (; !isTokValidChar(js[val->start]); val->start++);
	for (; !isTokValidChar(js[val->end]); val->end--);

	if (val->start > val->end)
		return false;

	val->type = getTokenType(val);

	if (val->type == TOKEN_INVALID)
		return false;

	return true;
}

#include "stack.h"

STACK_DEF(jsonContext, int, 8);

enum { C_BRACKETS_O = 0, C_BRACKETS_C,
	S_BRACKETS_O, S_BRACKETS_C, COMMA,
	COLON, QUOTES, ALPHA, SPACE, DIGIT, DOT
};

static int mask = 0x00;

bool isValidChar(char c)
{
	int cType;

	if (c == '{')
		cType = C_BRACKETS_O;
	else if (c == '}')
		cType = C_BRACKETS_C;
	else if (c == '[')
		cType = S_BRACKETS_O;
	else if (c == ']')
		cType = S_BRACKETS_C;
	else if (c == ',')
		cType = COMMA;
	else if (c == ':')
		cType = COLON;
	else if (c == '\"' || c == '\'')
		cType = QUOTES;
	else if (isalpha(c))
		cType = ALPHA;
	else if (isdigit(c))
		cType = DIGIT;
	else if (c == '.')
		cType = DOT;
	else
		cType = SPACE;

	return mask & (1 << cType);
}

void setContext(char c)
{
	stackPush(jsonContext, &mask);
	mask = 0x00;

	mask |= (1 << SPACE);

	if (c == ',')
		mask |= (1 << QUOTES) | (1 << DIGIT);
	else if (c == ':')
		mask |= (1 << QUOTES) | (1 << S_BRACKETS_O) | (1 << C_BRACKETS_O);
	else if (c == '[')
		mask = 0x00;
	else if (c == ']')
		mask |= (1 << ALPHA) | (1 << QUOTES) | (1 << S_BRACKETS_C);
	else if (c == '{')
		mask |= (1 << QUOTES) | (1 << C_BRACKETS_C);
	else if (c == '}')
		mask = 0x00;
	else if (c == '\"' || c == '\'')
		mask |= (1 << ALPHA) | (1 << SPACE) | (1 << DIGIT);
	else if (isdigit(c))
		mask |= (1 << DIGIT) | (1 << DOT);
}

int restorePreviousContext()
{
	return stackPop(jsonContext, &mask);
}

int jsonParseArray()
{

}

int jsonParseObject()
{
	jasonPair_t *pair;
	jasonTok_t *tok;

	char c;
	char keyOrValue = 'u'; //'k' => key, 'v' => value, 'u' => unknown token!
	char tokType = 'i'; // 's' => string, 'p' => primitive, 'a' => array, 'o' => object, 'i' => invalid

	int objDepth = 0;

	do {
		switch (c = jsParser.jsStr[jsParser.jsOffset]) {
			case '{':
				if (!isValidChar(c))
					goto ERROR;
				setContext(c);
				objDepth++;

				if (objDepth > 1)
					jsonParseObject();

				keyOrValue = 'k';
				tokType = 'i';
				pair = jsPair_alloc();
				if (!pair)
					goto ERROR;
				tok = keyTok_alloc(pair);
				break;
			//FALL THOUGH!
			case ',': //Should be here after processing 'value' tok
				if (!isValidChar(c))
					goto ERROR;
				setContext(c);
				jasonPair_t newPair = jsPair_alloc();
				pair->nextJSPair = newPair;
				pair = newPair;
				if (!pair)
					goto ERROR;
				if (keyOrValue != 'v')
					goto ERROR;

				tok = keyTok_alloc(pair);
				keyOrValue = 'k';
				tokType = 'i';
				break;
			case '[':
				if (!isValidChar(c))
					goto ERROR;
				setContext(c);
				if (keyOrValue != 'v')
					goto ERROR;
				tokType = 'a';
				break;

			case ']':
				if (!isValidChar(c))
					goto ERROR;
				restorePreviousContext();
				if (keyOrValue != 'v')
					goto ERROR;
				tokType = 'i';
				break;
			case '}':
				if (!isValidChar(c))
					goto ERROR;
				restorePreviousContext();
				if (keyOrValue != 'v')
					goto ERROR;

				goto EXIT;
				break;

			case ':': //Should be here after processing 'key' token
				if (!isValidChar(c))
					goto ERROR;
				setContext(c);
				if (keyOrValue != 'k')
					goto ERROR;
				tok = valTok_alloc(pair);
				tok->start = jsParser.jsOffset + 1;
				keyOrValue = 'k';
				break;
			case '\"':
				if (!isValidChar(c))
					goto ERROR;
				if (tokType == 'i') {
					setContext(c);
					tokType = 's';
					tok->start = jsParser.jsOffset + 1;
				}
				if (tokType != 's')
					goto ERROR;
				tok->end = jsParser.jsOffset - 1;
				break;

			case '0': case '1': case '2': case '3': case '4': case '5':
			case '6': case '7': case '8': case '9': case '.':
				if (!isValidChar(c))
					goto ERROR;
				if (tokType == 'i') {
					setContext(c);
					tokType = 'n';
				}
				static bool decimal = false;
				if (tokType != 'n')
					decimal = false;
				if (decimal && c == '.')
					goto ERROR;
				if (c == '.')
					decimal = true;
				tok->end = jsParser.jsOffset;
				break;

			case '\n': case '\r': case '\t':
				if (!isValidChar(c))
					goto ERROR;
				setContext(c);
				if (tokType == 'i')
					tok->start = jsParser.jsOffset + 1;
				break;

			default: //Alphabets!!
				if (!isValidChar(c))
					goto ERROR;
				if (keyOrValue == 'i')
					break;
				if (!tok)
					goto ERROR;
				tok->end = jsParser.jsOffset;
				break;
		}
		jsParser.jsOffset++;
	}  while (1);

	EXIT:
	printf("exit\n");
	{
	int i;
	for (i = 0; i < jsParser.numJSPair; i++)
		if (!jsTokValidation(jsPair + i))
			goto ERROR;
	jsParser.parsed = 1;
	return 0;
	}

	ERROR:
	printf("Error");
	jsParser.parsed = 0;
	jsParser.jsOffset = 0;
	jsParser.numJSPair = 0;

	return -1;

}

int getValueAtJSKey(char *key, char *val)
{
	char *js = jsParser.jsStr;
	int i;
	for (i = 0; i < jsParser.numJSPair; i++) {
		if (strncmp(key, &js[jsPair[i].key.start], jsPair[i].key.end - jsPair[i].key.start + 1))
			continue;
		strlcpy(val, &js[jsPair[i].value.start], jsPair[i].value.end - jsPair[i].value.start + 1);
		return 0;
	}
	return 1;
}

#ifdef __MAIN__

int main()
{
	char js[100] = "  \t{\"abhi\":\"luck\", \"anu\":\"best\", \"papa\":\"angry\", \"mommy\":\"sweet\"}";
	jason_init(js);
	printf("Jason String - %s\n", js);
	jsonParseObject(true);
	int i;
	for (i = 0; i < jsParser.numJSPair; i++) {
		char token[100];
		strlcpy(token, &jsParser.jsStr[jsPair[i].key.start], jsPair[i].key.end - jsPair[i].key.start + 1);
		printf("Key[%d] - %1.10s \t", i, token);
		strlcpy(token, &jsParser.jsStr[jsPair[i].value.start], jsPair[i].value.end - jsPair[i].value.start + 1);
		printf("Value[%d] - %1.10s\n", i, token);
	}
	char val[10];
	getValueAtJSKey("abhi", val);
	printf("val - %s\n", val);

	printf("Jason String - %s\n", js);

	return 0;
}
#endif
