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

	auto a = std::make_shared<pseudo::Int>(5);
	std::cout << a->castToCFloat() << "\n";
}
