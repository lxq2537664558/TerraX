#include "server_config.h"
#include <sys/stat.h> 
using namespace terra;

void ServerConfig::GetJsonValue(const char* key_name, int32_t& val)
{
	js_.GetValue(key_name, val);
}

void ServerConfig::GetJsonValue(const char* key_name, std::string& val)
{
	js_.GetValue(key_name, val);
}

void ServerConfig::GetJsonObjectValue(const char* obj_name, const char* key_name, int32_t& val)
{
	js_.GetObjectValue(obj_name, key_name, val);
}

void ServerConfig::GetJsonObjectValue(const char* obj_name, const char* key_name, std::string& val)
{
	js_.GetObjectValue(obj_name, key_name, val);
}

//std::transform(val_name.begin(), val_name.end(), val_name.begin(), ::tolower);

void ServerConfig::LoadConfigFromJson(const std::string& path)
{
	js_.LoadFile(path);
}