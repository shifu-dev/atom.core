#pragma once
#include "Atom/Core/Exceptions.h"

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// Exception representing error during processing character encoding.
    /// --------------------------------------------------------------------------------------------
    class EncodingException: public Exception
    {
    public:
        EncodingException(Str msg):
            Exception(mov(msg)) {}
    };
}