#pragma once

using ParserError = struct
{
	std::string details;
	UINT line;
	UINT pos;
};

class Parser
{
public:
	std::vector<Token> tokenized;
	UINT index;
	Token currentToken;

	Parser(const std::vector<Token> &lexerOutput) : tokenized(lexerOutput), index(0)
	{}

	inline void advance()
	{
		index++;
	}

	inline Token nextToken()
	{
		return tokenized[index++];
	}

	inline ParserError generateAST()
	{
		currentToken = nextToken();

		if (currentToken.name != "INT")
			return ParserError{"Expected an integer", currentToken.line, 0};
		auto left = newInt(std::stoll(currentToken.value));

		auto op = nextToken();

		if (op.name != "ADD" && op.name != "SUB")
			return ParserError{"Expected operator + or -", currentToken.line, 0};

		if (currentToken.name != "INT")
			return ParserError{"Expected an integer", currentToken.line, 0};
		auto right = newInt(std::stoll(currentToken.value));

		if (op.name == "ADD")
			std::cout << "Result: " << OB_INT_TO_C(left) + OB_INT_TO_C(right) << "\n";
		else
			std::cout << "Result: " << OB_INT_TO_C(left) - OB_INT_TO_C(right) << "\n";

		return ParserError{"PASSED", 0, 0};
	}
};
