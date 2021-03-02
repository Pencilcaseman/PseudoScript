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
	std::cout << "Value: " << OB_STRING_TO_C(OB_TYPE(c)->tp_copy(c)) << "\n";

	auto testList = newList(4, newInt(123), newFloat(3.14159), newString("Hello, World"), newList(3, newInt(123), newFloat(3.14159), newString("Hello, World")));
	std::cout << "Info: " << OB_STRING_TO_C(OB_TYPE(testList)->tp_represent(testList)) << "\n";
	
	auto gotten = OB_TYPE(testList)->tp_methods[0].mt_meth(testList, newInt(3));
	std::cout << "Get val test: " << OB_STRING_TO_C(OB_TYPE(gotten)->tp_represent(gotten)) << "\n";
	OB_TYPE(c)->tp_dealloc(gotten);

	OB_TYPE(a)->tp_dealloc(a);
	OB_TYPE(b)->tp_dealloc(b);
	OB_TYPE(c)->tp_dealloc(c);
	OB_TYPE(c)->tp_dealloc(testList);

	std::cout << "\n\n\n";

	while (true)
	{
		std::cout << "Pseudo >>> ";
		std::string line;
		std::getline(std::cin, line);

		if (line == "exit")
			return 0;

		std::cout << "\n";

		auto lexer = Lexer({line});

		auto res = lexer.tokenize();

		if (res.details != "PASSED")
		{
			std::cout << "Linter error: \"" << res.details << "\" at position " << res.line << ":" << res.charPos << "\n";
			continue;
		}

		Parser parser(lexer.tokenized);

		auto res2 = parser.generateAST();

		if (res2.details != "PASSED")
		{
			std::cout << "Error: " << res2.details << " at line " << res2.line << "\n";
			continue;
		}

		std::cout << "\n";
	}

	return 0;
}
