#define PROGRAM_MEM_SIZE 65536   // Memory buffer size for the program. Bigger = more ram
#define CUSTOM_ALLOCATOR         // Disable this to use normal C++ allocations

// DEBUG, NDEBUG, _DEBUG
#if defined(DEBUG) || !defined(NDEBUG) || defined (_DEBUG)
#define PS_DEBUG
#else
#define PS_RELEASE
#endif

#include <iostream>
#include "pseudoscript/pseudoscript.h"

int main()
{
	objectAllocator = newAllocator(PROGRAM_MEM_SIZE);

	std::cout << "PseudoScript Testing\n";

	auto a = newInt(123);
	auto b = newFloat(3.1415);
	auto c = newString("Hello, World!");

	std::cout << "Value: " << OB_INT_TO_C(a) << "\n";
	std::cout << "Value: " << OB_FLOAT_TO_C(b) << "\n";
	std::cout << "Value: " << OB_STRING_TO_C(c) << "\n";
	std::cout << "Value: " << OB_STRING_TO_C(OB_TYPE(c)->tp_value(c)) << "\n";

	OB_TYPE(a)->tp_dealloc(a);
	OB_TYPE(b)->tp_dealloc(b);
	OB_TYPE(c)->tp_dealloc(c);

	std::cout << "\n\n\n";

	auto lexer = Lexer({"abc = 3.1415;"});
	
	auto res = lexer.tokenize();

	if (res.details != "PASSED")
	{
		std::cout << "Linter error: \"" << res.details << "\" at position " << res.line << ":" << res.charPos << "\n";
	}
	else
	{
		for (const auto &token : lexer.tokenized)
		{
			std::cout << "Line " << token.line << ": " << token.name << " | " << token.value << "\n";
		}
	}

	Parser parser(lexer.tokenized);

	auto res2 = parser.generateAST();

	if (res2.details != "PASSED")
	{
		std::cout << "Error: " << res2.details << " at line " << res2.line << "\n";
	}

	return 0;
}
