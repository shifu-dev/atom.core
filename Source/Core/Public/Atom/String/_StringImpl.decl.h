#pragma once
#include "Atom/Text/BasicChar.h"
#include "Atom/Text/BasicRune.h"

namespace Atom
{
    template <typename _TEncoding, typename _TContainer>
    class _StringImpl: public _TContainer
    {
    public:
        using TEncoding            = _TEncoding;

        using TChar                = BasicChar<TEncoding>;
        using TIter                = typename _TContainer::TIter;
        using TIterEnd             = typename _TContainer::TIterEnd;

        using TRune                = BasicRune<TEncoding>;
        // using TRuneIter            = typename _TContainer::TRuneIter;
        // using TRuneIterEnd         = typename _TContainer::TRuneIterEnd;
        // using TConstRuneIter       = typename _TContainer::TConstRuneIter;
        // using TConstRuneIterEnd    = typename _TContainer::TConstRuneIterEnd;
        // using TRevRuneIter         = typename _TContainer::TRevRuneIter;
        // using TRevRuneIterEnd      = typename _TContainer::TRevRuneIterEnd;
        // using TConstRevRuneIter    = typename _TContainer::TConstRevRuneIter;
        // using TConstRevRuneIterEnd = typename _TContainer::TConstRevRuneIterEnd;

    public:
        using _TContainer::_TContainer;
        using _TContainer::operator =;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr _StringImpl(const TChar* arr) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, TChar>
        constexpr bool operator ==(const TRange& range) const noexcept
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, TChar>
        constexpr bool operator !=(const TRange& range) const noexcept
        {
            return !(*this == range);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize Size>
        constexpr bool operator ==(const TChar(&arr)[Size]) const noexcept
        {
            return true;
        }
    };
}