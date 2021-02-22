#include <iostream>
#include "pseudoscript/pseudoscript.h"

int main()
{
	std::cout.precision(10);
	std::cout << std::fixed;
	std::cout << "PseudoScript\n";

	int64_t loops = 100000;
	auto start = omp_get_wtime();
	for (int64_t i = 0; i < loops; i++)
	{
		auto a = CREATE_OBJECT("int", false, 1);
		auto b = CREATE_OBJECT("int", false, 2);
		auto c = CREATE_OBJECT("int", false, 3);

		// std::cout << "A: " << GET_VALUE(a, int) << "\n";
		// std::cout << "B: " << GET_VALUE(b, int) << "\n";
		// std::cout << "C: " << GET_VALUE(c, int) << "\n";

		auto list = CREATE_OBJECT("list", {a, b, c}, false, 0);

		for (int64_t i = 0; i < GET_ATTRIBUTE(list, memberCount); i++)
		{
			if (GET_ATTRIBUTE(list->members[i], type) == "int")
				// std::cout << "Value (INT): " << GET_VALUE(list->members[i], int) << "\n";
				auto val = GET_VALUE(list->members[i], int, uint64_t);
		}

		DESTROY_OBJECT(a);
		DESTROY_OBJECT(b);
		DESTROY_OBJECT(c);
		DESTROY_OBJECT(list);
	}
	auto end = omp_get_wtime();

	std::cout << "Time: " << end - start << "s\n";
	std::cout << "Mean: " << ((end - start) / (double) loops) * 1000000000 << "ns\n";

	loops = 100000;
	start = omp_get_wtime();
	auto a = CREATE_OBJECT("int", false, 1);
	for (int64_t i = 0; i < loops; i++)
	{
		SET_VALUE(a, 123456, uint64_t);
	}
	DESTROY_OBJECT(a);
	end = omp_get_wtime();

	std::cout << "\n\nTime: " << end - start << "s\n";
	std::cout << "Mean: " << ((end - start) / (double) loops) * 1000000000 << "ns\n";

	auto floatVal = CREATE_OBJECT("float", false, 3.1415);
	std::cout << GET_VALUE(floatVal, double, uint64_t) << "\n";

	const char *x = "Hello, World!";
	auto val = PTR_CAST(uint64_t, &x);

	std::cout << x << "\n";
	std::cout << val << "\n";
	std::cout << PTR_EVAL(const char *, val) << "\n";

	return 0;
}
