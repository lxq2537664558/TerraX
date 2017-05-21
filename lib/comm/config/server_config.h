#pragma once

#include "base/macro.h"
#include "jsonstream.h"

namespace terra
{
	class ServerConfig
	{
		DISABLE_COPY(ServerConfig);
		MAKE_INSTANCE(ServerConfig);
	private:
		JsonStream js_;
	public:
		ServerConfig() = default;
		~ServerConfig() = default;

		void LoadConfigFromJson(const std::string& path);

		void GetJsonValue(const char* key_name, int32_t& val);
		void GetJsonValue(const char* key_name, std::string& val);

		void GetJsonObjectValue(const char* obj_name, const char* key_name, int32_t& val);
		void GetJsonObjectValue(const char* obj_name, const char* key_name, std::string& val);

	};


}