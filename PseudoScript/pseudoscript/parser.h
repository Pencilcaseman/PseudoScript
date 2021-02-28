#pragma once

using ParserError = struct
{
	std::string details;
	UINT line;
	UINT pos;
};

enum class NodeType
{
	ORIGIN,
	ASSIGNMENT,
	VARIABLE
};

class AST
{
public:
	NodeType type;
	std::vector<std::shared_ptr<AST>> children;

	AST() = default;

	virtual inline std::string getName() const
	{
		throw std::runtime_error("AST type does not implement 'getName()'");
	}

	virtual inline std::string getVarType() const
	{
		throw std::runtime_error("AST type does not implement 'getName()'");
	}
};

class Variable : public AST
{
public:
	std::string name;
	std::string varType;
	NodeType type = NodeType::VARIABLE;

	Variable(const std::string &n) : name(n)
	{}

	Variable(const std::string &n) : name(n)
	{}

	std::string getName() const override
	{
		return name;
	}

	std::string getVarType() const override
	{
		return varType;
	}
};

class Assignment : public AST
{
public:
	std::shared_ptr<AST> var;
	std::shared_ptr<AST> expr;
	NodeType type = NodeType::ASSIGNMENT;

	Assignment(std::shared_ptr<AST> left, std::shared_ptr<AST> right) : var(left), expr(right)
	{}
};

class Parser
{
public:
	std::vector<Token> tokenized;
	AST tree;

	Parser(const std::vector<Token> &lexerOutput) : tokenized(lexerOutput)
	{}

	inline ParserError generateAST()
	{
		for (UINT index = 0; index < tokenized.size(); index++)
		{
			UINT EOL = -1;

			for (UINT i = index; i < tokenized.size(); i++)
			{
				if (tokenized[i].name == "SEMICOLON")
				{
					EOL = i;
					break;
				}
			}

			if (tokenized[index].name == "EQ") // Check for an assignment operation
			{
				if (index > 0)
				{
					if (tokenized[index - 1].name == "ALPHANUM")
					{
						std::cout << "Found an assignment: Value: " << tokenized[index - 1].value << " | Value: " << tokenized[index + 1].value << "\n";
						
						Variable var(tokenized[index - 1].value);
						auto varPtr =  std::make_shared<Variable>(var);
						Assignment assignmentOp(varPtr, varPtr);

						tree.children.emplace_back(assignmentOp);
					}
					else
					{
						return ParserError{"Invalid Type. Assignment operator requires a variable type", tokenized[index].line, 0};
					}
				}
				else
				{
					return ParserError{"Invalid syntax. Cannot place token \"=\" here", tokenized[index].line, 0};
				}
			}

			std::cout << "EOL at " << EOL << "\n";
		}

		return ParserError{"PASSED", 0, 0};
	}
};
