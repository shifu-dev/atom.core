#pragma once
#include "StrArgFormatters.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    struct _FmtStrViewCnvter
    {
        constexpr StrView FromFmt(_FmtStrView strv) noexcept
        {
            return StrView{ Range(strv.data(), strv.size()) };
        }

        constexpr _FmtStrView ToFmt(StrView strv) noexcept
        {
            return _FmtStrView{ strv.Data(), strv.Count() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Wrapper over {StrView} to represent format string. This is done to avoid compile 
    /// time checks.
    /// --------------------------------------------------------------------------------------------
    struct RunFmtStr
    {
        StrView str;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RStrFmtArgFmtable... TArgs>
    using FmtStr = _FmtFmtStr<TArgs...>;
//     struct FmtStr
//     {
//         template <typename T>
//         consteval FmtStr(const T& strv) noexcept { }
//             _fmt{ _FmtStrViewCnvter().ToFmt(strv) } { }
// 
//         FmtStr(RunFmtStr str) noexcept { }
//             _fmt{ _FmtRunFmtStr{ _FmtStrViewCnvter().ToFmt(str.str) } } { }
// 
//         _FmtFmtStr<TArgs...> _fmt;
//     };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    struct StrFmter
    {
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TOut, RStrFmtArgFmtable... TArgs>
        requires ROutput<TOut, Char>
        void FmtTo(TOut out, FmtStr<TArgs...> fmt, TArgs&&... args)
        {
            struct _OutIterWrap
            {
                _OutIterWrap& operator ++ (int)
                {
                    return *this;
                }

                _OutIterWrap& operator * ()
                {
                    return *this;
                }

                _OutIterWrap& operator = (Char ch)
                {
                    *out+= ch;
                    return *this;
                }

                TOut* out;
            };

            fmt::detail::iterator_buffer<_OutIterWrap, Char> buf{ _OutIterWrap{ &out } };

            try
            {
                fmt::detail::vformat_to<Char>(buf, fmt,
                    fmt::make_format_args<fmt::buffer_context<Char>>(FORWARD(args)...),
                    fmt::detail::locale_ref{});
            }
            catch (const _FmtFmtEx& err)
            {
                throw StrFmtEx(err);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RStrFmtArgFmtable... TArgs>
        Str Fmt(FmtStr<TArgs...> fmt, TArgs&&... args)
        {
            Str out;
            FmtTo(out, fmt, FORWARD(args)...);

            return out;
        }
    };
}

#define ATOM_STR_FMT(fmt, ...) \
    ::Atom::StrFmter(TEXT(fmt), __VA_ARGS__)