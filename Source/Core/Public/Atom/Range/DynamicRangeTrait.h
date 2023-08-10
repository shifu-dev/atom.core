#pragma once
#include "MutRangeTrait.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// # To Do: Implement this.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RDynamicRange = RMutRange<TRange>;

    template <typename T>
    class DynamicRangeTraitImpl
    {
    };

    template <typename T>
    class _DynamicRangeTraitImpl
    {
    };

    template <typename T>
    class DynamicRangeTrait
    {
        using _Impl = DynamicRangeTraitImpl<T>;

    public:
        using TElem = typename _Impl::TElem;
        using TMutIter = typename _Impl::TMutIter;
        using TMutIterEnd = typename _Impl::TMutIterEnd;

        //// -------------------------------------------------------------------------------------------
        //// Removal
        //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TPred>
        constexpr fn removeIf(TPred&& pred)->usize
            requires(RInvokable<TPred, bool(const TElem&)>)
        {
            usize count = 0;
            for (auto it = _impl.iter(); it != _impl.iterEnd(); it++)
            {
                if (pred(*it))
                {
                    it = _impl.removeAt(it);
                    count++;
                }
            }

            return count;
        }

    private:
        _Impl _impl;
    };
}