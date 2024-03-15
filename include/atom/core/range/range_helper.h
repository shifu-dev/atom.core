#pragma once
#include "atom/core/_std.h"
#include "atom/core/core.h"
#include "atom/core/preprocessors.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"
#include "atom/core/range/std_iter_wrap_for_atom_iter.h"

namespace atom
{
    class range_helper
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto can_get_count() const -> bool
            requires rrange<range_t>
        {
            return rfwd_range<range_t>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto get_count(const range_t& range) const -> usize
            requires rfwd_range<range_t>
        {
            return _get_count(range.get_iter(), range.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t, class elem_t>
        constexpr auto fill(range_t&& range, elem_t&& val) const -> void
            requires rmut_range<range_t>
                     and rassignable<typename range_t::elem_t, elem_t>
        {
            _fill(range.get_iter(), range.get_iter_end(), forward<elem_t>(val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        template <class range_t, typename elem_t>
        constexpr auto fill_explicit(range_t&& range, elem_t&& val) const -> void
            requires rmut_range<range_t>
                     and rassignable<typename range_t::elem_t, elem_t>
        {
            _fill(range.get_iter(), range.get_iter_end(), forward<elem_t>(val));
        }

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t_0, class range_t_1>
        constexpr auto fwd_copy_to(const range_t_0& range0, range_t_1&& range1) const -> void
            requires rrange<range_t_0> and rmut_range<range_t_1>
                     and rassignable<typename range_t_1::elem_t,
                         typename range_t_0::elem_t>
        {
            _fwd_copy(range0.get_iter(), range0.get_iter_end(), range1.get_iter(),
                range1.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t_0, class range_t_1>
        constexpr auto bwd_copy_to(const range_t_0& range0, range_t_1&& range1) const -> void
            requires rrange<range_t_0> and rmut_range<range_t_1>
                     and rassignable<typename range_t_1::elem_t,
                         typename range_t_0::elem_t>
        {
            _bwd_copy(range0.get_iter(), range0.get_iter_end(), range1.get_iter(),
                range1.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t_0, class range_t_1>
        constexpr auto fwd_move_to(range_t_0&& range0, range_t_1&& range1) const -> void
            requires rmut_range<range_t_0> and rmut_range<range_t_1>
                     and rassignable<typename range_t_1::elem_t,
                         typename range_t_0::elem_t>
        {
            _fwd_move(range0.get_iter(), range0.get_iter_end(), range1.get_iter(),
                range1.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t_0, class range_t_1>
        constexpr auto bwd_move_to(range_t_0&& range0, range_t_1&& range1) const -> void
            requires rmut_range<range_t_0> and rmut_range<range_t_1>
                     and rassignable<typename range_t_1::elem_t,
                         typename range_t_0::elem_t>
        {
            _bwd_move(range0.get_iter(), range0.get_iter_end(), range1.get_iter(),
                range1.get_iter_end());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto shift_fwd(range_t&& range, usize steps) const -> void
            requires rmut_fwd_range<range_t> and rmove_assignable<typename range_t::elem_t>
        {
            _fwd_shift(range.get_iter(), range.get_iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto shift_bwd(range_t&& range, usize steps) const -> void
            requires rmut_fwd_range<range_t> and rmove_assignable<typename range_t::elem_t>
        {
            _bwd_shift(range.get_iter(), range.get_iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto shift_by(range_t&& range, isize steps) const -> void
            requires rmut_fwd_range<range_t> and rswappable<typename range_t::elem_t>
        {
            if (steps > 0)
            {
                _bwd_shift(range.get_iter(), range.get_iter_end(), steps);
            }
            else
            {
                _fwd_shift(range.get_iter(), range.get_iter_end(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto rotate_fwd(range_t&& range, usize steps) const -> void
            requires rmut_range<range_t> and rswappable<typename range_t::elem_t>
        {
            _fwd_rotate(range.get_iter(), range.get_iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto rotate_bwd(range_t&& range, usize steps) const -> void
            requires rmut_range<range_t> and rswappable<typename range_t::elem_t>
        {
            _bwd_rotate(range.get_iter(), range.get_iter_end(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto rotate_by(range_t&& range, isize steps) const -> void
            requires rmut_range<range_t> and rswappable<typename range_t::elem_t>
        {
            if (steps > 0)
            {
                _bwd_rotate(range.get_iter(), range.get_iter_end(), steps);
            }
            else
            {
                _fwd_rotate(range.get_iter(), range.get_iter_end(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class range_t>
        constexpr auto destruct(range_t&& range) const -> void
            requires rmut_range<range_t> and rdestructible<typename range_t::elem_t>
        {
            _destruct(range.get_iter(), range.get_iter_end());
        }

    private:
        template <class iter_t, class iter_end_t>
        constexpr auto _get_count(iter_t iter, iter_end_t iter_end) const -> usize
        {
            if constexpr (rjump_iter_pair<iter_t, iter_end_t>)
            {
                return iter.compare(iter_end).abs().template to<usize>();
            }

            usize count = 0;
            for (; not iter.is_eq(iter_end); iter.next())
                count++;

            return count;
        }

        template <class iter_t, class iter_end_t, typename elem_t>
        constexpr auto _fill(iter_t iter, iter_end_t iter_end, elem_t&& val) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::fill(std_iter, std_iter_end, forward<elem_t>(val));
        }

        template <class iter_t_0, class iter_end_t_0, class iter_t_1,
            class iter_end_t_1>
        constexpr auto _fwd_copy(iter_t_0 iter0, iter_end_t_0 iter_end0, iter_t_1 iter1,
            iter_end_t_1 iter_end1) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter0(iter0);
            std_iter_wrap_for_atom_iter std_iter_end0(iter_end0);
            std_iter_wrap_for_atom_iter std_iter1(iter1);

            std::copy(std_iter0, std_iter_end0, std_iter1);
        }

        template <class iter_t_0, class iter_end_t_0, class iter_t_1,
            class iter_end_t_1>
        constexpr auto _bwd_copy(iter_t_0 iter0, iter_end_t_0 iter_end0, iter_t_1 iter1,
            iter_end_t_1 iter_end1) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter0(iter0);
            std_iter_wrap_for_atom_iter std_iter_end0(iter_end0);
            std_iter_wrap_for_atom_iter std_iter1(iter1);

            std::copy_backward(std_iter0, std_iter_end0, std_iter1);
        }

        template <class iter_t_0, class iter_end_t_0, class iter_t_1,
            class iter_end_t_1>
        constexpr auto _fwd_move(iter_t_0 iter0, iter_end_t_0 iter_end0, iter_t_1 iter1,
            iter_end_t_1 iter_end1) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter0(iter0);
            std_iter_wrap_for_atom_iter std_iter_end0(iter_end0);
            std_iter_wrap_for_atom_iter std_iter1(iter1);

            std::move(std_iter0, std_iter_end0, std_iter1);
        }

        template <class iter_t_0, class iter_end_t_0, class iter_t_1,
            class iter_end_t_1>
        constexpr auto _bwd_move(iter_t_0 iter0, iter_end_t_0 iter_end0, iter_t_1 iter1,
            iter_end_t_1 iter_end1) const -> void
        {
            std_iter_wrap_for_atom_iter std_iter0(iter0);
            std_iter_wrap_for_atom_iter std_iter_end0(iter_end0);
            std_iter_wrap_for_atom_iter std_iter1(iter1);

            std::move_backward(std_iter0, std_iter_end0, std_iter1);
        }

        template <class iter_t, class iter_end_t>
        constexpr auto _fwd_shift(iter_t iter, iter_end_t iter_end, usize steps) const -> void
        {
            if constexpr (rarray_iter_pair<iter_t, iter_end_t>)
            {
                std::shift_right(iter.get_data(), iter_end.get_data(), steps);
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::shift_right(std_iter, std_iter_end, steps);
        }

        template <class iter_t, class iter_end_t>
        constexpr auto _bwd_shift(iter_t iter, iter_end_t iter_end, usize steps) const -> void
        {
            if constexpr (rarray_iter_pair<iter_t, iter_end_t>)
            {
                std::shift_left(iter.get_data(), iter_end.get_data(), steps);
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::shift_left(std_iter, std_iter_end, steps);
        }

        template <class iter_t, class iter_end_t>
        constexpr auto _bwd_rotate(iter_t iter, iter_end_t iter_end, usize steps) const
            -> void
        {
            if constexpr (rarray_iter_pair<iter_t, iter_end_t>)
            {
                std::rotate(iter.get_data(), iter.get_data() + steps, iter_end.get_data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::rotate(std_iter, std_iter + steps, std_iter_end);
        }

        template <class iter_t, class iter_end_t>
        constexpr auto _fwd_rotate(iter_t iter, iter_end_t iter_end, isize steps) const
            -> void
        {
            if constexpr (rarray_iter_pair<iter_t, iter_end_t>)
            {
                std::rotate(iter.get_data(), iter_end.get_data() - steps, iter_end.get_data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::rotate(std_iter, std_iter_end - steps, std_iter_end);
        }

        template <class iter_t, class iter_end_t>
        constexpr auto _destruct(iter_t iter, iter_end_t iter_end) const -> void
        {
            if constexpr (rarray_iter_pair<iter_t, iter_end_t>)
            {
                std::destroy(iter.get_data(), iter_end.get_data());
                return;
            }

            std_iter_wrap_for_atom_iter std_iter(iter);
            std_iter_wrap_for_atom_iter std_iter_end(iter_end);

            std::destroy(std_iter, std_iter_end);
        }
    };
}
