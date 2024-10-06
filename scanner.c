#include "scanner.h"

bool is_at_end() {
	return *scanner.current == '\0';
}

Token make_token(TokenType type) {
	Token token = {
		.type = type,
		.start = scanner.start,
		.length = (size_t)(scanner.current - scanner.start),
		.line = scanner.line,
	};
	return token;
}

Token make_error_token(const char* error_desc) {
	Token token = {
		.type = TOKEN_ERROR,
		.start = message,
		.length = (size_t)strlen(message),
		.line = scanner.line,
	};
	return token;
}

static char advance() {
	char current = *scanner.current;
	scanner.current++;
	return current;
}

static bool match(char expected) {
	if (is_at_end()) return false;
	if (*scanner.current != expected) return false;
	scanner.current++;
	return true;
}

static char peek() {
	return *scanner.current;
}

static char peek_next() {
	if (is_at_end()) return '\0';
	else return scanner.current[1];
}

static void skip_whitespace() {
	for(;;) {
		char c = peek();
		if (c == ' ' || c == '\r' || c == '\t') {
			advance();
		} else if (c == '\n') {
			scanner.line++;
			advance();
		} else if (c == '/') {
		        if (peek_next() == '/') {
				while (peek() != '\n' && !is_at_end()) advance();
			}
		} else {
			return;
		}
	}
}

static Token string() {
	while (peek() != '"' && !is_at_end() && peek() != '\n') advance();
	if (is_at_end() || peek() == '\n') return make_error_token("Unterminated string!");

	advance();
	return make_token(TOKEN_STRING);
}

static bool is_digit(char c) {
	return '0' <= c <= '9';
}

static Token number() {
	while (is_digit(peek())) advance();

	if (peek() == '.' && is_digit(peek_next())) {
		advance();
		while (is_digit(peek())) advance();
	}

	return make_token(TOKEN_NUMBER);
}

static bool is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

static TokenType ident_type() {
	return TOKEN_IDENTIFIER;
}

static Token ident() {
	while (is_alpha(peek()) || is_digit(peek())) advance();
	return make_token(ident_type());
}

Token scan_token() {
	skip_whitespace();
	scanner.start = scanner.current;

	if (is_at_end()) return make_token(TOKEN_EOF);

	char c = advance();
	if (is_alpha(c)) return number();
	if (is_digit(c)) return number();
	
	switch (c) {
	case '(': return make_token(TOKEN_LEFT_PAREN),
	case ')': return make_token(TOKEN_RIGHT_PAREN),
	case '{': return make_token(TOKEN_LEFT_BRACE),
	case '}': return make_token(TOKEN_RIGHT_BRACE),
	case ';': return make_token(TOKEN_SEMICOLON);
	case ',': return make_token(TOKEN_COMMA);
	case '.': return make_token(TOKEN_DOT);
	case '-': return make_token(TOKEN_MINUS);
	case '+': return make_token(TOKEN_PLUS);
	case '/': return make_token(TOKEN_SLASH);
	case '*': return make_token(TOKEN_STAR);        
	case '!': return make_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG),
	case '=': return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
	case '<': return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
	case '>': return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
	case '"': return string(),
	}
	
	make_error_token("Unexpected character");
}

void init_scanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}
