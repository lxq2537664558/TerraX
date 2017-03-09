#include <iostream>
#include <fstream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "DataPool.h"
#include <memory>
#include "FileReader.h"
#include "DataPoolConfig.h"
#include <array>
#include "dynamic_bitset.hpp"
using namespace rapidjson;
using namespace TerraX;

template<int ITEM_COUNT>
class ItemDB
{
public:
	ItemDB(const std::string& strTableName) : strTableName_(strTableName), 
		dt_(TableConfig::GetInstance().GetTable(strTableName_.c_str()))
	{
		
	}
	int CreateItem() {
		for (std::size_t i = 0; i < arrRow_.size(); ++i) {
			if (arrRow_[i] == nullptr) {
				arrRow_[i].reset(dt_.NewRow());
				return i;
			}
		}
		return -1;
	}
	Row* GetRow(int index) {
		return arrRow_[index].get();
	}
	void SetGuid(int index, const char* szGuid)
	{
		arrRow_[index]->SetValueString("Guid", "IIII");
	}
	const char* GetGuid(int index)
	{
		return arrRow_[index]->GetValueString("Guid");
	}

	void SetItemID(int index, int nItemID)
	{
		arrRow_[index]->SetValue<int>("ItemID", 1001);
	}
	int GetItemID(int index)
	{
		return arrRow_[index]->GetValue<int>(1);
	}
private:
	const std::string& strTableName_;
	DataTable& dt_;
	std::array<std::unique_ptr<Row>, ITEM_COUNT> arrRow_;
};

int main()
{
	/*
	//std::ifstream ifs("avatar.json");
	//IStreamWrapper isw(ifs);
	//d.ParseStream(isw);
	Document d;
	d.ParseStream(is);
	*/
	dynamic_bitset bs(13);
	bs.set(2);
	bs.set(3);
	std::string str = std::move(bs.to_string());
	TableConfig::GetInstance().LoadTables(CONFIG_ROOT_PATH);
	ItemDB<64> db("item");
	int nIndex = db.CreateItem();

	db.SetGuid(nIndex, "ABCD");
	db.SetItemID(nIndex, 1001);

	const char* szGuid = db.GetGuid(nIndex);
	std::cout << szGuid << std::endl;
	int nItemID = db.GetItemID(nIndex);
	std::cin.get();

	return 0;
}