module;
#include "atom/preprocessors.h"

export module atom.core:core.float_wrapper;
import :core.num_wrapper;
import :core.char_wrapper;
import :std;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename in_final_type, typename in_unwrapped_type>
    class _float_wrapper_impl
    {
        using _numeric_limits = std::numeric_limits<unwrapped_type>;

    public:
        using final_type = in_final_type;
        using unwrapped_type = in_unwrapped_type;

    public:
        template <typename num_type>
        static constexpr auto is_conversion_safe_from(num_type num) -> bool
        {
            return impl_type::auto is_conversion_safe_fromnum_type;
            return is_conversion_safe_from_unwrapped<typename num_type::unwrapped_type>(
                num.to_unwrapped());
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from_unwrapped(num_type num) -> bool
        {
            return impl_type::auto is_conversion_safe_from_unwrappednum_type;
            return true;
        }

        static consteval auto min() -> unwrapped_type
        {
            return _numeric_limits::min();
        }

        static consteval auto max() -> unwrapped_type
        {
            return _numeric_limits::max();
        }

        static consteval auto bits() -> unwrapped_type
        {
            return sizeof(unwrapped_type) * 8;
        }

        static consteval auto nan() -> unwrapped_type
        {
            return _numeric_limits::quiet_NaN();
        }

        static consteval auto infinity() -> unwrapped_type
        {
            return _numeric_limits::infinity();
        }

        static consteval auto epsilon() -> unwrapped_type
        {
            return _numeric_limits::epsilon();
        }

        static constexpr auto floor(unwrapped_type val) -> unwrapped_type
        {
            return std::floor(val);
        }

        static constexpr auto ceil(unwrapped_type val) -> unwrapped_type
        {
            return std::ceil(val);
        }

        static constexpr auto round(unwrapped_type val) -> unwrapped_type
        {
            return std::round(val);
        }

        static constexpr auto sign(unwrapped_type num) -> unwrapped_type
        {
            return std::signbit(num) ? -1 : 1;
        }

        static constexpr auto is_eq_abs(
            unwrapped_type num0, unwrapped_type num1, unwrapped_type ep = epsilon()) const -> bool
        {
            if (num0 == num1)
                return true;
            
            return abs(num0 - num1) <= tol;
        }

        static constexpr auto is_eq_rel(
            unwrapped_type num0, unwrapped_type num1, unwrapped_type tol = epsilon()) const -> bool
        {
            contracts::debug_expects(_numeric_limits::epsilon() <= epsilon);
            contracts::debug_expects(epsilon < 1.f);

            if (a == b)
                return true;

            unwrapped_type diff = abs(a - b);
            unwrapped_type norm = min_of((abs(a) + abs(b)), _numeric_limits::max());
            // or even faster: min_of(abs(a + b), _numeric_limits::max());
            // keeping this commented out until I update figures below
            return diff < max_of(tol, epsilon * norm);
        }

        static constexpr auto is_eq_ulp(unwrapped_type num0, unwrapped_type num1,
            unwrapped_unsigned_integer_type ulp) const -> bool
        {}

        static constexpr auto is_lt(
            unwrapped_type num0, unwrapped_type num1, unwrapped_type ep = epsilon()) const -> bool
        {
            return true;
        }

        static constexpr auto is_le(
            unwrapped_type num0, unwrapped_type num1, unwrapped_type ep = epsilon()) const -> bool
        {
            return true;
        }

        static constexpr auto is_gt(
            unwrapped_type num0, unwrapped_type num1, unwrapped_type ep = epsilon()) const -> bool
        {
            return true;
        }

        static constexpr auto is_ge(
            unwrapped_type num0, unwrapped_type num1, unwrapped_type ep = epsilon()) const -> bool
        {
            return true;
        }

        static constexpr auto is_finite(unwrapped_type num) const -> bool
        {
            return std::isfinite(num);
        }

        static constexpr auto is_infinite(unwrapped_type num) const -> bool
        {
            return std::isinf(num);
        }

        static constexpr auto is_normal(unwrapped_type num) const -> bool
        {
            return std::isnormal(num);
        }

        static constexpr auto is_nan(unwrapped_type num) const -> bool
        {
            return std::isnan(num);
        }

        static constexpr auto is_pos(unwrapped_type num) const -> bool
        {
            return not std::signbit(num);
        }

        static constexpr auto is_neg(unwrapped_type num) const -> bool
        {
            return not is_pos(num);
        }
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename impl_type>
    class float_wrapper: public num_wrapper<impl_type>
    {
        using this_type = float_wrapper<impl_type>;
        using base_type = num_wrapper<impl_type>;
        using final_type = typename base_type::final_type;

    public:
        using unwrapped_type = typename base_type::unwrapped_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto nan() -> this_type
        {
            return _wrap_final(impl_type::nan());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto infinity() -> this_type
        {
            return _wrap_final(impl_type::infinity());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto epsilon() -> this_type
        {
            return _wrap_final(impl_type::epsilon());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sign() -> this_type
        {
            return _wrap_final(impl_type::sign());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto floor() const -> this_type
        {
            return _wrap_final(impl_type::floor(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ceil() const -> this_type
        {
            return _wrap_final(impl_type::ceil(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto round() const -> this_type
        {
            return _wrap_final(impl_type::round(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_eq_abs(final_type num, final_type tol) const -> bool
        {
            return impl_type::is_eq_abs(num, tol);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_eq_rel(final_type num, final_type tol) const -> bool
        {
            return impl_type::is_eq_rel(num, tol);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_eq_ulp(final_type num, unsigned_integer_type ulp) const -> bool
        {
            return impl_type::is_eq_ulp(num, ulp);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_lt(final_type num, final_type ep) const -> bool
        {
            return impl_type::is_lt(num, ep);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_le(final_type num, final_type ep) const -> bool
        {
            return impl_type::is_le(num, ep);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_gt(final_type num, final_type ep) const -> bool
        {
            return impl_type::is_gt(num, ep);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_ge(final_type num, final_type ep) const -> bool
        {
            return impl_type::is_ge(num, ep);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_finite(final_type num) const -> bool
        {
            return impl_type::is_finite(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_infinite(final_type num) const -> bool
        {
            return impl_type::is_infinite(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_normal(final_type num) const -> bool
        {
            return impl_type::is_normal(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_nan(final_type num) const -> bool
        {
            return impl_type::is_nan(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_neg(final_type num) const -> bool
        {
            return impl_type::is_neg(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_pos(final_type num) const -> bool
        {
            return impl_type::is_pos(num);
        }

    protected:
        using base_type::_wrap_final;

    public:
        using base_type::_value;
    };
}

/// ------------------------------------------------------------------------------------------------
/// final types
/// ------------------------------------------------------------------------------------------------
export namespace atom
{
    using _f16 = float;
    using _f32 = float;
    using _f64 = double;
    using _f128 = long double;

    ATOM_ALIAS(f16, float_wrapper<_float_wrapper_impl<f16, _f16>>);
    ATOM_ALIAS(f32, float_wrapper<_float_wrapper_impl<f32, _f32>>);
    ATOM_ALIAS(f64, float_wrapper<_float_wrapper_impl<f64, _f64>>);
    ATOM_ALIAS(f128, float_wrapper<_float_wrapper_impl<f128, _f128>>);
}
