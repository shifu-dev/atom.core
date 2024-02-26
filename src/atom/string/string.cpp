export module atom.core:string;
import :std;
import :fmt;
import :core;
import :buf_string;
import :output_requirements;
import :string_view;
import :string_formatting;
import :memory.default_mem_allocator;

namespace atom
{
    export class string: public buf_string<40, default_mem_allocator>
    {
        using base_type = buf_string<40, default_mem_allocator>;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename output_type, typename... arg_types>
        static constexpr auto format_to(
            output_type&& out, format_string<arg_types...> fmt, arg_types&&... args)
            requires routput<output_type, uchar>
        {
            _format_to(out, fmt, forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        static constexpr auto format(format_string<arg_types...> fmt, arg_types&&... args) -> string
        {
            string out;
            format_to(out, fmt, forward<arg_types>(args)...);

            return out;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `string`.
    /// --------------------------------------------------------------------------------------------
    template <>
    struct string_formatter<string>: public string_formatter<string_view>
    {};
}

namespace fmt
{
    template <>
    class formatter<atom::string>: public _formatter_helper<atom::string>
    {};
}

namespace std
{
    template <>
    class hash<atom::string>
    {
    public:
        constexpr auto operator()(const atom::string& str) const -> std::size_t
        {
            const char* begin = str.to_std_char_ptr();
            atom::_usize count = str.get_count().to_unwrapped();
            return hash<std::string_view>()(std::string_view(begin, count));
        }
    };
}
