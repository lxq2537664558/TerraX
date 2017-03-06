#pragma once

#include <vector>
#include <map>

class Property
{
public:
	std::string Name_;
	std::string Type_;
	int Public_;
	int Private_;
	int Save_;
};

class Field
{
public:
	Field(int pub, int pri, int sv, int data_size, int offset)
	{
		pub_ = pub;
		pri_ = pri;
		sv_ = sv;
		data_size_ = data_size;
		offset_ = offset;
	}

	template<typename T>
	T GetValue(char* pDataBuffer) {
		assert(sizeof(T) == data_size_);
		return *((T*)(pDataBuffer + offset_));
	}
	char* GetValueString(char* pDataBuffer) {
		return pDataBuffer + offset_;
	}


	template<typename T>
	void SetValue(T value, char* pDataBuffer)
	{
		assert(sizeof(T) == data_size_);
		*((T*)(pDataBuffer + offset_)) = value;
	}
	void SetValueString(const char* pValue, char* pDataBuffer)
	{
		int len = strlen(pValue);
		if (len > data_size_) {
			len = data_size_;
		}
		memcpy(pDataBuffer, pValue, len);
	}
private:
	int pub_{ 0 };
	int pri_{ 0 };
	int sv_{ 0 };
	int offset_{ 0 };
	int data_size_{ 0 };
};

class Column
{
public:
	void PushProperty(const char* field_name, const char* type_name, int pub, int pri, int sv)
	{
		m_mapFieldName2Index[field_name] = m_vecFields.size();
		int nDataSize = 0;
		if (strcmp(type_name, "char16") == 0) {
			nDataSize = 16;
		} else if (strcmp(type_name, "int32") == 0) {
			nDataSize = 4;
		} else
		{ 
		}
		m_vecFields.emplace_back(Field(pub, pri, sv, nDataSize, m_datasize));
		m_datasize += nDataSize;
	}
	void AllocBuffer() { m_pDataBuffer = new char[m_datasize] {0}; }

	
	template<typename T>
	T GetValue(int index) {
		return m_vecFields[index].GetValue<T>(m_pDataBuffer);
	}
	template<typename T>
	void SetValue(int index, T val) {
		m_vecFields[index].SetValue<T>(val, m_pDataBuffer);
	}

	char* GetValueString(int index) {
		return m_vecFields[index].GetValueString(m_pDataBuffer);
	}
	void SetValueString(int index, const char* p) {
		m_vecFields[index].SetValueString(p, m_pDataBuffer);
	}

private:
	int m_datasize{ 0 };
	char* m_pDataBuffer{ nullptr };
	std::map<std::string, int> m_mapFieldName2Index;
	std::vector<Field> m_vecFields;
};