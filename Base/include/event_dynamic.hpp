#pragma once
#include <functional>
#include <memory>

namespace TerraX
{
	//powerful wrapped-class
    class events_dynamic
    {
    private:
        struct base {
            virtual ~base(){};
        };

        template <typename... Args>
        struct wrapped : base {
            typedef std::function<void(Args...)> func_type;
            func_type f;
            wrapped(func_type&& aFunc) : f(aFunc){};
        };

        std::unique_ptr<base> p_base;

    public:
        template <typename... Args>
        events_dynamic(std::function<void(Args...)>&& aFunc)
            : p_base(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc))){};

        template <typename... Args>
        events_dynamic& operator=(std::function<void(Args...)>&& aFunc)
        {
            p_base.reset(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc)));
            return *this;
        };

		//universal forwarding will deduce int to int& and it will confuse the user.
		//maybe it is not a good idea to use std::forward here.
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
