#pragma once
#include <functional>
#include <memory>
#include <type_traits>

namespace TerraX
{
    class events_dynamic2
    {
    private:
        struct base {
            virtual ~base(){};
            virtual std::size_t args_size() { return 0; }
            virtual std::size_t args_hashcode() { return 0; }
        };

        template <typename... Args>
        struct wrapped : base {
            typedef std::function<void(Args...)> func_type;
            func_type f;
            std::tuple<Args...> t;

            wrapped(func_type&& aFunc) : f(aFunc){};
            std::size_t args_hashcode() override final { return typeid(t).hash_code(); }
            std::size_t args_size() override final { return std::tuple_size<decltype(t)>::value; }
        };

        std::unique_ptr<base> p_base;

    public:
        template <typename... Args>
        events_dynamic2(std::function<void(Args...)>&& aFunc)
            : p_base(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc))){};

        template <typename... Args>
        events_dynamic2& operator=(std::function<void(Args...)>&& aFunc)
        {
            p_base.reset(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc)));
            return *this;
        };
        // Call the function by casting the base
        //  pointer back to its derived type:
        template <typename... Args>
        void operator()(Args&&... args) const
        {
            // boost::polymorphic_downcast
            // arguments are not same becauseof (typeid(pkt*) != typeid(message*));
            // assert(p_base->args_hashcode() == typeid(std::tuple<Args...>).hash_code());

            wrapped<Args...>* p_wrapped = static_cast<wrapped<Args...>*>(p_base.get());
            if (p_wrapped) {
                p_wrapped->f(std::forward<Args>(args)...);
            }
        };

        std::size_t args_size() { return p_base->args_size(); }
    };
}
