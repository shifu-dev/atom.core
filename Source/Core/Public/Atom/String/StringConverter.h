#pragma once
#include "Atom/TTI.h"
#include "Atom/Containers/OutputReqMock.h"

#include "Atom/String/String.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts {T} object to {String}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class StringConverter;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TStringConverter} can convert {T} object to {String}.
    /// 
    /// @TPARAM[IN] TConverter Converter type to convert to stirng.
    /// @TPARAM[IN] T Object type to convert to stirng.
    /// --------------------------------------------------------------------------------------------
    template <typename TStringConverter, typename T>
    concept RStringConverter = requires(TStringConverter converter, T obj, OutputReqMock<Char> out)
    {
        { converter.Convert(obj) }
            -> RSameAs<String>;

        { converter.Convert(obj, out) }
            -> RSameAs<void>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {StringConverter<T>} for {T} is {RStringConverter}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStringConvertible = RStringConverter<StringConverter<T>, T>;

    /// --------------------------------------------------------------------------------------------
    /// {StirngConverter} specialization for {T} containing {const}, {volatile} or {lvalue} 
    /// and {rvalue} reference.
    /// 
    /// @TODO Needs refactoring.
    /// --------------------------------------------------------------------------------------------
	template <typename T>
	requires (!RSameAs<T, TTI::TRemoveCVRef<T>>) && RStringConvertible<TTI::TRemoveCVRef<T>>
	struct StringConverter<T>: StringConverter<TTI::TRemoveCVRef<T>> { };

    /// --------------------------------------------------------------------------------------------
    /// {StringConverter} specialization for {String}.
    /// --------------------------------------------------------------------------------------------
    template < >
    struct StringConverter<String>
    {
        constexpr String Convert(String&& str) noexcept
        {
            return MOVE(str);
        }

        constexpr String Convert(const String& str) noexcept
        {
            return String{ str };
        }

        constexpr void Convert(const String& str, ROutput<Char> auto out) noexcept
        {
            out += str;
        }
    };
}