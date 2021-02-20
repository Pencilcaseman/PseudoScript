#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <functional>
#include <map>
#include <regex>

namespace pseudo
{
#pragma region TypeEnums
	// Error types
	enum class ErrorType
	{
		SUCCESS,
		MEMBER_DOES_NOT_EXIST,
		INDEX_OUT_OF_RANGE
	};

	// Member locations
	enum class MemberLocation
	{
		UNKNOWN,
		UNDEFINED,
		CPP,
		PSEUDO
	};

	ErrorType pseudoInterpreterError = ErrorType::SUCCESS;
#pragma endregion InfoEnums

#pragma region ClassTypes

	class Interpreter;
	class Object;
	class Function;
	class None;
	class String;
	// class Bool;
	class Int;
	// class Float;
	class List;

#pragma endregion ClassTypes

#pragma region InterpreterForwardDeclare

	class Interpreter
	{
	public:
		std::vector<std::string> program;
		std::map<std::string, std::shared_ptr<Object>> heap;

		Interpreter(const std::vector<std::string> &code);

		inline std::shared_ptr<Object> interpretLine(const std::string &line);
	};

#pragma endregion InterpreterForwardDeclare

#pragma region TypeForwardDeclares

	class Object
	{
	public:
		std::string name;
		std::map<std::string, MemberLocation> memberLocations;
		std::map<std::string, std::shared_ptr<Object>> members;

		inline virtual std::string type() const
		{
			return "object";
		}

		inline virtual bool castToCBool() const = 0;
		inline virtual int64_t castToCInt() const = 0;
		inline virtual double castToCFloat() const = 0;
		inline virtual std::string castToCString() const = 0;

		inline virtual std::shared_ptr<Object> operator()(std::shared_ptr<Object> args)
		{
			// return NONE;
			return nullptr;
		}
	};

	class Function : public Object
	{
	public:
		Interpreter *interpreter;
		std::function<std::shared_ptr<Object>(std::shared_ptr <Object>)> cppFunc;
		std::vector<std::string> pseudoFunc;
		MemberLocation funcLocation;

		Function();
		Function(const std::string &str);

		Function(const std::string &str, const std::vector<std::string> &body);

		template<typename lambda>
		Function(const std::string &str, lambda func);

		inline void construct();

		inline std::string type() const override;

		bool castToCBool() const override;
		int64_t castToCInt() const override;
		double castToCFloat() const override;
		std::string castToCString() const override;

		std::shared_ptr<Object> operator()(std::shared_ptr<Object> args) override;
	};

	class String : public Object
	{
	public:
		std::string value;

		String();
		String(const std::string &str);

		inline void construct();

		inline std::string type() const override;

		bool castToCBool() const override;
		int64_t castToCInt() const override;
		double castToCFloat() const override;
		std::string castToCString() const override;
	};

	class None : public Object
	{
	public:
		None();

		inline void construct();

		inline std::string type() const override;

		bool castToCBool() const override;
		int64_t castToCInt() const override;
		double castToCFloat() const override;
		std::string castToCString() const override;
	};

	class Int : public Object
	{
	public:
		int64_t value;

		Int();
		Int(int64_t val);

		inline void construct();

		inline std::string type() const override;

		bool castToCBool() const override;
		int64_t castToCInt() const override;
		double castToCFloat() const override;
		std::string castToCString() const override;
	};

	class List : public Object
	{
	public:
		std::vector<std::shared_ptr<Object>> values;

		List();
		List(const std::vector<std::shared_ptr<Object>> &data);

		inline void construct();

		inline std::string type() const override;

		inline bool castToCBool() const override;
		inline int64_t castToCInt() const override;
		inline double castToCFloat() const override;
		inline std::string castToCString() const override;
	};

	std::shared_ptr<None> NONE = std::make_shared<None>();

#pragma endregion TypeForwardDeclares

#pragma region FunctionType

	Function::Function()
	{
		construct();
	}

	Function::Function(const std::string &str)
	{
		construct();
		name = str;
	}

	Function::Function(const std::string &str, const std::vector<std::string> &body)
	{
		construct();
		name = str;
		pseudoFunc = body;
		funcLocation = MemberLocation::PSEUDO;
	}

	template<typename lambda>
	Function::Function(const std::string &str, lambda func)
	{
		construct();
		name = str;
		cppFunc = func;
		funcLocation = MemberLocation::CPP;
	}

	inline void Function::construct()
	{
		name = "Undefined Function";
		funcLocation = MemberLocation::UNDEFINED;
	}

	inline std::string Function::type() const
	{
		return "function";
	}

	bool Function::castToCBool() const
	{
		return (funcLocation != MemberLocation::UNDEFINED) && (funcLocation != MemberLocation::UNKNOWN);
	}

	int64_t Function::castToCInt() const
	{
		return 0;
	}

	double Function::castToCFloat() const
	{
		return 0;
	}

	std::string Function::castToCString() const
	{
		return name;
	}

	std::shared_ptr<Object> Function::operator()(std::shared_ptr<Object> args)
	{
		if (funcLocation == MemberLocation::CPP)
			return cppFunc(args);
		return interpreter->interpretLine("hello");
	}

#pragma endregion FunctionType

#pragma region NoneType

