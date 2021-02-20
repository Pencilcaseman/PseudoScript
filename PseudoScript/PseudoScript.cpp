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

	std::cout << (*list->members["castToString"])(nullptr)->castToCString() << "\n";
}
