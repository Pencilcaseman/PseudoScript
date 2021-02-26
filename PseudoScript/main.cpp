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

	std::cout << "Value: " << OB_INT_TO_C(a) << "\n";
	std::cout << "Value: " << OB_FLOAT_TO_C(b) << "\n";

	OB_TYPE(a)->tp_dealloc(a);
	OB_TYPE(b)->tp_dealloc(b);

	return 0;
}
