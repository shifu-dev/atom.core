export module atom.core:types.type_info;

import std;
import :types.type_info_impl;

namespace atom
{
    export using type_id = std::size_t;

    export template <typename in_value_type>
    class type_info
    {
        using this_type = type_info;
        using usize = std::size_t;

    private:
        template <typename value_type>
        static consteval auto _get_sizeof() -> usize
        {
            if constexpr (std::is_void_v<value_type>)
            {
                return 0;
            }
            else
            {
                return sizeof(value_type);
            }
        }

        template <typename value_type>
        static consteval auto get_alignof() -> usize
        {
            if constexpr (std::is_void_v<value_type>)
            {
                return 0;
            }
            else
            {
                return alignof(value_type);
            }
        }

    public:
        using value_type = in_value_type;

        using add_const_type = type_info<std::add_const_t<value_type>>;
        using remove_const_type = type_info<std::remove_const_t<value_type>>;

        using add_volatile_type = type_info<std::add_volatile_t<value_type>>;
        using remove_volatile_type = type_info<std::remove_volatile_t<value_type>>;

        using unqualified_type = type_info<std::remove_cv_t<value_type>>;

        using add_lvalue_ref_type = type_info<std::add_lvalue_reference_t<value_type>>;
        using add_rvalue_ref_type = type_info<std::add_rvalue_reference_t<value_type>>;
        using remove_ref_type = type_info<std::remove_reference_t<value_type>>;

        using add_ptr_type = type_info<std::add_pointer_t<value_type>>;
        using remove_ptr_type = type_info<std::remove_pointer_t<value_type>>;

        using pure_type = type_info<std::remove_cvref_t<value_type>>;

        template <typename like_type>
        using unpure_like_type = type_info<
            typename type_info_impl::unpure_like_type<in_value_type, like_type>::value_type>;

        static constexpr bool size = _get_sizeof<value_type>();
        static constexpr bool align = _get_alignof<value_type>();

        template <typename other_type>
        static constexpr bool is_same_as = std::is_same_v<value_type, other_type>;

        template <typename base_type>
        static constexpr bool is_derived_from = std::is_base_of_v<base_type, value_type>;

        template <typename base_type>
        static constexpr bool is_not_derived_from = not is_derived_from<base_type>;

        template <typename other_type>
        static constexpr bool is_same_or_derived_from =
            is_same_as<other_type> or is_derived_from<other_type>;

        static constexpr bool is_complete = type_info_impl::is_complete<value_type>::value;

        static constexpr bool is_void = std::is_void_v<value_type>;
        static constexpr bool is_not_void = not is_void;

        static constexpr bool is_lvalue_ref = std::is_lvalue_reference_v<value_type>;
        static constexpr bool is_not_lvalue_ref = not is_lvalue_ref;

        static constexpr bool is_rvalue_ref = std::is_rvalue_reference_v<value_type>;
        static constexpr bool is_not_rvalue_ref = not is_rvalue_ref;

        static constexpr bool is_ref = std::is_reference_v<value_type>;
        static constexpr bool is_not_ref = not is_ref;

        static constexpr bool is_const = std::is_const_v<value_type>;
        static constexpr bool is_not_const = not is_const;

        static constexpr bool is_volatile = std::is_volatile_v<value_type>;
        static constexpr bool is_not_volatile = not is_volatile;

        static constexpr bool is_qualified = is_const || is_volatile;
        static constexpr bool is_not_qualified = not is_qualified;

        static constexpr bool is_empty = std::is_empty_v<value_type>;
        static constexpr bool is_not_empty = not is_empty;

        static constexpr bool is_pure = not is_qualified and not is_ref;
        static constexpr bool is_not_pure = not is_pure;

        static constexpr bool is_enum = std::is_enum_v<value_type>;
        static constexpr bool is_not_enum = not is_enum;

        template <typename signature>
        static constexpr bool is_function =
            type_info_impl::template is_function<value_type, signature>::value;

        template <typename... args_type>
        static constexpr bool is_constructible_from =
            std::is_constructible_v<value_type, args_type...>;

