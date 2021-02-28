#pragma once

class Parser
{
public:
	std::vector<Token> linted;

	Parser(const std::vector<Token> &linterOutput) : linted(linterOutput)
	{}

	inline void generateAST()
	{
		for (UINT index = 0; index < linted.size(); index++)
		{
			
		}
	}
};
