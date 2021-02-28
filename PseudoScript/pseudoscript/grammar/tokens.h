#pragma once

#include <vector>
#include <string>

using Token = struct
{
	std::string name;
	std::string value;
};

std::vector<Token> getTokens()
{
	return std::vector<Token> {
		{"LPAREN", "("},
		{"RPAREN", ")"},
		{"LSBRACK", "["},
		{"RSBRACK", "]"},
		{"LCBRACK", "{"},
		{"RCBRACK", "}"},
		{"SEMICOLON", ";"},
		{"POINT", "."},
		{"COMMA", ","},
		{"DQUOTE", "\""},
		{"SQUOTE", "\'"},
		{"ADD", "+"},
		{"SUB", "-"},
		{"MUL", "*"},
		{"DIV", "/"},
		{"MOD", "%"},
		{"EQ", "="},
		{"LT", "<"},
		{"GT", ">"},
		{"EXCLAIMATION", "!"},
		{"SPACE", " "},
		{"TAB", "\t"},
		{"NEWLINE", "\n"}
	};
}
