#pragma once
#include <functional>
#include <memory>

namespace TerraX
{
    class events_dynamic
    {
    private:
        // have a placeholder base-type:
        struct base {
            virtual ~base(){};
        };
        // have a derived type to wrap the
        //  actual functor object:
        template <typename... Args>
        struct wrapped : base {
            typedef std::function<void(Args...)> func_type;
            func_type f;
            wrapped(func_type aFunc) : f(aFunc){};
        };
        // Hold a base-type pointer:
        std::unique_ptr<base> p_base;

    public:
        // Initialize object with any function:
        template <typename... Args>
        events_dynamic(std::function<void(Args...)> aFunc) : p_base(new wrapped<Args...>(aFunc)){};
        // Assign object with any function:
        template <typename... Args>
        events_dynamic& operator=(std::function<void(Args...)> aFunc)
		{
			p_base.reset(new wrapped<Args...>(aFunc));
            return *this;
        };
        // Call the function by casting the base
        //  pointer back to its derived type:
        template <typename... Args>
        void operator()(Args... args) const
        {
            // boost::polymorphic_downcast
            wrapped<Args...>* p_wrapped = dynamic_cast<wrapped<Args...>*>(p_base.get());
            if (p_wrapped)  // <- if cast successful
                p_wrapped->f(args...);
            else
                throw std::runtime_error("Invalid arguments to function object call!");
        };
    };
}
