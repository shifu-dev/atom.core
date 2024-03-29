#pragma once
#include "atom/core/string/_format_arg_wrapper.h"
#include "atom/core/string/string_view.h"
#include "atom/core/typeinfo.h"
#include "fmt/core.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// type used to bypass compile time string formatting checks.
    /// --------------------------------------------------------------------------------------------
    class runtime_format_string
    {
    public:
        constexpr runtime_format_string(string_view str)
            : str(str)
        {}

    public:
        string_view str;
    };

    /// --------------------------------------------------------------------------------------------
    /// string type used to store the format for formatting. this also checks at compile time for
    /// invalid format or args.
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_ts>
    class _format_string
    {
    public:
        template <typename string_t>
        consteval _format_string(const string_t& str)
            requires typeinfo::is_constructible_from<string_view, string_t>
            : str(str)
        {
            using fmt_format_string =
                fmt::format_string<_format_arg_wrapper<typeinfo::get_pure<arg_ts>>...>;

            fmt_format_string check(str);
        }

        constexpr _format_string(runtime_format_string str)
            : str(str.str)
        {}

    public:
        string_view str;
    };

    template <typename... arg_ts>
    using format_string = _format_string<typeinfo::identity_t<arg_ts>...>;
}
