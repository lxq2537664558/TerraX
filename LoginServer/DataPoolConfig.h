#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include "DataPool.h"
#include "FileReader.h"
#include "base_macro.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/istreamwrapper.h"

using namespace rapidjson;
namespace TerraX
{
#define CONFIG_ROOT_PATH "."

	static const char* kJsonTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

	class TableConfig
	{
		MAKE_INSTANCE(TableConfig);

	private:
		std::map<std::string, DataTable> m_TableMap;

	public:
		TableConfig() = default;
		~TableConfig() = default;

		void LoadTables(const std::string& foler_path)
		{
			FileReader fr(".");
			auto& files = fr.GetFileNames();
			for (const auto& val : files) {
				LoadFile(val);
			}
		}
		DataTable& GetTable(const char* szTableName) {
			auto& iter = m_TableMap.find(szTableName);
			assert(iter != m_TableMap.end());
			return iter->second;
		}
	private:
		void LoadFile(const FileInfo_t& fi)
		{
			FILE* fp = fopen(fi.szFilePath, "rb"); // 非 Windows 平台使用 "r", window "rb"
			fseek(fp, 0, SEEK_END); //定位到文件末 
			long file_length = ftell(fp); //文件长度
			fseek(fp, 0, SEEK_SET);
			std::unique_ptr<char> readBuffer(new char[file_length]);
			FileReadStream is(fp, readBuffer.get(), file_length);
			Document d;
			d.ParseStream(is);
			fclose(fp);

			m_TableMap.emplace(fi.szFileName, DataTable());

			m_TableMap[fi.szFileName].SetTableName(fi.szFileName);
			ParseFromDoc(d, m_TableMap[fi.szFileName]);
		}
		void ParseFromDoc(Document& d, DataTable& dt)
		{
			for (Value::ConstMemberIterator itr = d.MemberBegin(); itr != d.MemberEnd(); ++itr) {
				auto val = d[itr->name].GetObject();
				assert(d.IsObject() && val.HasMember("Field_Name"));
				assert(d.IsObject() && val.HasMember("Type"));
				assert(d.IsObject() && val.HasMember("Public"));
				assert(d.IsObject() && val.HasMember("Private"));
				assert(d.IsObject() && val.HasMember("Save"));
				dt.GetColumn().PushProperty(val["Field_Name"].GetString(), val["Type"].GetString(),
					val["Public"].GetInt(), val["Private"].GetInt(), val["Save"].GetInt());
			}
		}
	};
}