#pragma once
#include "_enums_impl.h"

namespace atom::enums
{
    /// --------------------------------------------------------------------------------------------
    /// returns true if enum type is flags.
    ///
    /// @note this always returns false, specialize this function for to return `true` for flag
    /// enums.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr bool is_flags = false;

    /// --------------------------------------------------------------------------------------------
    /// returns the underlying data type used for storage of enum, if the type is not enum,
    /// compiler error occurs.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    using get_underlying_t = _enums_impl<enum_t, is_flags<enum_t>>::underlying_t;

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if `enum_t` is an enum.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr bool is_enum = _enums_impl<enum_t, is_flags<enum_t>>::is_enum();

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if enum is a scoped enum.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr bool is_scoped = _enums_impl<enum_t, is_flags<enum_t>>::is_scoped();

    /// --------------------------------------------------------------------------------------------
    /// returns first enum value of type `enum_t` matching string `str`.
    ///
    /// @example value from string.
    /// ```
    /// enum class example
    /// {
    ///     value1,
    ///     value2,
    ///     value3,
    /// };
    ///
    /// example value = enums::from_string<example>("value1");
    /// ```
    ///
    /// @example value from flags.
    /// ```
    /// enum class flags
    /// {
    ///     value1 = 1 << 0,
    ///     value2 = 1 << 1,
    ///     value3 = 1 << 2,
    /// };
    ///
    /// flags value = enums::from_string<flags>("value1|value2");
    /// ```
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto from_string(string_view str) -> option<enum_t>
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::from_string(str);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns first enum value of type `enum_t` matching string `str`. same as above, except
    /// `comparer` is used to match the string.
    ///
    /// @param comparer used to match string `str` with enum values.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t, typename comparer_t>
    constexpr auto from_string(string_view str, comparer_t&& comparer) -> option<enum_t>
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::from_string(str, ATOM_FORWARD(comparer));
    }

    /// --------------------------------------------------------------------------------------------
    /// returns enum value of type `enum_t` created from underlying type.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto from_underlying_unchecked(get_underlying_t<enum_t> value) -> enum_t
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::from_underlying_unchecked(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns enum value of type `enum_t` created from underlying type.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto from_underlying_try(get_underlying_t<enum_t> value) -> option<enum_t>
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::from_underlying_try(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns enum value at index `index`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto from_index_unchecked(usize index) -> enum_t
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::from_index_unchecked(index);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns enum value at index `index`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto from_index_try(usize index) -> option<enum_t>
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::from_index_try(index);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the name of the `enum_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    consteval auto get_type_name() -> string_view
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_type_name();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the count of enum values present in `enum_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    consteval auto get_count() -> usize
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_count();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns an `array_view` of values.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    consteval auto get_values() -> array_view<enum_t>
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_values();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns an `array_view` of string representation of enum values.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    consteval auto get_strings() -> array_view<string_view>
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_strings();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns an `array_view` of pairs of enum values and their string representations.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    consteval auto get_entries() -> array_view<tuple<enum_t, string_view>>
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_entries();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if value `value` exists.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto is_value_valid(enum_t value) -> bool
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::is_value_valid(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if a value at index `index` exists.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto is_index_valid(usize index) -> bool
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::is_index_valid(index);
    }

    /// --------------------------------------------------------------------------------------------
    ///  returns `true` if a value whose underlying representation is `value` exists.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto is_underlying_valid(get_underlying_t<enum_t> value) -> bool
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::is_underlying_valid(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the index of the enum value if valid, else max.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto to_index(enum_t value) -> usize
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::to_index(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the underlying representation of enum value.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto to_underlying(enum_t value) -> get_underlying_t<enum_t>
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::to_underlying(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the null terminated string representation of enum value if valid, else empty.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto to_string_view(enum_t value) -> string_view
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::to_string_view(value);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the enum value which has the minimum underlying type value. in case of flags,
    /// returns zero.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    consteval auto get_min() -> enum_t
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_min();
    }

    /// --------------------------------------------------------------------------------------------
    /// returns the enum value which has the maximum underlying type. in case of flags, returns
    /// enum value having all flags on.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    consteval auto get_max() -> enum_t
        requires is_enum<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_max();
    }

    /// --------------------------------------------------------------------------------------------
    /// adds the rhs flags into `lhs` flags and returns the result.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto add_flags(enum_t lhs, enum_t rhs) -> enum_t
        requires is_enum<enum_t> and is_flags<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::add_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// removes the rhs flags from `lhs` flags and returns the result.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto remove_flags(enum_t lhs, enum_t rhs) -> enum_t
        requires is_enum<enum_t> and is_flags<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::remove_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns only the flags present in both `lhs` and `rhs`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto get_common_flags(enum_t lhs, enum_t rhs) -> enum_t
        requires is_enum<enum_t> and is_flags<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_common_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// adds `lhs` and `rhs` and removes the flags that are common in both.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto get_uncommon_flags(enum_t lhs, enum_t rhs) -> enum_t
        requires is_enum<enum_t> and is_flags<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_uncommon_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns all the flags not present in both `flags`.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto get_reverse_flags(enum_t flags) -> enum_t
        requires is_enum<enum_t> and is_flags<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::get_reverse_flags(flags);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if `lhs` has all the `rhs` flags.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto has_all_flags(enum_t lhs, enum_t rhs) -> bool
        requires is_enum<enum_t> and is_flags<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::has_all_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `true` if `lhs` has any of the `rhs` flags.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto has_any_flags(enum_t lhs, enum_t rhs) -> bool
        requires is_enum<enum_t> and is_flags<enum_t>
    {
        return _enums_impl<enum_t, is_flags<enum_t>>::has_any_flags(lhs, rhs);
    }
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// returns `enums::get_reverse_flags(flags)`;
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto operator~(enum_t flags) -> enum_t
        requires enums::is_enum<enum_t> and enums::is_flags<enum_t>
    {
        return enums::get_reverse_flags(flags);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `enums::add_flags(lhs, rhs)`;
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto operator|(enum_t lhs, enum_t rhs) -> enum_t
        requires enums::is_enum<enum_t> and enums::is_flags<enum_t>
    {
        return enums::add_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `lhs = lhs | rhs`;
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto operator|=(enum_t& lhs, enum_t rhs) -> enum_t&
        requires enums::is_enum<enum_t> and enums::is_flags<enum_t>
    {
        return lhs = lhs | rhs;
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `enums::get_common_flags(lhs, rhs)`;
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto operator&(enum_t lhs, enum_t rhs) -> enum_t
        requires enums::is_enum<enum_t> and enums::is_flags<enum_t>
    {
        return enums::get_common_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `lhs = lhs & rhs`;
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto operator&=(enum_t& lhs, enum_t rhs) -> enum_t&
        requires enums::is_enum<enum_t> and enums::is_flags<enum_t>
    {
        return lhs = lhs & rhs;
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `enums::get_uncommon_flags(lhs, rhs)`;
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto operator^(enum_t lhs, enum_t rhs) -> enum_t
        requires enums::is_enum<enum_t> and enums::is_flags<enum_t>
    {
        return enums::get_uncommon_flags(lhs, rhs);
    }

    /// --------------------------------------------------------------------------------------------
    /// returns `lhs = lhs ^ rhs`;
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
    constexpr auto operator^=(enum_t& lhs, enum_t rhs) -> enum_t&
        requires enums::is_enum<enum_t> and enums::is_flags<enum_t>
    {
        return lhs = lhs ^ rhs;
    }
}
