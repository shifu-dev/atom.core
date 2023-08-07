#pragma once
#include "Exceptions.decl.h"
#include "Atom/Str/Str.h"
#include "Atom/Str/StrView.h"
#include "Atom/Str/AsciiStrView.h"

namespace Atom
{
    cexpr ExceptionSource ExceptionSource::Current(TImpl src) noex
    {
        return ExceptionSource
        {
            .line = src.line(),
            .column = src.column(),
            // TODO: Fix this usage.
            .fileName = Range(src.file_name(), usize(0)),
            .funcName = Range(src.function_name(), usize(0))
        };
    }

    inline Exception::ctor Exception(Str msg) noex:
        msg(mov(msg)) { }

    inline Exception::dtor Exception() noex { }

    inline IndexOutOfRangeException::ctor IndexOutOfRangeException(
        Str msg, usize index, usize begin, usize end):
        OutOfRangeException(mov(msg)), index{ index }, begin{ begin }, end{ end } { }

    inline IndexOutOfRangeException::ctor IndexOutOfRangeException(
        usize index, usize begin, usize end):
        OutOfRangeException(),
        index{ index }, begin{ begin }, end{ end } { }
}

namespace Atom::Ex::Internal
{
    inline ctor Thrower::Thrower(ExceptionSource src, StackTrace stackTrace) noex:
        _src{ mov(src) }, _stackTrace{ mov(stackTrace) } { }

    inline fn Thrower::RecordStack(StackTrace stackTrace) -> Thrower&
    {
        this->_stackTrace = mov(stackTrace);
        return *this;
    }

    inline fn Thrower::RecordSource(ExceptionSource src) -> Thrower&
    {
        this->_src = mov(src);
        return *this;
    }

    template <tname TEx>
    requires RDerivedFrom<TEx, Exception>
    inline fn Thrower::op<<(TEx&& ex)
    {
        ex.src = mov(_src);
        ex.stackTrace = mov(_stackTrace);

        throw ex;
    }
}
