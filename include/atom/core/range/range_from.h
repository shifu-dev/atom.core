#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/tti.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/range/range_literal.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"
#include "atom/core/range/range_extensions.h"
#include "atom/core/range/fwd_range_extensions.h"
#include "atom/core/range/bidi_range_extensions.h"
#include "atom/core/range/jump_range_extensions.h"
#include "atom/core/range/array_range_extensions.h"
#include "atom/core/range/mut_range_extensions.h"
#include "atom/core/range/mut_fwd_range_extensions.h"
#include "atom/core/range/mut_bidi_range_extensions.h"
#include "atom/core/range/mut_jump_range_extensions.h"
#include "atom/core/range/mut_array_range_extensions.h"

namespace atom
{
    template <typename in_iter_type, typename in_iter_end_type>
    class _basic_range_from_iter_pair
    {
    public:
        using elem_type = typename in_iter_type::elem_type;
        using iter_type = in_iter_type;
        using iter_end_type = in_iter_end_type;

    public:
        constexpr _basic_range_from_iter_pair(iter_type it, iter_end_type it_end)
            : _it(move(it))
            , _it_end(move(it_end))
        {}

    public:
        constexpr auto get_iter() const -> iter_type
        {
            return _it;
        }

        constexpr auto get_iter_end() const -> iter_type
        {
            return _it_end;
        }

        constexpr auto get_data() const -> const elem_type*
            requires rarray_iter_pair<iter_type, iter_end_type>
        {
            return &_it.value();
        }

        constexpr auto get_count() const -> usize
            requires rjump_iter_pair<iter_type, iter_end_type>
        {
            return _it_end.compare(_it);
        }

    private:
        iter_type _it;
        iter_end_type _it_end;
    };

