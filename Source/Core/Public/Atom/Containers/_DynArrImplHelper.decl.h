#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <typename TImpl>
    class _DynArrImplHelper: public _ArrImplHelper<TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

        using Base = _ArrImplHelper<TImpl>;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Insert
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename T2, typename = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr fn InsertAt(TIter pos, T2&& el) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr fn InsertAt(TIter pos, const TRange& range) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at front.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename T2, typename = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr fn InsertFront(T2&& el) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at front.
        /// 
        /// @PARAM el: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr fn InsertFront(const TRange& range) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at back.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename T2, typename = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr fn InsertBack(T2&& el) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at back.
        /// 
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr fn InsertBack(const TRange& range) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Calls {InsertBack(...)}.
        /// ----------------------------------------------------------------------------------------
        template <typename T2, typename = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr fn operator+=(T2&& el) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Calls {InsertBack(...)}.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr fn operator+=(const TRange& range) -> TMutIter;

    private:
        // TODO: Refactor this. Should we use T2 here to denote other typename?
        template <typename T2>
        constexpr fn _InsertAt(usize index, T2&& el) -> usize;

        template <typename TIter2>
        constexpr fn _InsertAtCounted(usize index, TIter2 it, usize count) -> usize;

        template <typename TIter2, typename TIterEnd2>
        constexpr fn _InsertAtUncounted(usize index, TIter2 begin, TIterEnd2 end) -> usize;

        template <typename T2>
        constexpr fn _InsertBack(T2&& el) -> usize;

        template <typename TIter2>
        constexpr fn _InsertBackCounted(TIter2 it, usize count) -> usize;

        template <typename TIter2, typename TIterEnd2>
        constexpr fn _InsertBackUncounted(TIter2 begin, TIterEnd2 end) -> usize;

        constexpr fn _ValidateIndexForInsert(isize index) const -> bool;

    //// -------------------------------------------------------------------------------------------
    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Removes element at pos{pos}.
        /// 
        /// @PARAM pos: Position to remove element at.
        /// 
        /// @RETURNS {TMutIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        constexpr fn RemoveAt(TIter pos) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Removes element at range{range}.
        /// 
        /// @PARAM range: Range to remove element at.
        /// 
        /// @RETURNS {TMutIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        constexpr fn RemoveRange(Range<TIter, TIterEnd> range) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn Clear() -> void;

    public:
        constexpr fn _RemoveAt(usize index) -> usize;

        constexpr fn _RemoveRange(usize begin, usize count) -> usize;

    //// -------------------------------------------------------------------------------------------
    //// Memory
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn Reserve(usize size);

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn Release() -> void;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn Capacity() const -> usize;

        using Base::iter;
        using Base::iterEnd;
        using Base::mutIter;
        using Base::mutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    protected:
        constexpr fn _ValidateIter(TIter it) const -> bool;
        constexpr fn _UpdateIterDebugId() -> void;
        constexpr fn _ValidateIndex(isize index) const -> bool;
        constexpr fn _FetchIndex(TIter pos) const -> isize;
        constexpr fn _CalcCapGrowth(usize required) const -> usize;
        constexpr fn _EnsureCapFor(usize count) -> void;

        constexpr fn _ConstructAt(usize index, auto&&... args) -> void;
        constexpr fn _DestructAt(usize index) -> void;
        constexpr fn _DestructRange(usize index, usize count) -> void;
        constexpr fn _MoveRangeFront(usize index, usize count) -> void;
        constexpr fn _MoveRangeBack(usize index, usize count) -> void;
        constexpr fn _MoveRangeTo(usize index, TElem* dest) -> void;
        constexpr fn _RotateRangeBack(usize index, usize count) -> void;

        template <typename TRange>
        static constexpr fn _CanGetRangeSize() -> bool;

        template <typename TRange>
        static constexpr fn _GetRangeSize(const TRange& range) -> usize;

        using Base::_Data;
        using Base::_Count;
        using Base::_Capacity;
        using Base::_AllocMem;
        using Base::_DeallocMem;
    };
}