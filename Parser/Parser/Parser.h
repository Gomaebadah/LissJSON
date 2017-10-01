#pragma once
#include<string>
#include<istream>
#include<fstream>
#include<exception>
using namespace std;
using cstring = char*;

enum class TokenType{
	STRING,
	INT,
	DOUBLE,
	OBJECT,
	BOOL,
	UNDEFINED
};

struct Token{
	Token()
	{
		type = TokenType::UNDEFINED;
	}
	~Token()
	{

	}
	TokenType type;
	union{
		cstring s;
		int i;
		double r;
		bool b;
		struct{
			Token* objectKeys;
			Token* objectValues;
			int objectSize;
		};
	};
	Token& operator[](int integer) const;
	Token& operator[](cstring str) const;

	Token& operator=(int integer);
	Token& operator=(double real);
	Token& operator=(const cstring str);
	Token& operator=(bool boolean);


	void Release();
};
using JSON = Token;

class Parser
{
private:
	int GetObjectSize(istream& in);
	int GetArraySize(istream& in);
	int GetStringLength(istream& in);
	bool IsInteger(istream& in);
	int GetNumberLength(istream& in);
public:
	Parser();
	~Parser();

	JSON Parse(const std::string& path);
	JSON Parse(istream& in);
};

