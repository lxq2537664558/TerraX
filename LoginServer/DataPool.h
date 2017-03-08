#pragma once

#include <vector>
#include <map>
#include <memory>

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
	int GetDataSize() const { return m_datasize; }
	Field& GetField(int nIndex) { 
		assert(nIndex >= 0 && nIndex < static_cast<int>(m_vecFields.size()));
		return m_vecFields[nIndex];
	}
	Field& GetField(const char* field_name) {
		auto& iter = m_mapFieldName2Index.find(field_name);
		assert(iter != m_mapFieldName2Index.end());
		return GetField(iter->second);
	}

private:
	int m_datasize{ 0 };
	std::map<std::string, int> m_mapFieldName2Index;
	std::vector<Field> m_vecFields;
};

class Row
{
private:
	Column* pColumn_{ nullptr };
	char* pDataBuffer_{ nullptr };
public:
	Row(Column* pCol) {
		assert(pCol != nullptr);
		SetColumn(pCol);
		AllocBuffer(pCol->GetDataSize());
	}
	~Row() { delete[] pDataBuffer_; }

	//void RegValueChangeCB()

	template<typename T>
	T GetValue(int index) {
		Field& field = pColumn_->GetField(index);
		return field.GetValue<T>(pDataBuffer_);
	}
	template<typename T>
	T GetValue(const char* field_name) {
		Field& field = pColumn_->GetField(field_name);
		return field.GetValue<T>(pDataBuffer_);
	}

	template<typename T>
	void SetValue(int index, T val) {
		Field& field = pColumn_->GetField(index);
		field.SetValue<T>(val, pDataBuffer_);
	}
	template<typename T>
	void SetValue(const char* field_name, T val) {
		Field& field = pColumn_->GetField(field_name);
		field.SetValue<T>(val, pDataBuffer_);
	}

	const char* GetValueString(int index) {
		Field& field = pColumn_->GetField(index);
		return field.GetValueString(pDataBuffer_);
	}
	const char* GetValueString(const char* field_name) {
		Field& field = pColumn_->GetField(field_name);
		return field.GetValueString(pDataBuffer_);
	}

	void SetValueString(int index, const char* p) {
		Field& field = pColumn_->GetField(index);
		field.SetValueString(p, pDataBuffer_);
	}
	void SetValueString(const char* field_name, const char* p) {
		Field& field = pColumn_->GetField(field_name);
		field.SetValueString(p, pDataBuffer_);
	}
private:
	void SetColumn(Column* pCol) { pColumn_ = pCol; }
	void AllocBuffer(int buffer_size) {
		pDataBuffer_ = new char[buffer_size] {0};
	}
};

class DataTable
{
private:
	Column col_;
public:
	Column& GetColumn() { return col_; }

	Row* NewRow() {
		return new Row(&col_);
	}
};