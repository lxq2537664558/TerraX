#include <iostream>
#include <fstream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "DataPool.h"
#include <memory>

using namespace rapidjson;
int main()
{
	FILE* fp = fopen("avatar.json", "rb"); // 非 Windows 平台使用 "r", window "rb"
	fseek(fp, 0, SEEK_END); //定位到文件末 
	long file_length = ftell(fp); //文件长度
	fseek(fp, 0, SEEK_SET);
	std::unique_ptr<char> readBuffer(new char[file_length]);
	FileReadStream is(fp, readBuffer.get(), file_length);
	//std::ifstream ifs("avatar.json");
	//IStreamWrapper isw(ifs);
	//d.ParseStream(isw);
	Document d;
	d.ParseStream(is);
	fclose(fp);
	Column col;
	static const char* kTypeNames[] =
	{ "Null", "False", "True", "Object", "Array", "String", "Number" };
	for (Value::ConstMemberIterator itr = d.MemberBegin();
		itr != d.MemberEnd(); ++itr)
	{
		//printf("Type of member %s is %s\n",
			//itr->name.GetString(), kTypeNames[itr->value.GetType()]);
		auto val = d[itr->name].GetObject();
		/*for (auto it = val.MemberBegin(); it != val.MemberEnd(); ++it) {
			printf("Type of member %s is %s\n",
				it->name.GetString(), kTypeNames[it->value.GetType()]);
		}*/
		col.PushProperty(val["Field_Name"].GetString(), val["Type"].GetString(), 
			val["Public"].GetInt(), val["Private"].GetInt(), val["Save"].GetInt());
	}
	col.AllocBuffer();

	col.SetValueString(0, "Aaaa");
	auto val0 = col.GetValueString(0);

	col.SetValue(1, 99);
	auto val1 = col.GetValue<int>(1);

	std::cin.get();


	return 0;
}