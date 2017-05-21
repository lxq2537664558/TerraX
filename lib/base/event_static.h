#pragma once

#include <functional>
#include <memory>
#include <tuple>

namespace terra
{
    namespace detail
    {
        template <typename F, typename Tuple, bool Done, int Total, int... N>
        struct call_impl {
            static void call(F f, Tuple&& t)
            {
                call_impl<F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(
                    f, std::forward<Tuple>(t));
            }
        };

        template <typename F, typename Tuple, int Total, int... N>
        struct call_impl<F, Tuple, true, Total, N...> {
            static void call(F f, Tuple&& t) { f(std::get<N>(std::forward<Tuple>(t))...); }
        };
    }

    // user invokes this
    template <typename F, typename Tuple>
    void call(F f, Tuple&& t)
    {
        typedef typename std::decay<Tuple>::type ttype;
        detail::call_impl<F, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(
            f, std::forward<Tuple>(t));
    }

    /* //c++14
    template <class F, class Tuple, std::size_t... I>
    constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
    {
    return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
    }


    template <class F, class Tuple>
    constexpr decltype(auto) apply(F&& f, Tuple&& t)
    {
    return apply_impl(
    std::forward<F>(f), std::forward<Tuple>(t),
    std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
    }
    */

    class event_static
    {
    private:
        struct base {
            virtual ~base(){};
            virtual void call_func() {}
        };
        template <typename... Args>
        struct wrapped : base {
            typedef std::function<void(Args...)> func_type;
            func_type f;
            std::tuple<Args...> t;
            wrapped(func_type&& aFunc, Args&&... params) : f(aFunc), t(params...){};

            void call_func() override final
            {
                // apply(f, t);
                call(f, t);
            }
        };
        std::unique_ptr<base> base_;

    public:
        template <typename... Args>
        event_static(std::function<void(Args...)>&& aFunc, Args&&... params)
            : base_(new wrapped<Args...>(std::forward<std::function<void(Args...)>>(aFunc),
                                          std::forward<Args>(params)...))
        {
        }

        void operator()() const { base_->call_func(); }
    };
}