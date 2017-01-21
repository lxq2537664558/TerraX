#pragma once
#include "base_macro.h"
#include <set>
namespace TerraX
{
	//elegant observer-pattern
	template<typename... Args>
	class IObserver
	{
	protected:
		IObserver() = default;
	public:
		virtual ~IObserver(){}
	public:
		virtual void OnUpdate(Args... args) {}
	};

	template<typename... Args>
	class ISubject
	{
	protected:
		ISubject() = default;
	public:
		virtual ~ISubject() {}
	public:
		void AddObserver(IObserver<Args...>* pObserver)
		{
			m_setObservers.insert(pObserver);
		}
		void RemoveObserver(IObserver<Args...>* pObserver)
		{
			m_setObservers.erase(pObserver);
		}
		template<typename... Args>
		void Notify(Args... args)
		{
			for (auto pObserver : m_setObservers)
			{
				pObserver->OnUpdate(args...);
			}
		}
	private:
		std::set<IObserver<Args...>*> m_setObservers;
	};
}