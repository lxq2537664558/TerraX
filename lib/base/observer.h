#pragma once

#include <set>
#include "macro.h"
namespace terra
{
    template <typename... Args>
    class IObserver
    {
    protected:
        IObserver() = default;

    public:
        virtual ~IObserver() {}

    public:
        virtual void OnUpdate(Args... args) = 0;
    };

    template <typename... Args>
    class ISubject
    {
    private:
        std::set<IObserver<Args...>*> observers_;

    protected:
        ISubject() = default;

    public:
        virtual ~ISubject() {}

    public:
        void AddObserver(IObserver<Args...>* pObserver) { observers_.insert(pObserver); }
        void RemoveObserver(IObserver<Args...>* pObserver) { observers_.erase(pObserver); }

        void Notify(Args... args)
        {
            for (const auto& pObserver : observers_) {
                pObserver->OnUpdate(args...);
            }
        }
    };
}