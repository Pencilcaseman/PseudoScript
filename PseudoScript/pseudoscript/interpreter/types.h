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
	constexpr uint64_t CAST_ERROR = 1;
	uint64_t EdExInterpreterError = 0;

	class Object
	{
	public:
		inline virtual std::string type() const
		{
			return "object";
		}

		std::map<std::string, int> memberLocations;
		std::map<std::string, std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)>> cppMembers;
		std::map<std::string, std::shared_ptr<Object>> edexMembers;

		inline virtual bool castToCBool() const = 0;
		inline virtual int64_t castToCInt() const = 0;
		inline virtual double castToCFloat() const = 0;
		inline virtual std::string castToCString() const = 0;
	};

	class String : public Object
	{
	public:
		std::string value;

		String()
		{
			construct();
		}

		String(const std::string &str) : value(str)
		{
			construct();
		};

		inline void construct()
		{
			cppMembers["to_string"] = [=](const std::shared_ptr<Object> &args)
			{
				return std::make_shared<String>(value);
			};
		}

		inline std::string type() const override
		{
			return "string";
		}

		bool castToCBool() const override
		{
			return !value.empty();
		}

		int64_t castToCInt() const override
		{
			return std::stoll(value);
		}

		double castToCFloat() const override
		{
			return std::stod(value);
		}

		std::string castToCString() const override
		{
			return value;
		}
	};

	class Int : public Object
	{
	public:
		int64_t value;

		Int()
		{
			construct();
		}

		Int(int64_t val) : value(val)
		{
			construct();
		}

		inline void construct()
		{
			cppMembers["to_string"] = [=](const std::shared_ptr<Object> &args)
			{
				return std::make_shared<String>(std::to_string(value));
			};
		}

		inline std::string type() const
		{
			return "int";
		}

		bool castToCBool() const override
		{
			return value;
		}

		int64_t castToCInt() const override
		{
			return value;
		}

		double castToCFloat() const override
		{
			return (double) value;
		}

		std::string castToCString() const override
		{
			return std::to_string(value);
		}
	};

	class Interpreter
	{
	public:
		std::vector<std::string> program;
		std::map<std::string, Object *> heap;

		Interpreter(const std::vector<std::string> &code) : program(code)
		{}
	};
}