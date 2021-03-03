#if defined(DEBUG) || !defined(NDEBUG) || defined (_DEBUG)
#define PS_DEBUG
#else
#define PS_RELEASE
#endif

#include <iostream>
#include "pseudoscript/pseudoscript.h"

int main()
{
	std::cout << "PseudoScript Testing\n";

	auto a = newInt(123);
	auto b = newFloat(3.1415);
	auto c = newString("Hello, World!");

	std::cout << "Value: " << OB_INT_TO_C(a) << "\n";
	std::cout << "Value: " << OB_FLOAT_TO_C(b) << "\n";
	std::cout << "Value: " << OB_STRING_TO_C(c) << "\n";
	std::cout << "Value: " << OB_STRING_TO_C(OB_TYPE(c)->tp_copy(c)) << "\n";

	auto testList = newList(3, newInt(123), newFloat(3.14159), newString("Hello, World"));
	std::cout << "Info: " << OB_STRING_TO_C(OB_TYPE(testList)->tp_represent(testList)) << "\n";

	auto index = newInt(0);
	auto gotten = OB_TYPE(testList)->tp_methods[0].mt_meth(testList, index);
	std::cout << "Get val test: " << OB_STRING_TO_C(OB_TYPE(gotten)->tp_represent(gotten)) << "\n";
	OB_TYPE(gotten)->tp_dealloc(gotten);
	OB_TYPE(index)->tp_dealloc(index);

	OB_TYPE(a)->tp_dealloc(a);
	OB_TYPE(b)->tp_dealloc(b);
	OB_TYPE(c)->tp_dealloc(c);
	OB_TYPE(testList)->tp_dealloc(testList);

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
			std::cout << res2.details << " at line " << res2.line << "\n";
			continue;
		}

		parser.interpret(true);

		UINT loops = 10000;

		auto start = TIME;
		for (UINT i = 0; i < loops; i++)
		{
			parser.interpret(false);
		}
		auto end = TIME;
		std::cout << "Time: " << end - start << "s\n";
		std::cout << "Mean: " << ((end - start) / (double) loops) * 1000000000 << "ns\n";

		std::cout << "\n";
	}

	return 0;
}
