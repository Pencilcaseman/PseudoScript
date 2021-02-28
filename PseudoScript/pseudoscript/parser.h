#pragma once

class AST
{
public:

};



class Parser
{
public:
	std::vector<Token> linted;

	Parser(const std::vector<Token> &linterOutput) : linted(linterOutput)
	{}


};