        template <typename... args_type>
        static constexpr bool is_trivially_constructible_from =
            std::is_trivially_constructible_v<value_type, args_type...>;

        template <typename... args_type>
        static constexpr bool is_assignable_from = std::is_assignable_v<value_type, args_type...>;

        static constexpr bool is_swappable = std::is_swappable_v<value_type>;

        template <typename other_type>
        static constexpr bool is_convertible_to = std::is_convertible_v<value_type, other_type>;

        template <typename... args_type>
        static constexpr bool is_trivially_asignable_from =
            std::is_trivially_assignable_v<value_type, args_type...>;

        static constexpr bool is_default_constructible =
            std::is_default_constructible_v<value_type>;
        static constexpr bool is_not_default_constructible = not is_default_constructible;
        static constexpr bool is_trivially_default_constructible =
            std::is_trivially_default_constructible_v<value_type>;
        static constexpr bool is_not_trivially_default_constructible =
            not is_trivially_default_constructible;

        static constexpr bool is_copy_constructible = std::is_copy_constructible_v<value_type>;
        static constexpr bool is_not_copy_constructible = not is_copy_constructible;
        static constexpr bool is_trivially_copy_constructible =
            std::is_trivially_copy_constructible_v<value_type>;
        static constexpr bool is_not_trivially_copy_constructible =
            not is_trivially_copy_constructible;

        static constexpr bool is_copy_assignable = std::is_copy_assignable_v<value_type>;
        static constexpr bool is_not_copy_assignable = not is_copy_assignable;
        static constexpr bool is_trivially_copy_assignable =
            std::is_trivially_copy_assignable_v<value_type>;
        static constexpr bool is_not_trivially_copy_assignable = not is_trivially_copy_assignable;

        static constexpr bool is_copyable = is_copy_constructible and is_copy_assignable;
        static constexpr bool is_not_copyable = not is_copyable;
        static constexpr bool is_trivially_copyable =
            is_trivially_copy_constructible and is_trivially_copy_assignable;
        static constexpr bool is_not_trivially_copyable = not is_trivially_copyable;

        static constexpr bool is_move_constructible = std::is_move_constructible_v<value_type>;
        static constexpr bool is_not_move_constructible = not is_move_constructible;
        static constexpr bool is_trivially_move_constructible =
            std::is_trivially_move_constructible_v<value_type>;
        static constexpr bool is_not_trivially_move_constructible =
            not is_trivially_move_constructible;

        static constexpr bool is_move_assignable = std::is_move_assignable_v<value_type>;
        static constexpr bool is_not_move_assignable = not is_move_assignable;
        static constexpr bool is_trivially_move_assignable =
            std::is_trivially_move_assignable_v<value_type>;
        static constexpr bool is_not_trivially_move_assignable = not is_trivially_move_assignable;

        static constexpr bool is_moveable = is_move_constructible and is_move_assignable;
        static constexpr bool is_not_moveable = not is_moveable;
        static constexpr bool is_trivially_moveable =
            is_trivially_move_constructible and is_trivially_move_assignable;
        static constexpr bool is_not_trivially_moveable = not is_trivially_moveable;

        static constexpr bool is_destructible = std::is_destructible_v<value_type>;
        static constexpr bool is_not_destructible = not is_destructible;
        static constexpr bool is_trivially_destructible =
            std::is_trivially_destructible_v<value_type>;
        static constexpr bool is_not_trivially_destructible = not is_trivially_destructible;

        template <typename other_type>
        static constexpr bool is_equality_comparable_with = true;

        static constexpr bool is_equality_comparable = true;

        template <typename other_type>
        static constexpr bool is_comparable_with = true;

        static constexpr bool is_comparable = true;

        constexpr auto operator==(const this_type& other) -> bool
        {
            return true;
        }

        template <typename other_type>
        constexpr auto operator==(const other_type& other) -> bool
        {
            return false;
        }

        template <typename other_type>
        constexpr auto operator!=(const other_type& other) -> bool
        {
            return not *this == other;
        }

        static auto get_id() -> type_id
        {
            return typeid(value_type).hash_code();
        }
    };
}