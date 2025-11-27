#ifndef __SCANNER_H_
#define __SCANNER_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

typedef struct {
	char* start;
	size_t length;
} string_t;

string_t ms(const char* buf, int l);
string_t s(const char* buf);
bool string_cmp(string_t s1, string_t s2);

#define STRING_FMT "%.*s"
#define STRING_VAL(s) s.length, s.start

#define TOKEN_FMT "Token { type: %s, symbols: %.*s, line: %d }"
#define TOKEN_VAL(t) tokentype_str(t.type), (int)t.symbols.length, t.symbols.start, t.line

typedef enum {
  // Single-character tokens.
  TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
  // One or two character tokens.
  TOKEN_BANG, TOKEN_BANG_EQUAL,
  TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_GREATER_EQUAL,
  TOKEN_LESS, TOKEN_LESS_EQUAL,
  // Literals.
  TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
  // Keywords.
  TOKEN_AND, TOKEN_ELSE, TOKEN_FALSE, TOKEN_LET,
  TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NULL, TOKEN_OR,
  TOKEN_PRINT, TOKEN_RETURN,
  TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

  TOKEN_ERROR, TOKEN_EOF,

  TOKEN_LENGTH
} TokenType;

typedef struct {
	const char* start;
	const char* current;
	int line;
} Scanner;

typedef struct {
	TokenType type;
	string_t symbols;
	int line;
} Token;

// global scanner singleton
static Scanner scanner;

void init_scanner(const char* source);
char* tokentype_str(TokenType t);
static string_t get_current_syms();
Token scan_token();

#endif // __SCANNER_H_
