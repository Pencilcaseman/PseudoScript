#pragma once

#pragma warning(disable : 4996)

inline char *TO_C_STRING(OBJECT obj)
{
	char *res;
	if (GET_TYPE(obj) == "int")
	{
		auto val = *GET_VALUE(obj, int *, uint64_t *);
		
		// res = new char[ceil(log10(val)) + 2]();

		res = (char *) malloc(sizeof(char) * (ceil(log10(val)) + 2));

		sprintf(res, "%i", val);
		return res;
	}

	if (GET_TYPE(obj) == "float")
	{
		auto val = *GET_VALUE(obj, double *, uint64_t *);
		
		// res = new char[ceil(log10(val)) + 10 + 2]();
		res = (char *) malloc(sizeof(char) * (ceil(log10(val)) + 10 + 2));

		sprintf(res, "%.*f", 10, val);
		return res;
	}

	if (GET_TYPE(obj) == "string")
	{
		auto val = GET_VALUE(obj, const char *, uint64_t *);
		
		// res = new char[strlen(val) + 1]();
		res = (char *) malloc(sizeof(char) * (strlen(val) + 1));

		sprintf(res, "%s", val);
		return res;
	}

	if (GET_TYPE(obj) == "list")
	{
		// auto comma = new char[3]();
		// comma[0] = ',';
		// comma[1] = ' ';
		// comma[2] = 0;
		auto comma = ", \0";

		// auto res = new char[2];
		auto res = (char *) malloc(sizeof(char) * 2);

		res[0] = '[';
		res[1] = 0;
		uint64_t location = 1;
		uint64_t length = 2;
		for (int64_t i = 0; i < GET_ATTRIBUTE(obj, memberCount); i++)
		{
			auto strVal = TO_C_STRING(obj->members[i]);
			auto strLen = strlen(strVal);

			uint64_t commaLen = 2;
			if (i + 1 >= GET_ATTRIBUTE(obj, memberCount))
				commaLen = 0;

			res = (char *) realloc(res, sizeof(char) * (length + strLen + commaLen));
			memcpy(res + location, strVal, sizeof(char) * strLen);

			if (commaLen)
				memcpy(res + location + strLen, comma, sizeof(char) * 2);

			res[location + strLen + commaLen] = 0;

			// delete[] strVal;
			free(strVal);

			length += strLen + commaLen;
			location += strLen + commaLen;
		}

		// delete[] comma;

		res[strlen(res) - 1] = ']';

		return res;
	}
}

inline OBJECT TO_STRING(OBJECT obj)
{
	auto cStr = TO_C_STRING(obj);
	auto res = CREATE_STRING(cStr);
	
	// delete[] cStr;
	free(cStr);

	return res;
}
