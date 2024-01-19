#pragma once

// #include <iostream>
// #include <type_traits>

namespace atom
{
    template <typename value_type>
    constexpr auto mov(value_type&& val) -> typename std::remove_reference_t<value_type>&&
    {
        return static_cast<typename std::remove_reference_t<value_type>&&>(val);
    }

    template <typename value_type>
    constexpr value_type&& forward(typename std::remove_reference_t<value_type>& val)
    {
        return static_cast<value_type&&>(val);
    }

    template <typename value_type>
    constexpr value_type&& forward(typename std::remove_reference_t<value_type>&& val)
    {
        static_assert(
            !std::is_lvalue_reference_v<value_type>, "can not forward an rvalue as an lvalue.");

        return static_cast<value_type&&>(val);
    }

    template <typename value_type>
    using pure_type = std::remove_cvref_t<value_type>;
}
