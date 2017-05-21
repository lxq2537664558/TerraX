#include "jsonstream.h"
#include <sys/stat.h> 

using namespace terra;

void JsonStream::GetValue(const char* key_name, int32_t& val)
{
	assert(doc_.HasMember(key_name));
	assert(doc_[key_name].IsInt());
	val = doc_[key_name].GetInt();
}

void JsonStream::GetValue(const char* key_name, std::string& val)
{
	assert(doc_.HasMember(key_name));
	assert(doc_[key_name].IsString());
	val = doc_[key_name].GetString();
}

void JsonStream::GetObjectValue(const char* obj_name, const char* key_name, int32_t& val)
{
	assert(doc_.HasMember(obj_name));
	assert(doc_[obj_name].IsObject());
	const auto& obj = doc_[obj_name].GetObject();
	assert(obj.HasMember(key_name));
	assert(obj[key_name].IsInt());
	val = obj[key_name].GetInt();
}

void JsonStream::GetObjectValue(const char* obj_name, const char* key_name, std::string& val)
{
	assert(doc_.HasMember(obj_name));
	assert(doc_[obj_name].IsObject());
	const auto& obj = doc_[obj_name].GetObject();
	assert(obj.HasMember(key_name));
	assert(obj[key_name].IsString());
	val = obj[key_name].GetString();
}

//std::transform(val_name.begin(), val_name.end(), val_name.begin(), ::tolower);

void JsonStream::LoadFile(const std::string& path)
{
	FILE* fp = fopen(path.c_str(), "r"); // "rb", 非 Windows 平台使用 "r"
	assert(fp);
	int descriptor = fileno(fp);
	struct stat statBuf;
	if (fstat(descriptor, &statBuf) == -1) {
		fclose(fp);
		return;
	}
	std::size_t file_length = statBuf.st_size;
	char* buffer = new char[file_length];
	std::size_t readsize = fread(buffer, 1, file_length, fp);
	assert(readsize == file_length);
	fclose(fp);
	doc_.Parse(buffer);
	delete[] buffer;
}