#include "scanner.h"

#define KEYWORD_COUNT 14

string_t ms(const char* buf, int l) {
	return (string_t){
		.start = buf,
		.length = l
	};
}

string_t s(const char* buf) {
	return ms(buf, strlen(buf));
}

bool string_cmp(string_t s1, string_t s2) {
	if (s1.length != s2.length) return false;
	for (size_t i = 0; i < s1.length; ++i) {
		if (s1.start[i] - s2.start[i] != 0) return false;
	}
	return true;
}

bool is_at_end() {
	return *scanner.current == '\0';
}

Token make_token(TokenType type) {
	Token token = {
		.type = type,
		.symbols = get_current_syms(),
		.line = scanner.line,
	};
	return token;
}

Token make_error_token(const char* error_desc) {
	Token token = {
		.type = TOKEN_ERROR,
		.symbols = s(error_desc),
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

static Token number() {
	while (isdigit(peek())) advance();

	if (peek() == '.' && isdigit(peek_next())) {
		advance();
		while (isdigit(peek())) advance();
	}

	return make_token(TOKEN_NUMBER);
}

static char* reserved_keyword[KEYWORD_COUNT] = {
	"and",
	"else",
	"false",
	"let",
	"for",
	"fun",
	"if",
	"null",
	"or",
	"print",
	"return",
	"true",
	"var",
	"while"
};

static string_t get_current_syms() {
	return ms(scanner.start, scanner.current - scanner.start);
} 

static int is_reserved() {
	string_t current = get_current_syms();
	for (size_t i = 0; i < KEYWORD_COUNT; i++) {
		if (string_cmp(current, s(reserved_keyword[i]))) {
			return i + 22; 
		}
	}
	return -1;
}

static TokenType ident_type() {
	return TOKEN_IDENTIFIER;
}

static Token ident_or_keyword() {
	while (isalpha(peek()) || isdigit(peek())) advance();
	int type = is_reserved();
	if (type != -1) {
		return make_token(type);
	}
	
	return make_token(ident_type());
}

Token scan_token() {
	skip_whitespace();
	scanner.start = scanner.current;

	if (is_at_end()) return make_token(TOKEN_EOF);

	char c = advance();
	switch (c) {
	case '(': return make_token(TOKEN_LEFT_PAREN);
	case ')': return make_token(TOKEN_RIGHT_PAREN);
	case '{': return make_token(TOKEN_LEFT_BRACE);
	case '}': return make_token(TOKEN_RIGHT_BRACE);
	case ';': return make_token(TOKEN_SEMICOLON);
	case ',': return make_token(TOKEN_COMMA);
	case '.': return make_token(TOKEN_DOT);
	case '-': return make_token(TOKEN_MINUS);
	case '+': return make_token(TOKEN_PLUS);
	case '/': return make_token(TOKEN_SLASH);
	case '*': return make_token(TOKEN_STAR);        
	case '!': return make_token(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
	case '=': return make_token(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
	case '<': return make_token(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
	case '>': return make_token(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
	case '"': return string();
	}
	
	if (isdigit(c)) return number();
	if (isalpha(c)) return ident_or_keyword();
	
	make_error_token("Unexpected character");
}

void init_scanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

char* tokentype_str(TokenType t) {
    assert(TOKEN_LENGTH == 38 && "Update this function: new token type added!");

    switch (t) {
        case TOKEN_LEFT_PAREN:     return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN:    return "TOKEN_RIGHT_PAREN";
        case TOKEN_LEFT_BRACE:     return "TOKEN_LEFT_BRACE";
        case TOKEN_RIGHT_BRACE:    return "TOKEN_RIGHT_BRACE";
        case TOKEN_COMMA:          return "TOKEN_COMMA";
        case TOKEN_DOT:            return "TOKEN_DOT";
        case TOKEN_MINUS:          return "TOKEN_MINUS";
        case TOKEN_PLUS:           return "TOKEN_PLUS";
        case TOKEN_SEMICOLON:      return "TOKEN_SEMICOLON";
        case TOKEN_SLASH:          return "TOKEN_SLASH";
        case TOKEN_STAR:           return "TOKEN_STAR";

        case TOKEN_BANG:           return "TOKEN_BANG";
        case TOKEN_BANG_EQUAL:     return "TOKEN_BANG_EQUAL";
        case TOKEN_EQUAL:          return "TOKEN_EQUAL";
        case TOKEN_EQUAL_EQUAL:    return "TOKEN_EQUAL_EQUAL";
        case TOKEN_GREATER:        return "TOKEN_GREATER";
        case TOKEN_GREATER_EQUAL:  return "TOKEN_GREATER_EQUAL";
        case TOKEN_LESS:           return "TOKEN_LESS";
        case TOKEN_LESS_EQUAL:     return "TOKEN_LESS_EQUAL";

        case TOKEN_IDENTIFIER:     return "TOKEN_IDENTIFIER";
        case TOKEN_STRING:         return "TOKEN_STRING";
        case TOKEN_NUMBER:         return "TOKEN_NUMBER";

        case TOKEN_AND:            return "TOKEN_AND";
        case TOKEN_ELSE:           return "TOKEN_ELSE";
        case TOKEN_FALSE:          return "TOKEN_FALSE";
        case TOKEN_LET:            return "TOKEN_LET";
        case TOKEN_FOR:            return "TOKEN_FOR";
        case TOKEN_FUN:            return "TOKEN_FUN";
        case TOKEN_IF:             return "TOKEN_IF";
        case TOKEN_NULL:           return "TOKEN_NULL";
        case TOKEN_OR:             return "TOKEN_OR";
        case TOKEN_PRINT:          return "TOKEN_PRINT";
        case TOKEN_RETURN:         return "TOKEN_RETURN";
        case TOKEN_TRUE:           return "TOKEN_TRUE";
        case TOKEN_VAR:            return "TOKEN_VAR";
        case TOKEN_WHILE:          return "TOKEN_WHILE";

        case TOKEN_ERROR:          return "TOKEN_ERROR";
        case TOKEN_EOF:            return "TOKEN_EOF";
    }
}
