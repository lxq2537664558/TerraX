#pragma once

#include <initializer_list>
#include <unordered_map>
#include <tuple>

namespace terra
{
	template<typename Col, typename Key, typename... Args>
	class Table
	{
	public:
		std::vector<Col> cols_;
		std::unordered_map<Key, std::tuple<Args...>> rows_;
		Table(std::initializer_list<Col> l)
		{
			cols_.insert(cols_.end(), l.begin(), l.end());
		}
		~Table() = default;

		void Insert(Key&& k, Args&&... args)
		{
			rows_.emplace(std::make_pair(k, std::tuple<Args...>(args...)));
			assert(cols_.size() == rows_.size());
		}

		auto Find(const Key& k, const Col& col)
		{
			auto iter = rows_.find(k);
			if ( iter != rows_.end())
			{
				for (int i = 0; i < cols_.size(); i++)
				{
					if (col == cols_[i])
					{
						return std::get<i>(iter->second);
					}
				}
			}
			return nullptr;
		}
	};
}