#pragma once

#include <cassert>

namespace TerraX
{

	template <class T>
	class Enumerator_T
	{
	public:
		Enumerator_T() = default;
		virtual Enumerator_T() = default;

		virtual int Size() = 0;
		virtual void Push(T* pVal) = 0;
		virtual T* At(int nIndex) = 0;
		virtual void RemoveAt(int nIndex) = 0;
		virtual void Remove(T* pVal) = 0;

		virtual Iterator_T<T> Iterator() = 0;
	};

	template <class T>
	class Iterator_T final
	{
	private:
		Enumerator_T<T>* m_pAggregate{ nullptr };
		int m_nCurIndex{ 0 };

	public:
		Iterator_T(Enumerator_T<T>* pAggregate) : m_pAggregate(pAggregate) {}
		~Iterator_T() = default;

		//T* First() { return (m_pAggregate->Count() > 0) ? m_pAggregate->At(0) : nullptr; }
		T* Next()
		{
			return (m_nCurIndex < m_pAggregate->Count()) ? m_pAggregate->At(m_nCurIndex++) : nullptr;
		}
		T* Current()
		{
			return (m_nCurIndex < m_pAggregate->Count() && m_nCurIndex >= 0) ? m_pAggregate->At(m_nCurIndex) : nullptr;
		}
		bool IsEnd() { return m_nCurIndex >= m_pAggregate->Count(); }
	};

	template <class T, int CONTAINER_SIZE = 1>
	class Container_T : public Enumerator_T<T>
	{
	private:
		std::array<std::unique_ptr<T>, CONTAINER_SIZE> m_pItems;
		int m_nCurSize{ 0 };

	public:
		Container_T() {}
		int Size() override { return m_nCurSize; }
		void Push(T* pVal) override
		{
			assert(m_pItems[m_nCurSize] == nullptr);
			m_pItems[m_nCurSize++].reset(pVal);
		}
		T* At(int nIndex) override { return m_pItems[nIndex].get(); }
		Iterator_T<T> Iterator() override { return Iterator_T<T>(this); }

		void RemoveAt(int nIndex) override
		{
			if (nIndex >= 0 && nIndex < m_nCurSize) {
				if (nIndex == m_nCurSize) {
					m_pItems[nIndex].reset();
				}
				else {
					m_pItems[nIndex] = std::move(m_pItems[m_nCurSize - 1]);
				}
				--m_nCurSize;
			}
		}

		void Remove(T* pItem) override
		{
			auto iter = Iterator();
			int nIndex = 0;
			while (!iter.IsEnd()) {
				T* pVal = iter.Next();
				if (pVal == pItem) {
					RemoveAt(nIndex);
					return;
				}
				++nIndex;
			}
		}
	};
}