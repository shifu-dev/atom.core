#pragma once
#include "_ConstArrImplHelper.decl.h"
#include "Atom/Exceptions.h"

namespace Atom
{
    template <typename TImpl>
    constexpr auto _ConstArrImplHelper<TImpl>::ElemAt(usize index) const
        -> const TElem&
    {
        ATOM_EXPECTS(_ValidateIndexForAccess(index)) << IndexOutOfRangeException(
            ATOM_TEXT("{index} was out of range[{0}, {1}]."), index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <typename TImpl>
    constexpr auto _ConstArrImplHelper<TImpl>::operator [](usize index) const noexcept
        -> const TElem&
    {
        ATOM_DEBUG_EXPECTS(_ValidateIndexForAccess(index)) << IndexOutOfRangeException(
            ATOM_TEXT("{index} was out of range[{0}, {1}]."), index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <typename TImpl>
    constexpr auto _ConstArrImplHelper<TImpl>::ElemFront() const noexcept
        -> const TElem&
    {
        ATOM_DEBUG_EXPECTS(!IsEmpty()) << OutOfRangeException(
            String(Range(ATOM_TEXT("Arr is empty."))));

        return _Data()[0];
    }

    template <typename TImpl>
    constexpr auto _ConstArrImplHelper<TImpl>::ElemBack() const noexcept
        -> const TElem&
    {
        ATOM_DEBUG_EXPECTS(!IsEmpty()) << OutOfRangeException(
            ATOM_TEXT("Arr is empty."));

        return _Data()[_Count() - 1];
    }
}