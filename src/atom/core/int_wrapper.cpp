module;
#include "atom/preprocessors.h"

export module atom.core:core.int_wrapper;
import :core.char_wrapper;
import :core.num_wrapper;
import :core.byte;
import :contracts_decl;
import :std;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// common implementation for signed and unsigned integers.
    /// --------------------------------------------------------------------------------------------
    template <typename in_final_type, typename in_signed_type, typename in_unsigned_type,
        typename in_unwrapped_type, typename limit_type>
    class _int_wrapper_impl
    {
    public:
        using final_type = in_final_type;
        using unwrapped_type = in_unwrapped_type;
        using signed_type = in_signed_type;
        using unsigned_type = in_unsigned_type;

    public:
        static consteval auto min() -> unwrapped_type
        {
            return unwrapped_type(std::numeric_limits<limit_type>::min());
        }

        static consteval auto max() -> unwrapped_type
        {
            return unwrapped_type(std::numeric_limits<limit_type>::max());
        }

        static consteval auto bits() -> unwrapped_type
        {
            return unwrapped_type(sizeof(limit_type) * byte_get_bit_count());
        }

        static consteval auto is_integer() -> bool
        {
            return std::is_integral_v<unwrapped_type>;
        }

        static consteval auto is_float() -> bool
        {
            return std::is_floating_point_v<unwrapped_type>;
        }

        static consteval auto is_signed() -> bool
        {
            return std::is_signed_v<unwrapped_type>;
        }

        static constexpr auto count_digits(unwrapped_type num) -> unwrapped_type
        {
            if (num == 0)
                return 1;

            if constexpr (is_signed())
                return std::log10(std::abs(num)) + 1;
            else
                return std::log10(num) + 1;
        }

        static constexpr auto is_add_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            if constexpr (not is_signed())
            {
                return rhs <= max() - lhs;
            }
            else
            {
                if (rhs > 0 && lhs > max() - rhs)
                    return false;

                if (rhs < 0 && lhs < min() - rhs)
                    return false;

                return true;
            }
        }

        static constexpr auto is_sub_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            if constexpr (not is_signed())
            {
                return rhs <= lhs - min();
            }
            else
            {
                return is_add_safe(lhs, -rhs);
            }
        }

        static constexpr auto is_mul_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            if constexpr (not is_signed())
            {
                if (rhs > 0 && lhs > max() / rhs)
                    return false;

                return true;
            }
            else
            {
                if (rhs > 0 && lhs > max() / rhs)
                    return false;

                if (rhs < 0 && lhs < min() / rhs)
                    return false;

                return true;
            }
        }

        static constexpr auto is_div_safe(unwrapped_type num, unwrapped_type den) -> bool
        {
            if constexpr (is_signed())
            {
                if (den == 0)
                    return false;

                if (num == min() && den == -1)
                    return false;
            }

            return true;
        }

        static constexpr auto is_abs_safe(unwrapped_type num) -> bool
        {
            if constexpr (is_signed())
                return num != min();

            return true;
        }

        static constexpr auto is_neg_safe(unwrapped_type num) -> bool
        {
            if constexpr (is_signed())
                return num != min();

            return true;
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from(num_type num) -> bool
        {
            return is_conversion_safe_from_unwrapped<typename num_type::unwrapped_type>(
                num.to_unwrapped());
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from_unwrapped(num_type num) -> bool
        {
            if constexpr (is_signed())
            {
                if constexpr (std::is_signed_v<num_type>
                              and std::numeric_limits<num_type>::min() < min())
                {
                    if (num < min())
                    {
                        return false;
                    }
                }

                if constexpr (std::numeric_limits<num_type>::max() > max())
                {
                    if (num > max())
                    {
                        return false;
                    }
                }
            }
            else
            {
                if constexpr (std::is_signed_v<num_type>)
                    if (num < 0)
                        return false;

                if constexpr (std::numeric_limits<num_type>::max() > max())
                    if (num > max())
                        return false;
            }

            return true;
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_to_unwrapped(unwrapped_type num) -> bool
        {
            if constexpr (is_signed())
            {
                if constexpr (min() < std::numeric_limits<num_type>::min())
                    if (num < std::numeric_limits<num_type>::min())
                        return false;

                if constexpr (max() > std::numeric_limits<num_type>::max())
                    if (num > std::numeric_limits<num_type>::max())
                        return false;
            }
            else
            {
                if constexpr (max() > std::numeric_limits<num_type>::max())
                {
                    if (num > std::numeric_limits<num_type>::max())
                        return false;
                }
            }

            return true;
        }

        static constexpr auto abs(unwrapped_type num) -> unwrapped_type
        {
            return std::abs(num);
        }

        static constexpr auto neg(unwrapped_type num) -> unwrapped_type
        {
            return -num;
        }

        static constexpr auto sign(unwrapped_type num) -> unwrapped_type
        {
            return num < 0 ? -1 : 1;
        }
    };
}

/// ------------------------------------------------------------------------------------------------
/// final types
/// ------------------------------------------------------------------------------------------------
export namespace atom
{
    using _i8 = std::int8_t;
    using _i16 = std::int16_t;
    using _i32 = std::int32_t;
    using _i64 = std::int64_t;
    using _ifast8 = std::int_fast8_t;
    using _ifast16 = std::int_fast16_t;
    using _ifast32 = std::int_fast32_t;
    using _ifast64 = std::int_fast64_t;
    using _imax = std::intmax_t;
    using _isize = std::ptrdiff_t;

    using _u8 = std::uint8_t;
    using _u16 = std::uint16_t;
    using _u32 = std::uint32_t;
    using _u64 = std::uint64_t;
    using _ufast8 = std::uint_fast8_t;
    using _ufast16 = std::uint_fast16_t;
    using _ufast32 = std::uint_fast32_t;
    using _ufast64 = std::uint_fast64_t;
    using _umax = std::uintmax_t;
    using _usize = std::size_t;

    class i8;
    class i16;
    class i32;
    class i64;
    class ifast8;
    class ifast16;
    class ifast32;
    class ifast64;
    class imax;
    class isize;

    class u8;
    class u16;
    class u32;
    class u64;
    class ufast8;
    class ufast16;
    class ufast32;
    class ufast64;
    class umax;
    class usize;

    ATOM_ALIAS(i8, num_wrapper<_int_wrapper_impl<i8, i8, u8, _i8, _i8>>);
    ATOM_ALIAS(i16, num_wrapper<_int_wrapper_impl<i16, i16, u16, _i16, _i16>>);
    ATOM_ALIAS(i32, num_wrapper<_int_wrapper_impl<i32, i32, u32, _i32, _i32>>);
    ATOM_ALIAS(i64, num_wrapper<_int_wrapper_impl<i64, i64, u64, _i64, _i64>>);
    ATOM_ALIAS(ifast8, num_wrapper<_int_wrapper_impl<ifast8, ifast8, ufast8, _ifast8, _i8>>);
    ATOM_ALIAS(ifast16, num_wrapper<_int_wrapper_impl<ifast16, ifast16, ufast16, _ifast16, _i16>>);
    ATOM_ALIAS(ifast32, num_wrapper<_int_wrapper_impl<ifast32, ifast32, ufast32, _ifast32, _i32>>);
    ATOM_ALIAS(ifast64, num_wrapper<_int_wrapper_impl<ifast64, ifast64, ufast64, _ifast64, _i64>>);
    ATOM_ALIAS(imax, num_wrapper<_int_wrapper_impl<imax, imax, umax, _imax, _imax>>);
    ATOM_ALIAS(isize, num_wrapper<_int_wrapper_impl<isize, isize, usize, _isize, _isize>>);

    ATOM_ALIAS(u8, num_wrapper<_int_wrapper_impl<u8, i8, u8, _u8, _u8>>);
    ATOM_ALIAS(u16, num_wrapper<_int_wrapper_impl<u16, i16, u16, _u16, _u16>>);
    ATOM_ALIAS(u32, num_wrapper<_int_wrapper_impl<u32, i32, u32, _u32, _u32>>);
    ATOM_ALIAS(u64, num_wrapper<_int_wrapper_impl<u64, i64, u64, _u64, _u64>>);
    ATOM_ALIAS(ufast8, num_wrapper<_int_wrapper_impl<ufast8, ifast8, ufast8, _ufast8, _u8>>);
    ATOM_ALIAS(ufast16, num_wrapper<_int_wrapper_impl<ufast16, ifast16, ufast16, _ufast16, _u16>>);
    ATOM_ALIAS(ufast32, num_wrapper<_int_wrapper_impl<ufast32, ifast32, ufast32, _ufast32, _u32>>);
    ATOM_ALIAS(ufast64, num_wrapper<_int_wrapper_impl<ufast64, ifast64, ufast64, _ufast64, _u64>>);
    ATOM_ALIAS(umax, num_wrapper<_int_wrapper_impl<umax, imax, umax, _umax, _umax>>);
    ATOM_ALIAS(usize, num_wrapper<_int_wrapper_impl<usize, isize, usize, _usize, _usize>>);
}
