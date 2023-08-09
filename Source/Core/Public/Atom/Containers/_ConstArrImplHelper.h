#pragma once
#include "_ConstArrImplHelper.decl.h"
#include "Atom/Exceptions.h"

namespace Atom
{
    template <typename TImpl>
    cexpr fn _ConstArrImplHelper<TImpl>::ElemAt(usize index) const -> const TElem&
    {
        ATOM_EXPECTS(_ValidateIndexForAccess(index)) <<
            IndexOutOfRangeException(index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <typename TImpl>
    cexpr fn _ConstArrImplHelper<TImpl>::op[](usize index) const noex -> const TElem&
    {
        ATOM_DEBUG_EXPECTS(_ValidateIndexForAccess(index)) <<
            IndexOutOfRangeException(index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <typename TImpl>
    cexpr fn _ConstArrImplHelper<TImpl>::ElemFront() const noex -> const TElem&
    {
        ATOM_DEBUG_EXPECTS(!isEmpty()) <<
            OutOfRangeException("Arr is empty.");

        return _Data()[0];
    }

    template <typename TImpl>
    cexpr fn _ConstArrImplHelper<TImpl>::ElemBack() const noex -> const TElem&
    {
        ATOM_DEBUG_EXPECTS(!isEmpty()) <<
            OutOfRangeException("Arr is empty.");

        return _Data()[_Count() - 1];
    }
}