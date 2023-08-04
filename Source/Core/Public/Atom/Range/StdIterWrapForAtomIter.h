#pragma once
#include "RangeReq.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    using StdIterCatForAtomIter = TTI::TEnableIf
    <
        RIter<TIter>,
        TTI::TNotConditional
        <
            RFwdIter<TIter>, std::input_iterator_tag,
            TTI::TNotConditional
            <
                RBidiIter<TIter>, std::forward_iterator_tag,
                TTI::TNotConditional
                <
                    RJumpIter<TIter>, std::bidirectional_iterator_tag,
                    TTI::TNotConditional
                    <
                        RArrIter<TIter>, std::random_access_iterator_tag,
                        std::contiguous_iterator_tag
                    >
                >
            >
        >
    >;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    class StdIterWrapForAtomIter
    {
        using This = StdIterWrapForAtomIter<TIter>;

    public:
        using value_type = tname TIter::TElem;
        using size_type = usize;
        using difference_type = isize;
        using iterator_category = StdIterCatForAtomIter<TIter>;
        using pointer = value_type*;
        using reference = value_type&;

    public:
        cexpr ctor StdIterWrapForAtomIter(const TIter& iter) noex:
            iter{ iter } { }

        cexpr ctor StdIterWrapForAtomIter(TIter& iter) noex:
            iter{ iter } { }

        cexpr ctor StdIterWrapForAtomIter(TIter&& iter) noex:
            iter{ mov(iter) } { }

    public:
        template <class = void>
        requires RIter<TIter>
        cexpr fn op*() const noex_if(*iter) -> const value_type&
        {
            return *iter;
        }

        template <class = void>
        requires RMutIter<TIter>
        cexpr fn op*() noex_if(*iter) -> value_type&
        {
            return *iter;
        }

        template <class TIterEnd>
        requires RIterEnd<TIter, TIterEnd>
        cexpr fn op== (const StdIterWrapForAtomIter<TIterEnd>& that)
            const noex_if(iter == that.iter) -> bool
        {
            return iter == that.iter;
        }

        template <class TIterEnd>
        requires RIterEnd<TIter, TIterEnd>
        cexpr fn op!= (const StdIterWrapForAtomIter<TIterEnd>& that)
            const noex_if(iter != that.iter) -> bool
        {
            return iter != that.iter;
        }

        template <class = void>
        requires RIter<TIter>
        cexpr fn op++() noex_if(++iter) -> This&
        {
            ++iter;
            return *this;
        }

        template <class = void>
        requires RIter<TIter>
        cexpr fn op++(int) noex_if(iter++) -> This
        {
            return This{ iter++ };
        }

        template <class = void>
        requires RBidiIter<TIter>
        cexpr fn op--() noex_if(--iter) -> This&
        {
            --iter;
            return *this;
        }

        template <class = void>
        requires RBidiIter<TIter>
        cexpr fn op--(int) const noex_if(iter--) -> This
        {
            return This{ iter-- };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn op+(difference_type steps) noex_if(iter + steps) -> This
        {
            return This{ iter + steps };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn op-(difference_type steps) noex_if(iter - steps) -> This
        {
            return This{ iter - steps };
        }

        template <class = void>
        requires RJumpIter<TIter>
        cexpr fn op-(const This& that) noex_if(iter - that.iter) -> difference_type
        {
            return iter - that.iter;
        }

    public:
        TIter iter;
    };
}