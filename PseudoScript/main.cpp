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

	auto linter = Lexer({"numb = 3.141.5;",
						 "print(\"Hello, World!\");"});
	
	auto res = linter.tokenize();

	if (res.details != "PASSED")
	{
		std::cout << "Linter error: \"" << res.details << "\" at position " << res.line << ":" << res.charPos << "\n";
	}
	else
	{
		for (const auto &token : linter.tokenized)
		{
			std::cout << "Token: " << token.name << " | " << token.value << "\n";
		}
	}

	return 0;
}
