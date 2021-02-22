/*
#include <iostream>
#include "pseudoscript/pseudoscript.h"

int main()
{
	std::cout << "_|_|_|                                        _|              _|_|_|                      _|              _|	  \n";
	std::cout << "_|    _|    _|_|_|    _|_|    _|    _|    _|_|_|    _|_|    _|          _|_|_|  _|  _|_|      _|_|_|    _|_|_|_|\n";
	std::cout << "_|_|_|    _|_|      _|_|_|_|  _|    _|  _|    _|  _|    _|    _|_|    _|        _|_|      _|  _|    _|    _|	  \n";
	std::cout << "_|            _|_|  _|        _|    _|  _|    _|  _|    _|        _|  _|        _|        _|  _|    _|    _|	  \n";
	std::cout << "_|        _|_|_|      _|_|_|    _|_|_|    _|_|_|    _|_|    _|_|_|      _|_|_|  _|        _|  _|_|_|        _|_|\n";
	std::cout << "                                                                                              _|                \n";

	auto a = std::make_shared<pseudo::Int>(1);
	auto b = std::make_shared<pseudo::String>("Hello!");
	auto c = std::make_shared<pseudo::Int>(3);
	auto list = std::make_shared<pseudo::List>(std::vector<std::shared_ptr<pseudo::Object>>{a, b, c});

	auto list2D = std::make_shared<pseudo::List>(std::vector<std::shared_ptr<pseudo::Object>>{list, list, list});

	(*list2D->members["setItem"])(std::make_shared<pseudo::List>(std::vector<std::shared_ptr<pseudo::Object>>{std::make_shared<pseudo::Int>(1), std::make_shared<pseudo::String>("This is some text to replace a list")}));

	std::cout << (*list->members["castToString"])(nullptr)->castToCString() << "\n";
	std::cout << (*list2D->members["castToString"])(nullptr)->castToCString() << "\n";

	for (int i = 0; i < 5; i++)
		std::cout << (*list->members["getItem"])(std::make_shared<pseudo::Int>(i))->castToCString() << "\n";

	std::cout << "\n\n\n";

	for (int i = 0; i < 5; i++)
		std::cout << (*list2D->members["getItem"])(std::make_shared<pseudo::Int>(i))->castToCString() << "\n";

	if (pseudo::interpreterError != pseudo::ErrorType::SUCCESS)
	{
		std::cout << "Error was: " << pseudo::errorDescriptions[(uint64_t) pseudo::interpreterError] << "\n";
	}

	int loops = 100000;

	auto start = TIME;
	for (int i = 0; i < loops; i++)
		auto thing = std::make_shared<pseudo::Int>(123);
	auto end = TIME;

	std::cout << "Time: " << (end - start) / double(loops) << "ms";
}
*/