	None::None()
	{
		construct();
	}

	inline void None::construct()
	{
		name = "NONE";
	}

	inline std::string None::type() const
	{
		return "NONE";
	}

	bool None::castToCBool() const
	{
		return false;
	}

	int64_t None::castToCInt() const
	{
		return 0;
	}

	double None::castToCFloat() const
	{
		return 0;
	}

	std::string None::castToCString() const
	{
		return "NONE";
	}

#pragma endregion NoneType

#pragma region IntType

	Int::Int()
	{
		construct();
	}

	Int::Int(int64_t val)
	{
		value = val;
		construct();
	}

	inline void Int::construct()
	{
		members["castToString"] = std::make_shared<Function>(
			"castToString", [&](std::shared_ptr<Object> args)
		{
			return std::make_shared<String>(std::to_string(value));
		}
		);

		members["represent"] = std::make_shared<Function>(
			"represent", [&](std::shared_ptr<Object> args)
		{
			return std::make_shared<String>(std::to_string(value));
		}
		);

		memberLocations["castToString"] = MemberLocation::CPP;
		memberLocations["represent"] = MemberLocation::CPP;
	}

	inline std::string Int::type() const
	{
		return "int";
	}

	bool Int::castToCBool() const
	{
		return (bool) value;
	}

	int64_t Int::castToCInt() const
	{
		return value;
	}

	double Int::castToCFloat() const
	{
		return (double) value;
	}

	std::string Int::castToCString() const
	{
		return std::to_string(value);
	}

#pragma endregion IntType

#pragma region StringType

	String::String()
	{
		construct();
	}

	String::String(const std::string &str)
	{
		construct();
		value = str;
	}
	
	inline void String::construct()
	{
		members["castToString"] = std::make_shared<Function>(
			"castToString", [&](std::shared_ptr<Object> args)
		{
			return std::make_shared<String>(value);
		});

		members["represent"] = std::make_shared<Function>(
			"represent", [&](std::shared_ptr<Object> args)
		{
			return std::make_shared<String>("\"" + value + "\"");
		});

		members["castToInt"] = std::make_shared<Function>(
			"castToInt", [&](std::shared_ptr<Object> args)
		{
			return std::make_shared<Int>(std::stoll(value));
		});

		members["length"] = std::make_shared<Function>(
			"length", [&](std::shared_ptr<Object> args)
		{
			return std::make_shared<Int>(value.length());
		});

		memberLocations["castToString"] = MemberLocation::CPP;
		memberLocations["represent"] = MemberLocation::CPP;
		memberLocations["castToInt"] = MemberLocation::CPP;
		memberLocations["length"] = MemberLocation::CPP;
	}

	inline std::string String::type() const
	{
		return "string";
	}

	bool String::castToCBool() const
	{
		return !value.empty();
	}

	int64_t String::castToCInt() const
	{
		return std::stoll(value);
	}

	double String::castToCFloat() const
	{
		return std::stod(value);
	}

	std::string String::castToCString() const
	{
		return value;
	}

#pragma endregion StringType

#pragma region ListType

	List::List()
	{
		construct();
	}

	List::List(const std::vector<std::shared_ptr<Object>> &data)
	{
		construct();
		values = data;
	}

	inline void List::construct()
	{
		members["castToString"] = std::make_shared<Function>(
			"castToString",
			[&](std::shared_ptr<Object> args)
		{
			std::string res = "[";
			for (uint64_t i = 0; i < values.size(); i++)
			{
				auto &val = values[i];
				res += (*val->members["represent"])(nullptr)->castToCString();

				if (i + 1 < values.size())
					res += ", ";
			}

			return std::make_shared<String>(res + "]");
		}
		);

		members["castToString"] = std::make_shared<Function>(
			"castToString",
			[&](std::shared_ptr<Object> args)
		{
			std::string res = "[";
			for (uint64_t i = 0; i < values.size(); i++)
			{
				auto &val = values[i];
				res += (*val->members["represent"])(nullptr)->castToCString();

				if (i + 1 < values.size())
					res += ", ";
			}

			return std::make_shared<String>(res + "]");
		}
		);

		memberLocations["castToString"] = MemberLocation::CPP;
	}

	inline std::string List::type() const
	{
		return "list";
	}

	inline bool List::castToCBool() const
	{
		return !values.empty();
	}

	inline int64_t List::castToCInt() const
	{
		return !values.empty();
	}

	inline double List::castToCFloat() const
	{
		return (double) !values.empty();
	}

	inline std::string List::castToCString() const
	{
		std::string res = "[";
		for (uint64_t i = 0; i < values.size(); i++)
		{
			auto &val = values[i];
			res += (*val->members["castToString"])(nullptr)->castToCString();

			if (i + 1 < values.size())
				res += ", ";
		}

		return res + "]";
	}

#pragma endregion ListType

#pragma region Interpreter

	Interpreter::Interpreter(const std::vector<std::string> &code)
	{}

	inline std::shared_ptr<Object> Interpreter::interpretLine(const std::string &line)
	{
		return std::make_shared<Int>(12345);
	}

#pragma endregion Interpreter
};
