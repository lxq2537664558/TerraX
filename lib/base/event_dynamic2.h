#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace terra
{
    class event_dynamic2
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
            // std::tuple<typename std::decay<Args>::type...> t;
            // std::tuple<typename std::decay_t<Args>...> t; //c++14
            std::tuple<Args...> t;
            wrapped(func_type&& aFunc) : f(aFunc){};
            std::size_t args_hashcode() override final { return typeid(t).hash_code(); }
            std::size_t args_size() override final { return std::tuple_size<decltype(t)>::value; }
        };

        std::unique_ptr<base> base_;

    public:
        template <typename... Args>
        event_dynamic2(std::function<void(Args...)>&& aFunc)
            : base_(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc))){};

        template <typename... Args>
        event_dynamic2& operator=(std::function<void(Args...)>&& aFunc)
        {
            base_.reset(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc)));
            return *this;
        };
        // Call the function by casting the base
        //  pointer back to its derived type:
        //     template <typename... Args>
        //     void operator()(Args&&... args) const
        //     {
        //         // boost::polymorphic_downcast
        //         // arguments are not same becauseof (typeid(pkt*) != typeid(message*));
        //         // assert(p_base->args_hashcode() == typeid(std::tuple<Args...>).hash_code());
        ////typename decay<Args>::type...
        //         wrapped<Args...>* p_wrapped = static_cast<wrapped<Args...>*>(p_base.get());
        //         if (p_wrapped) {
        //             p_wrapped->f(std::forward<Args>(args)...);
        //         }
        //     };
        template <typename... Args>
        void operator()(Args... args) const
        {
            // boost::polymorphic_downcast
            // arguments are not same becauseof (typeid(pkt*) != typeid(message*));
            // assert(p_base->args_hashcode() == typeid(std::tuple<Args...>).hash_code());
            // typename decay<Args>::type...
            wrapped<Args...>* p_wrapped = static_cast<wrapped<Args...>*>(base_.get());
            if (p_wrapped) {
                p_wrapped->f(args...);
            }
        };

        std::size_t args_size() { return base_->args_size(); }
    };
}
