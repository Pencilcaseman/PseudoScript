#pragma once

using ParserError = struct
{
	std::string details;
	UINT line;
	UINT pos;
};

typedef struct
{
	OB_BASE;
	Object *left;
	Object *operation;
	Object *right;
} BinaryOperation;

static Object *BinaryOperation_alloc(TypeObject *self, UINT nItems)
{
	return (Object *) OB_MALLOC(self->tp_size * (nItems == 0 ? 1 : nItems));
}

static void BinaryOperation_free(void *self)
{
	OB_FREE(self);
}

static void BinaryOperation_dealloc(BinaryOperation *self)
{
	OB_TYPE(self->left)->tp_free(self->left);
	OB_TYPE(self->operation)->tp_free(self->operation);
	OB_TYPE(self->right)->tp_free(self->right);
	OB_TYPE(self)->tp_free(self);
}

static Object *BinaryOperation_new(TypeObject *type, Object *args)
{
	BinaryOperation *self = (BinaryOperation *) type->tp_alloc(type, 0);

	if (self != nullptr)
	{
		self->left = nullptr;
		self->operation = nullptr;
		self->right = nullptr;
	}

	return (Object *) self;
}

static int BinaryOperation_init(BinaryOperation *self, Object *args)
{
	return 0;
}

static GetSet BinaryOperation_getset[] = {
	{nullptr}
};

Object *newBinaryOperation(Object *left, Object *op, Object *right);

static Object *BinaryOperation_copy(BinaryOperation *self)
{
	return newBinaryOperation(self->left, self->operation, self->right);
}

static Object *BinaryOperation_represent(BinaryOperation *self)
{
	const char *left = OB_STRING_TO_C(OB_TYPE(self->left)->tp_toString(self->left));
	const char *operation = OB_STRING_TO_C(OB_TYPE(self->operation)->tp_toString(self->operation));
	const char *right = OB_STRING_TO_C(OB_TYPE(self->right)->tp_toString(self->right));

	auto leftLen = strlen(left);
	auto opLen = strlen(operation);
	auto rightLen = strlen(right);

	auto resLen = leftLen + opLen + rightLen + 6;

	char *res = (char *) OB_MALLOC(sizeof(char) * resLen);

#pragma warning(suppress : 4996)
	sprintf(res, "(%s, %s, %s)", left, operation, right);

	return newString(res);
}

static Object *BinaryOperation_toString(BinaryOperation *self)
{
	return BinaryOperation_represent(self);
}

static TypeObject BinaryOperation_type = {
	"parser_binop",                           // Name of type
	sizeof(BinaryOperation),                // Size of the object in bytes
	BinaryOperation_new,                    // Allocate a new object and return a pointer to it
	(constructor) BinaryOperation_init,        // Initialize an object
	(destructor) BinaryOperation_dealloc,                // Free an object that has been created
	BinaryOperation_alloc,                  // Allocate memory for an object of this type
	BinaryOperation_free,                   // Free an object of a given type
	BinaryOperation_getset,                 // Getters and setters
	(CFunction) BinaryOperation_copy,       // Return an exact copy of a value
	(CFunction) BinaryOperation_represent,  // Return the value stored in a string form
	(CFunction) BinaryOperation_toString    // Return a string representation of the value
	// Return an integer representation of the value
	// Return a floating point representation of the value
	// Return the result of addition
	// Return the result of subtraction
	// Return the result of multiplication
	// Return the result of division
	// Return the result of raising to the power of a value
	// Return the result of reverse addition -- see notes ^^^
	// Return the result of reverse subtraction -- see notes ^^^
	// Return the result of reverse multiplication -- see notes ^^^
	// Return the result of reverse division -- see notes ^^^
	// Return the result of the reverse power -- see notes ^^^
	// Member definitions for the type
	// Method definitions for the type
};

Object *newBinaryOperation(Object *left, Object *op, Object *right)
{
	BinaryOperation *res = newObject(BinaryOperation, &BinaryOperation_type);
	res->left = left;
	res->operation = op;
	res->right = right;
	return (Object *) res;
}

