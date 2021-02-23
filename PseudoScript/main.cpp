#include <iostream>
#include "pseudoscript/pseudoscript.h"

int main()
{
	//const char *val = "Test Sentence";
	//
	//delete[] val;

	std::cout.precision(10);
	std::cout << std::fixed;
	std::cout << "PseudoScript\n";

	uint64_t loops = 1;

	auto start = omp_get_wtime();
	for (uint64_t i = 0; i < loops; i++)
	{
		auto a = CREATE_INT(1);
		auto b = CREATE_STRING("ABCDE");
		auto c = CREATE_FLOAT(3);

		auto list = CREATE_OBJECT("list", {a, b, c}, nullptr);

		SET_VALUE(a, INT(123), "int");

		for (int64_t i = 0; i < GET_ATTRIBUTE(list, memberCount); i++)
		{
			if (GET_TYPE(list->members[i]) == "int")
			{
				auto val = *GET_VALUE(list->members[i], int *, uint64_t *);
				std::cout << val << "\n";
			}

			if (GET_TYPE(list->members[i]) == "float")
			{
				auto val = *GET_VALUE(list->members[i], double *, uint64_t *);
				std::cout << val << "\n";
			}

			if (GET_TYPE(list->members[i]) == "string")
			{
				auto val = GET_VALUE(list->members[i], const char *, uint64_t *);
				std::cout << val << "\n";
			}
		}

		DESTROY_OBJECT(a);
		DESTROY_OBJECT(b);
		DESTROY_OBJECT(c);
		DESTROY_OBJECT(list);
	}
	auto end = omp_get_wtime();

	std::cout << "Time: " << end - start << "s\n";
	std::cout << "Mean: " << ((end - start) / (double) loops) * 1000000000 << "ns\n";

	return 0;
}
