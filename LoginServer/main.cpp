#include <iostream>
#include <fstream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "DataPool.h"
#include <memory>
#include "FileReader.h"
#include "DataPoolConfig.h"

using namespace rapidjson;
using namespace TerraX;
int main()
{
	/*
	//std::ifstream ifs("avatar.json");
	//IStreamWrapper isw(ifs);
	//d.ParseStream(isw);
	Document d;
	d.ParseStream(is);
	*/
	TableConfig::GetInstance().LoadTables(CONFIG_ROOT_PATH);
	auto&& val = TableConfig::GetInstance().GetTable("item");
	Row* pRow = val.CreateRows(8);
	pRow[0].SetValueString("Guid", "IIII");
	pRow[0].SetValue<int>("ItemID", 1001);

	 
	const char* szGuid = pRow[0].GetValueString(0);
	std::cout << szGuid << std::endl;
	int nItemID = pRow[0].GetValue<int>(1);
	std::cin.get();

	return 0;
}