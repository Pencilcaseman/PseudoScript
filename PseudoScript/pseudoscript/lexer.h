#pragma once

#include <vector>
#include <string>
#include "grammar/tokens.h"

using UINT = unsigned long long;
using INT = long long;
using STRING = const char *;

class Lexer
{
public:
	std::string program;
	std::vector<Token> tokenized;
	UINT index;

	Lexer(const std::string &p) : program(p), index(0)
	{}

	inline void advance(UINT len = 1)
	{
		index += len;
	}

	inline std::pair<std::string, UINT> tokenize()
	{
		auto tokens = getTokens();

		while (index < program.length())
		{
			bool foundToken = false;

			// Search through token list for the current character
			for (const auto &token : tokens)
			{
				if (program[index] == token.value[0])
				{
					// Check for single or double quotes
					if (token.name == "DQUOTE" || token.name == "SQUOTE")
					{
						auto literalLen = checkStringLiteral(program, index + 1, token.value);
						tokenized.emplace_back(Token{"STRINGLITERAL", std::string(program.begin() + index, program.begin() + index + literalLen + 1)});
						foundToken = true;
						advance(literalLen + 1);
					}
					else
					{
						// Found the token, so add to list
						tokenized.emplace_back(token);
						foundToken = true;
						advance();
					}
				}
			}

			if (!foundToken)
			{
				// Check for fixed/floating point value
				auto res = checkNumeric(program, index, tokens);
				auto numLen = res.first;
				auto points = res.second;

				if (points == 0)
				{
					tokenized.emplace_back(Token{"INT", std::string(program.begin() + index, program.begin() + index + numLen)});
					foundToken = true;
					advance(numLen);
				}
				else if (points == 1)
				{
					tokenized.emplace_back(Token{"FLOAT", std::string(program.begin() + index, program.begin() + index + numLen)});
					foundToken = true;
					advance(numLen);
				}
				else if (points != -1)
					return std::make_pair("Invalid number of decimal points", index + numLen);
			}

			if (!foundToken)
			{
				// Character was not in the list of tokens, so check for numbers and alphanumeric values
				auto alphaLen = checkAlphaNum(program, index, tokens);
				tokenized.emplace_back(Token{"ALPHANUM", std::string(program.begin() + index, program.begin() + index + alphaLen)});
				foundToken = true;
				advance(alphaLen);
			}

			if (!foundToken)
				return std::make_pair("Invalid character " + std::string(1, program[index]), index);
		}

		return std::make_pair("PASSED", -1);
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
