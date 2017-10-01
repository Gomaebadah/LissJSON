#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include"parser.h"

#include<iostream>
#include<crtdbg.h>

int main()
{
	//Parser Test

	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	//_CrtSetBreakAlloc(259);

	Parser parser;
	JSON json;
	try{
	
		json = parser.Parse("simple.json");

	/*	int test[3];
		1[test];
		json["integer"];
		cout << json["integer"] << endl;*/
	/*	cout << json["bool"].b << endl;
		cout << json["double"].r << endl;
		cout << json["string"].s << endl;
		json["string"] = "newStirng";
		cout << json["string"].s << endl;*/

		/*cout << json["array"][0].i << endl;
		cout << json["array"][1].b << endl;
		cout << json["array"][2].s << endl;
		cout << json["array"][3][0].i << endl;
		cout << json["array"][3][1].i << endl;
		cout << json["array"][4]["key1"].s << endl;
		cout << json["array"][4]["key2"].r << endl;
		cout << json["object"]["integer"].i << endl;
		cout << json["object"]["double"].r << endl;
		cout << json["object"]["string"].s << endl;
		cout << json["object"]["array"][0].i << endl;
		cout << json["object"]["array"][1].r << endl;
		cout << json["object"]["array"][2].s << endl;
		cout << json["object"]["object"]["kk"].b << endl;
		cout << json["object"]["object"]["gg"].b << endl;*/
		
	}
	catch (std::exception e)
	{
		cout << e.what() << endl;
	}
	
	json.Release();

	return 0;
}