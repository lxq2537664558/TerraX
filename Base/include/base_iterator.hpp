#pragma once

namespace TerraX
{
	template<class Container_T>
	class BaseIterator
	{
	public:
		BaseIterator() = default;
		virtual ~BaseIterator() = default;

		virtual Container_T* Begin() = 0;
		virtual Container_T* Next() = 0;
		virtual Container_T* Current() = 0;
		virtual bool IsEnd() = 0;
	};

	template<class Container_T>
	class BaseAggregate
	{
	public:
		virtual int Count() = 0;
		virtual void Push(Container_T&& T) = 0;
		virtual Container_T* Pop(const int nIndex) = 0;
		virtual BaseIterator<Container_T> Iterator() = 0;
	};
}