class Parser
{
public:
	std::vector<Token> tokenized;
	UINT index;
	Token currentToken;
	std::string errorDetails = "NONE";

	Parser(const std::vector<Token> &lexerOutput) : tokenized(lexerOutput), index(0)
	{
		currentToken = nextToken();
	}

	inline void advance()
	{
		index++;
	}

	inline Token nextToken()
	{
		return tokenized[index++];
	}

	inline Object *error(const std::string &description = "NONE")
	{
		errorDetails = description;
		return nullptr;
	}

	inline Object *eat(const char *type)
	{
		if (currentToken.name == type)
		{
			currentToken = nextToken();
			return (Object *) (void *) 1;
		}
		return error("Syntax Error: Expected type '" + std::string(type) + "'");
	}

	inline Object *factor()
	{
		Token token = currentToken;

		if (currentToken.name == "SUB")
		{
			if (eat("SUB") == nullptr)
				return nullptr;

			token = currentToken;

			if (currentToken.name == "INT")
			{
				if (eat("INT") == nullptr)
					return nullptr;
				return newInt(-std::stoll(token.value));
			}

			if (currentToken.name == "FLOAT")
			{
				if (eat("FLOAT") == nullptr)
					return nullptr;
				return newFloat(-std::stod(token.value));
			}

			return nullptr;
		}

		if (currentToken.name == "INT")
		{
			if (eat("INT") == nullptr)
				return nullptr;
			return newInt(std::stoll(token.value));
		}

		if (currentToken.name == "FLOAT")
		{
			if (eat("FLOAT") == nullptr)
				return nullptr;
			return newFloat(std::stod(token.value));
		}

		if (currentToken.name == "LPAREN")
		{
			if (eat("LPAREN") == nullptr)
				return nullptr;
			auto node = expression();
			if (eat("RPAREN") == nullptr)
				return nullptr;
			return node;
		}

		return error("Syntax Error: Unable to find variable or number literal");
	}

	inline Object *term()
	{
		auto node = factor();

		if (node == nullptr)
			return nullptr;

		while (currentToken.name == "MUL" || currentToken.name == "DIV")
		{
			Token token = currentToken;

			if (token.name == "MUL")
			{
				if (eat("MUL") == nullptr)
				{
					return error("Syntax Error: Expected '*'");
				}
			}
			else if (token.name == "DIV")
			{
				if (eat("DIV") == nullptr)
				{
					return error("Syntax Error: Expected '/'");
				}
			}

			char *name = (char *) OB_MALLOC(sizeof(char) * token.name.length());
			memcpy(name, token.name.c_str(), sizeof(char) * token.name.length());

			auto right = term();
			if (right == nullptr)
				return error("Syntax Error: Expected term");

			node = newBinaryOperation(node, newString(name), right);
		}

		return node;
	}

	inline Object *expression()
	{
		auto node = term();

		if (node == nullptr)
			return nullptr;

		while (currentToken.name == "ADD" || currentToken.name == "SUB")
		{
			Token token = currentToken;

			if (token.name == "ADD")
			{
				if (eat("ADD") == nullptr)
				{
					return error("Syntax Error: Expected '+'");
				}
			}
			else if (token.name == "SUB")
			{
				if (eat("SUB") == nullptr)
				{
					return error("Syntax Error: Expected '-'");
				}
			}

			char *name = (char *) OB_MALLOC(sizeof(char) * token.name.length());
			memcpy(name, token.name.c_str(), sizeof(char) * token.name.length());

			auto right = term();
			if (right == nullptr)
				return error("Syntax Error: Expected term");

			node = newBinaryOperation(node, newString(name), right);
		}

		return node;
	}

	inline ParserError generateAST()
	{
		auto tree = expression();

		if (tree == nullptr)
			return ParserError{errorDetails, tokenized[index - 1].line, 0};

		std::cout << OB_STRING_TO_C(OB_TYPE(tree)->tp_toString(tree)) << "\n";

		return ParserError{"PASSED", 0, 0};
	}
};
