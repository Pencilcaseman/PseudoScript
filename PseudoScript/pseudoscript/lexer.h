#pragma once

#include <vector>
#include <string>
#include "grammar/tokens.h"

using UINT = unsigned long long;
using INT = long long;
using STRING = const char *;

#define DEBUG_POINT std::cout << "Debug Point: Line " << __LINE__ << "\n";

using LexerError = struct  
{
	std::string details;
	UINT line;
	UINT charPos;
};

class Lexer
{
public:
	std::vector<std::string> program;
	std::vector<Token> tokenized;
	UINT line;
	UINT index;

	Lexer(const std::vector<std::string> &p) : program(p), line(0), index(0)
	{}

	inline void advance(UINT len = 1)
	{
		index += len;

		if (index >= program[line].length())
		{
			line++;
			index = 0;
		}
	}

	inline LexerError tokenize()
	{
		auto tokens = getTokens();

		// while (index < program[line].length())
		while (line < program.size())
		{
			bool foundToken = false;

			if (program[line][index] == ')')
				int x = 5;

			// Search through token list for the current character
			for (const auto &token : tokens)
			{
				if (program[line][index] == token.value[0])
				{
					// Check for single or double quotes
					if (token.name == "DQUOTE" || token.name == "SQUOTE")
					{
						auto literalLen = checkStringLiteral(program[line], index + 1, token.value);
						tokenized.emplace_back(Token{"STRINGLITERAL", std::string(program[line].begin() + index, program[line].begin() + index + literalLen + 1)});
						foundToken = true;
						advance(literalLen + 1);
						break;
					}

					// Found the token, so add to list
					tokenized.emplace_back(token);
					foundToken = true;
					advance();
					break;
				}
			}

			if (!foundToken)
			{
				// Check for fixed/floating point value
				auto res = checkNumeric(program[line], index, tokens);
				auto numLen = res.first;
				auto points = res.second;

				if (points == 0)
				{
					tokenized.emplace_back(Token{"INT", std::string(program[line].begin() + index, program[line].begin() + index + numLen)});
					foundToken = true;
					advance(numLen);
				}
				else if (points == 1)
				{
					tokenized.emplace_back(Token{"FLOAT", std::string(program[line].begin() + index, program[line].begin() + index + numLen)});
					foundToken = true;
					advance(numLen);
				}
				else if (points != -1)
					return {"Invalid number of decimal points", line, index};
			}

			if (!foundToken)
			{
				// Character was not in the list of tokens, so check for numbers and alphanumeric values
				auto alphaLen = checkAlphaNum(program[line], index, tokens);
				tokenized.emplace_back(Token{"ALPHANUM", std::string(program[line].begin() + index, program[line].begin() + index + alphaLen)});
				foundToken = true;
				advance(alphaLen);
			}

			if (!foundToken)
				return {"Invalid character " + std::string(1, program[line][index]), line, index};
		}

		return {"PASSED", (UINT) -1, (UINT) -1};
	}

	// Check for a string literal
	UINT checkStringLiteral(const std::string &prog, UINT pos, const std::string lookFor)
	{
		UINT tmpIndex = pos;

		while (tmpIndex < prog.length())
		{
			if (prog[tmpIndex++] == lookFor[0])
				return tmpIndex - pos;
		}

		return tmpIndex - pos;
	}

	// Check the program string for a numeric value (fixed/floating point)
	std::pair<UINT, UINT> checkNumeric(const std::string &prog, UINT pos, const std::vector<Token> &tokens)
	{
		UINT tmpIndex = pos;
		UINT points = 0;

		while (tmpIndex < prog.length())
		{
			bool foundPoint = false;
			// Check value is not an existing token
			for (const auto &token : tokens)
			{
				if (prog[tmpIndex] == token.value[0])
				{
					if (token.value != ".")
						return std::make_pair(tmpIndex - pos, points);

					foundPoint = true;
					points++;
					tmpIndex++;
				}
			}

			if (!foundPoint)
			{
				if (prog[tmpIndex] >= '0' && prog[tmpIndex] <= '9')
					tmpIndex++;
				else
					return std::make_pair(tmpIndex - pos, -1);
			}
		}

		return std::make_pair(tmpIndex - pos, points);
	}

	// Check the program string for an alphanumeric value
	UINT checkAlphaNum(const std::string &prog, UINT pos, const std::vector<Token> &tokens)
	{
		UINT tmpIndex = pos;

		while (tmpIndex < prog.length())
		{
			// Check value is not an existing token
			for (const auto &token : tokens)
				if (prog[tmpIndex] == token.value[0])
					return tmpIndex - pos;

			// Check for alphanumeric
			if ((prog[tmpIndex] >= 'A' && prog[tmpIndex] <= 'Z') ||
				(prog[tmpIndex] >= 'a' && prog[tmpIndex] <= 'z') ||
				(prog[tmpIndex] >= '0' && prog[tmpIndex] <= '9'))
			{
				// Is alphanumeric
				tmpIndex++;
			}
			else
			{
				return tmpIndex - pos;
			}
		}
	}
};
