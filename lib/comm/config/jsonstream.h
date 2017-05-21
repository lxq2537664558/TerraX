#pragma once

#include "base/types.h"
#include "rapidjson/document.h"

using namespace rapidjson;
namespace terra
{
	class JsonStream
	{
	private:
		Document doc_;
	public:
		JsonStream() = default;
		~JsonStream() = default;

		void LoadFile(const std::string& path);

		void GetValue(const char* key_name, int32_t& val);
		void GetValue(const char* key_name, std::string& val);

		void GetObjectValue(const char* obj_name, const char* key_name, int32_t& val);
		void GetObjectValue(const char* obj_name, const char* key_name, std::string& val);
	};


}