export module atom_core:core.num_wrapper;

import std;
import :contracts;

namespace atom
{
    class _num_wrapper_id
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename num_type>
    concept _is_num = std::is_integral_v<num_type> or std::is_floating_point_v<num_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename num_type>
    concept is_num = std::derived_from<num_type, _num_wrapper_id>;

    /// --------------------------------------------------------------------------------------------
    /// wraps any numeric type to provide safe operations like overflow and underflow checks.
    /// --------------------------------------------------------------------------------------------
    template <typename in_impl_type>
    class num_wrapper: public _num_wrapper_id
    {
        using this_type = num_wrapper<in_impl_type>;

    protected:
        using impl_type = in_impl_type;
        using final_type = typename impl_type::final_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type that `this_type` wraps.
        /// ----------------------------------------------------------------------------------------
        using unwrapped_type = typename impl_type::unwrapped_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper() = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy contructor
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper(const this_type&) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper& operator=(const this_type&) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move contructor
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper(this_type&&) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper& operator=(this_type&&) = default;

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        ///
        /// converts `num_type` to `this_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        explicit constexpr num_wrapper(num_type num)
            requires is_num<num_type>
            : _value{ num._value }
        {
            contract_debug_expects(is_conversion_safe_from(num));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        ///
        /// converts `num_type` to `this_type`. this is supposed to accept literals only.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr num_wrapper(num_type num)
            requires _is_num<num_type>
            : _value{ num }
        {
            contract_debug_expects(is_conversion_safe_from_unwrapped(num));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        ///
        /// converts `num_type` to `this_type`. this is supposed to accept literals only.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator=(num_type num) -> final_type&
            requires _is_num<num_type>
        {
            contract_debug_expects(is_conversion_safe_from_unwrapped(num));

            _value = num;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~num_wrapper() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this is an integer type.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_integer() -> bool
        {
            return impl_type::is_integer();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this is an integer type.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_float() -> bool
        {
            return impl_type::is_float();
        }

        /// ----------------------------------------------------------------------------------------
        /// counts number of digits needed to represent `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_digits() const -> final_type
        {
            return _wrap_final(impl_type::count_digits(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this is a signed type.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_signed() -> bool
        {
            return impl_type::is_signed();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto nan() -> this_type
            requires(is_float())
        {
            return _wrap_final(impl_type::nan());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto floor() const -> this_type
            requires(is_float())
        {
            return _wrap_final(impl_type::floor(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ceil() const -> this_type
            requires(is_float())
        {
            return _wrap_final(impl_type::ceil(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto round() const -> this_type
            requires(is_float())
        {
            return _wrap_final(impl_type::round(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_type` from `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto from(num_type num) -> final_type
            requires is_num<num_type>
        {
            contract_debug_expects(is_conversion_safe_from(num));

            return _wrap_final(num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_type` from `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto from_checked(num_type num) -> final_type
            requires is_num<num_type>
        {
            contract_expects(is_conversion_safe_from(num));

            return _wrap_final(num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_type` from `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto from_unchecked(num_type num) -> final_type
            requires is_num<num_type>
        {
            return _wrap_final(num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when converting `num_type` to
        /// `this_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto is_conversion_safe_from(num_type num) -> bool
            requires is_num<num_type>
        {
            return impl_type::is_conversion_safe_from(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_type` from unwrapped `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto from_unwrapped(num_type num) -> final_type
            requires _is_num<num_type>
        {
            contract_debug_expects(is_conversion_safe_from_unwrapped(num));

            return _wrap_final(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_type` from unwrapped `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto from_unwrapped_checked(num_type num) -> final_type
            requires _is_num<num_type>
        {
            contract_expects(is_conversion_safe_from_unwrapped(num));

            return _wrap_final(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_type` from unwrapped `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto from_unwrapped_unchecked(num_type num) -> final_type
            requires _is_num<num_type>
        {
            return _wrap_final(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when creating `this_type` from
        /// `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto is_conversion_safe_from_unwrapped(num_type num) -> bool
            requires _is_num<num_type>
        {
            return impl_type::is_conversion_safe_from_unwrapped(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `num_type::from(*this)`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to() const -> num_type
            requires is_num<num_type>
        {
            return num_type::from(_this_final());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `num_type::from_checked(*this)`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to_checked() const -> num_type
            requires is_num<num_type>
        {
            return num_type::from_checked(_this_final());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `num_type::from_unchecked(*this)`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to_unchecked() const -> num_type
            requires is_num<num_type>
        {
            return num_type::from_unchecked(_this_final());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when creating `this_type` from
        /// `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto is_conversion_safe_to() -> bool
            requires is_num<num_type>
        {
            return num_type::template is_conversion_safe_from<this_type>(_this_final());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `unwrapped_type` value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto to_unwrapped() const -> unwrapped_type
        {
            return _value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` converted to unwrapped `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to_unwrapped() const -> num_type
            requires _is_num<num_type>
        {
            contract_debug_expects(is_conversion_safe_to_unwrapped<num_type>());

            return num_type(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` converted to unwrapped `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to_unwrapped_checked() const -> num_type
            requires _is_num<num_type>
        {
            contract_expects(is_conversion_safe_to_unwrapped<num_type>());

            return num_type(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` converted to unwrapped `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to_unwrapped_unchecked() const -> num_type
            requires _is_num<num_type>
        {
            return num_type(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when converting `this_type` to
        /// unwrapped `num_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto is_conversion_safe_to_unwrapped() const -> bool
            requires _is_num<num_type>
        {
            return impl_type::template is_conversion_safe_to_unwrapped<num_type>(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add(final_type num) const -> final_type
        {
            contract_debug_expects(is_add_safe(num));

            return _wrap_final(_value + num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_checked(final_type num) const -> final_type
        {
            contract_expects(is_add_safe(num));

            return _wrap_final(_value + num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_unchecked(final_type num) const -> final_type
        {
            return _wrap_final(_value + num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`add(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+(final_type num) const -> final_type
        {
            return add(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign(final_type num) -> final_type&
        {
            contract_debug_expects(is_add_safe(num));

            _value += num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign_checked(final_type num) -> final_type&
        {
            contract_expects(is_add_safe(num));

            _value += num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign_unchecked(final_type num) -> final_type&
        {
            _value += num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`add_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+=(final_type num) -> final_type&
        {
            contract_debug_expects(is_add_safe(num));

            return add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`add_assign(1)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(int) -> final_type&
        {
            return add_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is no overflow or underflow occurs during addition.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_add_safe(final_type num) const -> bool
        {
            return impl_type::is_add_safe(_value, num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub(final_type num) const -> final_type
        {
            contract_debug_expects(is_sub_safe(num));

            return _wrap_final(_value - num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_checked(final_type num) const -> final_type
        {
            contract_expects(is_sub_safe(num));

            return _wrap_final(_value - num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_unchecked(final_type num) const -> final_type
        {
            return _wrap_final(_value - num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`sub(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-(final_type num) const -> final_type
        {
            return sub(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign(final_type num) -> final_type&
        {
            contract_debug_expects(is_sub_safe(num));

            _value -= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign_checked(final_type num) -> final_type&
        {
            contract_expects(is_sub_safe(num));

            _value -= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign_unchecked(final_type num) -> final_type&
        {
            _value -= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`sub_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-=(final_type num) -> final_type&
        {
            return sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`sub_assign(1)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator--(int) -> final_type&
        {
            return sub_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is no overflow or underflow occurs during subtraction.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_sub_safe(final_type num) const -> bool
        {
            return impl_type::is_sub_safe(_value, num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul(final_type num) const -> final_type
        {
            contract_debug_expects(is_mul_safe(num));

            return _wrap_final(_value * num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_checked(final_type num) const -> final_type
        {
            contract_expects(is_mul_safe(num));

            return _wrap_final(_value * num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_unchecked(final_type num) const -> final_type
        {
            return _wrap_final(_value * num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`mul(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*(final_type num) const -> final_type
        {
            return mul(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign(final_type num) -> final_type&
        {
            contract_debug_expects(is_mul_safe(num));

            _value *= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign_checked(final_type num) -> final_type&
        {
            contract_expects(is_mul_safe(num));

            _value *= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign_unchecked(final_type num) -> final_type&
        {
            _value *= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`mul_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*=(final_type num) -> final_type&
        {
            return mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is no overflow or underflow occurs during multiplication.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_mul_safe(final_type num) const -> bool
        {
            return impl_type::is_mul_safe(_value, num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div(final_type num) const -> final_type
        {
            contract_debug_expects(is_div_safe(num));

            return _wrap_final(_value / num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_checked(final_type num) const -> final_type
        {
            contract_expects(is_div_safe(num));

            return _wrap_final(_value / num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_unchecked(final_type num) const -> final_type
        {
            return _wrap_final(_value / num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`div(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator/(final_type num) const -> final_type
        {
            return div(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign(final_type num) -> final_type&
        {
            contract_debug_expects(is_div_safe(num));

            _value /= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign_checked(final_type num) -> final_type&
        {
            contract_expects(is_div_safe(num));

            _value /= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign_unchecked(final_type num) -> final_type&
        {
            _value /= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`div_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator/=(final_type num) -> final_type&
        {
            return div_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem(final_type num) const -> final_type
        {
            contract_debug_expects(is_div_safe(num));

            return _wrap_final(_value % num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_checked(final_type num) const -> final_type
        {
            contract_expects(is_div_safe(num));

            return _wrap_final(_value % num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_unchecked(final_type num) const -> final_type
        {
            return _wrap_final(_value % num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`rem(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator%(final_type num) const -> final_type
        {
            return rem(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_assign(final_type num) -> final_type&
        {
            contract_debug_expects(is_div_safe(num));

            _value %= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_assign_checked(final_type num) -> final_type&
        {
            contract_expects(is_div_safe(num));

            _value %= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_assign_unchecked(final_type num) -> final_type&
        {
            _value %= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`rem_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator%=(final_type num) -> final_type&
        {
            return rem_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is no overflow or underflow occurs during division.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_div_safe(final_type num) const -> bool
        {
            return impl_type::is_div_safe(_value, num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns minimum value of `this_type`.
        /// ----------------------------------------------------------------------------------------
        static consteval auto min() -> final_type
        {
            return _wrap_final(impl_type::min());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns maximum value of `this_type`.
        /// ----------------------------------------------------------------------------------------
        static consteval auto max() -> final_type
        {
            return _wrap_final(impl_type::max());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns number of bits this type takes.
        /// ----------------------------------------------------------------------------------------
        static consteval auto bits() -> final_type
        {
            return _wrap_final(impl_type::bits());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the minimum of `this` and `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto min_of(final_type num) const -> final_type
        {
            if (_value > num._value)
                return _wrap_final(num._value);

            return _clone_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the maximum of `this` and `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto max_of(final_type num) const -> final_type
        {
            if (_value < num._value)
                return _wrap_final(num._value);

            return _clone_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// return `this` clamped between `num0` and `num1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto clamp(final_type num0, final_type num1) const -> final_type
        {
            if (_value < num0._value)
                return _wrap_final(num0._value);

            if (_value > num1._value)
                return _wrap_final(num1._value);

            return _clone_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs() const -> final_type
            requires(is_signed())
        {
            contract_debug_expects(is_abs_safe());

            return _wrap_final(impl_type::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs_checked() const -> final_type
            requires(is_signed())
        {
            contract_expects(is_abs_safe());

            return _wrap_final(impl_type::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs_unchecked() const -> final_type
            requires(is_signed())
        {
            return _wrap_final(impl_type::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`abs()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+() const -> final_type
            requires(is_signed())
        {
            return abs();
        }

        /// ------------------------>----------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when performing abs operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_abs_safe() const -> bool
            requires(is_signed())
        {
            return impl_type::is_abs_safe(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg() const -> final_type
            requires(is_signed())
        {
            contract_debug_expects(is_neg_safe());

            return _wrap_final(impl_type::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg_checked() const -> final_type
            requires(is_signed())
        {
            contract_expects(is_neg_safe());

            return _wrap_final(impl_type::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg_unchecked() const -> final_type
            requires(is_signed())
        {
            return _wrap_final(impl_type::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`neg()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-() const -> final_type
            requires(is_signed())
        {
            return neg();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when performing neg operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_neg_safe() const -> bool
            requires(is_signed())
        {
            return impl_type::is_neg_safe(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `1` if `this >= 0` else `-1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sign() const -> final_type
            requires(is_signed())
        {
            return _wrap_final(impl_type::sign(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is positive.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_pos() const -> bool
            requires(is_signed())
        {
            return _value >= 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is negative.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_neg() const -> bool
            requires(is_signed())
        {
            return not is_pos();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is equal to `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator==(final_type num) const -> bool
        {
            return _value == num._value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is less than `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator<(final_type num) const -> bool
        {
            return _value < num._value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is equal to or less than `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator<=(final_type num) const -> bool
        {
            return _value <= num._value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is greater than `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator>(final_type num) const -> bool
        {
            return _value > num._value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is equal to or greater than `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator>=(final_type num) const -> bool
        {
            return _value >= num._value;
        }

    protected:
        constexpr auto _this_final() const -> const final_type&
        {
            return static_cast<const final_type&>(*this);
        }

        constexpr auto _this_final() -> final_type&
        {
            return static_cast<final_type&>(*this);
        }

        constexpr auto _clone_final() const -> final_type
        {
            return _wrap_final(_value);
        }

        static constexpr auto _wrap_final(unwrapped_type val) -> final_type
        {
            return final_type(val);
        }

    public:
        unwrapped_type _value;
    };
};
