#pragma once
#include "atom/core/_std.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/core.h"
#include "atom/core/core/requirements.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"
#include "atom/core/range/std_iter_wrap_for_atom_iter.h"

namespace atom
{
    template <typename range_type>
    class _range_extensions_impl
    {
    protected:
        using _impl_type = range_type;

    public:
        using elem_type = typename _impl_type::elem_type;
        using iter_type = typename _impl_type::iter_type;
        using iter_end_type = typename _impl_type::iter_end_type;

    public:
        constexpr _range_extensions_impl(range_type& range)
            : _range_(range)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter() const -> iter_type
        {
            return _range().get_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_end() const -> iter_end_type
        {
            return _range().get_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename telem1>
        constexpr auto find_elem(const telem1& el) const -> iter_type
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter());
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end());
            return std::find(std_iter, std_iter_end, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_type>
        constexpr auto find_range(const other_range_type& range) const -> iter_type
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter());
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end());
            std_iter_wrap_for_atom_iter std_iter1(range.get_iter());
            std_iter_wrap_for_atom_iter std_iter_end1(range.get_iter_end());
            return std::find(std_iter, std_iter_end, std_iter1, std_iter_end1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_type>
        auto compare(const other_range_type& range) const -> i8
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter());
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end());
            std_iter_wrap_for_atom_iter std_iter1(range.get_iter());
            std_iter_wrap_for_atom_iter std_iter_end1(range.get_iter_end());

            return std::equal(std_iter, std_iter_end, std_iter1, std_iter_end1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            if constexpr (rjump_iter_pair<iter_type, iter_end_type>)
            {
                return get_iter_end() - get_iter();
            }

            usize count = 0;
            for (auto it = get_iter(); it != get_iter_end(); it++)
                count++;

            return count;
        }

    protected:
        constexpr auto _range() const -> const range_type&
        {
            return _range_;
        }

        constexpr auto _range() -> range_type&
        {
            return _range_;
        }

    private:
        range_type& _range_;
    };

    template <typename range_type, typename _trange_extensions_impl = void>
    class range_extensions: public range_type
    {
        using this_type = range_extensions;
        using base_type = range_type;

    protected:
        using _impl_type = _trange_extensions_impl;

    public:
        using elem_type = typename _impl_type::elem_type;
        using iter_type = typename _impl_type::iter_type;
        using iter_end_type = typename _impl_type::iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter() const -> iter_type
        {
            return _impl().get_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_end() const -> iter_end_type
        {
            return _impl().get_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() const
        {
            return std_iter_wrap_for_atom_iter(_impl().get_iter());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() const
        {
            return std_iter_wrap_for_atom_iter(_impl().get_iter_end());
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// search
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename telem1>
        constexpr auto find(const telem1& el) const -> iter_type
            requires(requality_comparable_with<elem_type, telem1>)
        {
            return _impl().find_elem(el);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_type>
        constexpr auto find_range(const other_range_type& range) const -> iter_type
            requires(rfwd_range<other_range_type>)
                    and (requality_comparable_with<elem_type, typename other_range_type::elem_type>)
        {
            return _impl().find_range(range);
        }

        template <typename other_range_type>
        auto count_any(const other_range_type& range) const -> usize
        {
            usize count = 0;
            for (auto it = get_iter(); it.compare(get_iter_end()) != 0; it++)
                for (const auto& el : range)
                {
                    if (*it == el)
                        count++;
                }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename telem1>
        constexpr auto contains(const telem1& el) const -> bool
            requires(requality_comparable_with<elem_type, telem1>)
        {
            return _impl().find_elem(el).compare(_impl().get_iter_end()) != 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_type>
        constexpr auto contains(const other_range_type& range) const -> bool
            requires(rfwd_range<other_range_type>)
                    and (requality_comparable_with<elem_type, typename other_range_type::elem_type>)
        {
            return _impl().find_range(range).compare(_impl().get_iter_end()) != 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// comparision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_type>
        constexpr auto compare(const other_range_type& range) const -> i8
            requires(rrange<other_range_type>)
                    and (requality_comparable_with<elem_type, typename other_range_type::elem_type>)
        {
            return _impl().compare(range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename other_range_type>
        constexpr auto is_eq(const other_range_type& range) const -> bool
            requires(rrange<other_range_type>)
                    and (requality_comparable_with<elem_type, typename other_range_type::elem_type>)
        {
            return _impl().compare(range) == 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// helpers
        ////
        //// # to do
        //// - add range helper functions.
        //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto can_get_count() const -> bool
        {
            return rfwd_iter_pair<iter_type, iter_end_type>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _impl().get_count();
        }

    protected:
        constexpr auto _impl() const -> const _impl_type
        {
            return _impl_type(const_cast<this_type&>(*this));
        }

        constexpr auto _impl() -> _impl_type
        {
            return _impl_type(*this);
        }
    };

    template <typename range_type>
    class range_extensions<range_type, void>
        : public range_extensions<range_type, _range_extensions_impl<range_type>>
    {
        using base_type = range_extensions<range_type, _range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}