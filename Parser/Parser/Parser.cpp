#include "Parser.h"

static void IgnoreSpace(istream& in);
static char GetChar(istream& in);

static void IgnoreSpace(istream& in)
{
	while (isspace(in.peek()))
		in.get();
}
static char GetChar(istream& in)
{
	IgnoreSpace(in);
	char ret = in.get();
	IgnoreSpace(in);
	return ret;
}

Token& Token::operator[](int integer) const
{
	int index;
	for (int i = 0; i < objectSize; i++)
	{
		if (objectKeys[i].i == integer)
		{
			index = i;
			break;
		}
	}
	return objectValues[index];
}
//Token& Token::operator[](double real)
//{
//	int index;
//	for (int i = 0; i < objectSize; i++)
//	{
//		if (objectKeys[i].r == real)
//		{
//			index = i;
//			break;
//		}
//	}
//	return objectValues[index];
//}
Token& Token::operator[](cstring str) const
{
	int index;
	for (int i = 0; i < objectSize; i++)
	{
		if (strcmp(objectKeys[i].s, str) == 0)
		{
			index = i;
			break;
		}
	}
	return objectValues[index];
}

Token& Token::operator=(int integer)
{
	Release();
	type = TokenType::INT;
	i = integer;
	return *this;
}
Token& Token::operator=(double real)
{
	Release();
	type = TokenType::DOUBLE;
	r = real;
	return *this;
}
Token& Token::operator=(const cstring str)
{
	Release();
	type = TokenType::STRING;
	s = new char[strlen(str)+1];
	memcpy(s,str, strlen(str)+1);
	return *this;
}
Token& Token::operator=(bool boolean)
{
	Release();
	type = TokenType::BOOL;
	b = boolean;
	return *this;
}



void Token::Release()
{
	if (type == TokenType::OBJECT)
	{
		for (int i = 0; i < objectSize; i++)
		{
			objectKeys[i].Release();
			objectValues[i].Release();
		}
		delete[] objectKeys;
		delete[] objectValues;
	}
	else if (type == TokenType::STRING)
	{
		delete[] s;
	}
}

Parser::Parser()
{
}


Parser::~Parser()
{
}
int Parser::GetObjectSize(istream& in)
{
	//implementation : count ':'s between '{' and '}'
	streampos beginPos = in.tellg();
	int size = 0;
	int depth = 0;
	bool isInString = false;
	while (true)
	{
		char c = in.get();
		if (c == ':' && depth == 0 && !isInString)
			++size;
		else if (c == '{')
			++depth;
		else if (c == '}')
		{
			--depth;
			if (depth == -1)
				break;
		}
		if (c == '"')
			isInString = !isInString;
	}
	in.seekg(beginPos);
	return size;
}
int Parser::GetArraySize(istream& in)
{
	//implementation : count ','s between '[' and ']'
	int size = 1;
	int depth = 0;
	bool isInString = false;
	streampos beginPos = in.tellg();
	while (true)
	{
		char c = in.get();
		if (c == ',' && depth == 0 && !isInString)
			++size;
		else if (c == '{' || c== '[')
			++depth;
		else if (c == '}' || c== ']')
		{
			--depth;
			if (depth == -1)
				break;
		}
		if (c == '"')
			isInString = !isInString;
	}
	in.seekg(beginPos);
	return size;

}
int Parser::GetStringLength(istream& in)
{
	int length = 0;
	streampos beginPos = in.tellg();
	while (in.get() != '"')
	{
		length++;
	}
	in.seekg(beginPos);

	
	return length;
}

bool Parser::IsInteger(istream& in)
{
	bool isInteger = true;
	streampos beginPos = in.tellg();
	while (true)
	{
		char c = in.get();
		
		if (c == '.')
		{
			isInteger = false;
			break;
		}
		if (!isdigit(c))
		{
			break;
		}
	}
	in.seekg(beginPos);
	return isInteger;
}

int Parser::GetNumberLength(istream& in)
{
	int length = 0;
	streampos beginPos = in.tellg();
	while (true)
	{
		char c = in.get();
		if (!isdigit(c) && c != '.')
			break;

		length++;
	}
	in.seekg(beginPos);
	return length;
}

JSON Parser::Parse(const std::string& path)
{
	JSON json;
	ifstream ifs(path, ios::binary);
	json = Parse(ifs);
	ifs.close();
	return json;
}
JSON Parser::Parse(istream& in)
{
	JSON json;
	IgnoreSpace(in);
	char c = in.get();
	//if it's Object
	if (c == '{')
	{
		IgnoreSpace(in);
		json.type = TokenType::OBJECT;
		json.objectSize = GetObjectSize(in);
		json.objectKeys = new Token[json.objectSize];
		json.objectValues = new Token[json.objectSize];
		for (int i = 0; i < json.objectSize; i++)
		{
			IgnoreSpace(in);
			//key
			json.objectKeys[i] = Parse(in);
			//Colon (:)
			if (in.get() != ':')
				throw std::exception("Object Parse Error");
			//Value
			json.objectValues[i] = Parse(in); 
			IgnoreSpace(in);
			
			if (i == json.objectSize - 1)
			{
				break;
			}

			if (in.get() != ',')
				throw std::exception("Object Parse Error");
			
			
		}
		//( } )
		if (in.get() != '}')
			throw std::exception("Object Parse Error");
	
			
	}
	//if it's Array
	else if (c == '[')
	{
		json.type = TokenType::OBJECT;
		json.objectSize = GetArraySize(in);
		json.objectKeys = new Token[json.objectSize];
		json.objectValues = new Token[json.objectSize];
		for (int i = 0; i < json.objectSize; i++)
		{
			IgnoreSpace(in);
			//Key
			Token key;
			key.i = i;
			key.type = TokenType::INT;
			json.objectKeys[i] = key;
			json.objectValues[i] = Parse(in);
			IgnoreSpace(in);

			if (i == json.objectSize - 1)
			{
				break;
			}
			if (in.get() != ',')
				throw std::exception("Array Parse Error");
		}
		//( ] )
		if (in.get() != ']')
			throw std::exception("Array Parse Error");
	}
	//if it's String
	else if (c == '"')
	{
		json.type = TokenType::STRING;
		int length = GetStringLength(in);
		json.s = new char[length+1];
	
		in.read(json.s, length);

		json.s[length] = '\0';
	
		if (in.get() != '"')
			throw std::exception("String Parse Error");
	}
	//if it's Integer or Real
	else if (isdigit(c))
	{
		IgnoreSpace(in);
		int length = GetNumberLength(in);
		cstring tmp = new char[length+2];
		tmp[0] = c;
		if (IsInteger(in))
		{
			json.type = TokenType::INT;
			in.read(tmp+1,length);
			tmp[length+1] = '\0';
			json.i = atoi(tmp);
		}
		else
		{
			json.type = TokenType::DOUBLE;
			in.read(tmp+1, length);
			tmp[length+1] = '\0';
			json.r = atof(tmp);
		}
		delete tmp;
 	}
	//if it's bool
	else if (c == 't' || c == 'f')
	{
		json.type = TokenType::BOOL;
		IgnoreSpace(in);
		if (c == 't')
		{
			in.seekg(in.tellg()+3LL);
			char c = in.peek();
			json.b = true;
		}
		else
		{
			in.seekg(in.tellg()+4LL);
			json.b = false;
		}	
	}
	IgnoreSpace(in);
	return json;
}