    template <typename tmut_iter_, typename tmut_iter_end_>
    class _basic_mut_range_from_iter_pair
        : public _basic_range_from_iter_pair<tmut_iter_, tmut_iter_end_>
    {
        using base_type = _basic_range_from_iter_pair<tmut_iter_, tmut_iter_end_>;

    public:
        using mut_iter_type = tmut_iter_;
        using mut_iter_end_type = tmut_iter_end_;

    public:
        constexpr _basic_mut_range_from_iter_pair(mut_iter_type it, mut_iter_end_type it_end)
            : base_type(move(it), move(it_end))
        {}

    public:
        constexpr auto get_mut_iter() -> mut_iter_type
        {
            return this->get_iter();
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_type
        {
            return this->get_iter_end();
        }
    };

    template <typename iter_type, typename iter_end_type>
    class _range_from_iter_extended
    {
    private:
        template <typename t_>
        class _type_container
        {
        public:
            using elem_type = tti::remove_cvref_type<t_>;
        };

        static consteval auto _get()
        {
            using range_type = _basic_range_from_iter_pair<iter_type, iter_end_type>;

            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
                return _type_container<array_range_extensions<range_type>>();

            else if constexpr (rjump_iter_pair<iter_type, iter_end_type>)
                return _type_container<jump_range_extensions<range_type>>();

            else if constexpr (rbidi_iter_pair<iter_type, iter_end_type>)
                return _type_container<bidi_range_extensions<range_type>>();

            else if constexpr (rfwd_iter_pair<iter_type, iter_end_type>)
                return _type_container<fwd_range_extensions<range_type>>();

            else if constexpr (riter_pair<iter_type, iter_end_type>)
                return _type_container<range_extensions<range_type>>();
        }

    public:
        using elem_type = typename decltype(_get())::elem_type;
    };

    template <typename iter_type, typename iter_end_type>
    class _mut_range_from_iter_extended
    {
    private:
        template <typename t_>
        class _type_container
        {
        public:
            using elem_type = tti::remove_cvref_type<t_>;
        };

        static consteval auto _get()
        {
            using range_type = _basic_mut_range_from_iter_pair<iter_type, iter_end_type>;

            if constexpr (rarray_iter_pair<iter_type, iter_end_type>)
                return _type_container<mut_array_range_extensions<range_type>>();

            else if constexpr (rjump_iter_pair<iter_type, iter_end_type>)
                return _type_container<mut_jump_range_extensions<range_type>>();

            else if constexpr (rbidi_iter_pair<iter_type, iter_end_type>)
                return _type_container<mut_bidi_range_extensions<range_type>>();

            else if constexpr (rfwd_iter_pair<iter_type, iter_end_type>)
                return _type_container<mut_fwd_range_extensions<range_type>>();

            else if constexpr (riter_pair<iter_type, iter_end_type>)
                return _type_container<mut_range_extensions<range_type>>();
        }

    public:
        using elem_type = typename decltype(_get())::elem_type;
    };

    template <typename iter_type, typename iter_end_type>
    class _range_from_iter_pair
        : public _range_from_iter_extended<iter_type, iter_end_type>::elem_type
    {
        using base_type = _range_from_iter_extended<iter_type, iter_end_type>::elem_type;

    public:
        constexpr _range_from_iter_pair(iter_type it, iter_end_type it_end)
            : base_type(move(it), move(it_end))
        {}
    };

    template <typename mut_iter_type, typename mut_iter_end_type>
    class _mut_range_from_iter_pair
        : public _mut_range_from_iter_extended<mut_iter_type, mut_iter_end_type>::elem_type
    {
        using base_type =
            _mut_range_from_iter_extended<mut_iter_type, mut_iter_end_type>::elem_type;

    public:
        constexpr _mut_range_from_iter_pair(mut_iter_type it, mut_iter_end_type it_end)
            : base_type(move(it), move(it_end))
        {}
    };

    /// --------------------------------------------------------------------------------------------
    /// # to do
    ///
    /// - review this implementation after implementing character encoding.
    /// --------------------------------------------------------------------------------------------
    constexpr auto _range_find_str_len(const char* str) -> usize
    {
        if (std::is_constant_evaluated())
        {
            usize len = 0;
            while (*str != '\0')
            {
                str++;
                len++;
            }

            return len;
        }

        return std::strlen(str);
    }
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(std::initializer_list<elem_type> list)
    {
        return _range_from_iter_pair(array_iter(list.begin()), array_iter(list.end()));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(const elem_type* begin, const elem_type* end)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(elem_type* begin, elem_type* end)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(end));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(const elem_type* begin, usize count)
    {
        return _range_from_iter_pair(array_iter(begin), array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from(elem_type* begin, usize count)
    {
        return _mut_range_from_iter_pair(mut_array_iter(begin), mut_array_iter(begin + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type, usize count>
    constexpr auto range_from(const elem_type (&arr)[count])
    {
        return _range_from_iter_pair(array_iter(ptr(arr)), array_iter(ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type, usize count>
    constexpr auto range_from(elem_type (&arr)[count])
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(mut_ptr(arr)), mut_array_iter(mut_ptr(arr) + count));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(const char* str)
    {
        return _range_from_iter_pair(
            array_iter(str), array_iter(str + _range_find_str_len(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(char* str)
    {
        return _mut_range_from_iter_pair(
            mut_array_iter(str), mut_array_iter(str + _range_find_str_len(str)));
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iter_type, typename iter_end_type>
    constexpr auto range_from(iter_type it, iter_end_type it_end)
        requires riter_pair<iter_type, iter_end_type>
    {
        if constexpr (rmut_iter<iter_type>)
        {
            return _mut_range_from_iter_pair(move(it), move(it_end));
        }
        else
        {
            return _range_from_iter_pair(move(it), move(it_end));
        }
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    constexpr auto range_from_literal(range_literal<elem_type> lit)
    {
        return range_from(lit.get_data(), lit.get_count());
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto range_from(std::string_view str)
    {
        return range_from(str.data(), str.size());
    }
}