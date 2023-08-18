#pragma once
#include "fmt/format.h"

#include "Str.h"
#include "StrConverter.h"
#include "StrViewConverter.h"

#include "Atom/Exceptions.h"
#include "Atom/TTI.h"

namespace Atom
{
    using _FmtFmtParseCtx = ::fmt::basic_format_parse_context<Char>;
    using _FmtFmtParseCtxIter = typename _FmtFmtParseCtx::iterator;

    using _FmtFmtCtxOut = ::fmt::detail::buffer_appender<Char>;
    using _FmtFmtCtx = ::fmt::basic_format_context<_FmtFmtCtxOut, Char>;

    template <typename... TArgs>
    using _FmtFmtStr = ::fmt::basic_format_string<Char, fmt::type_identity_t<TArgs>...>;
    using _FmtRunFmtStr = ::fmt::runtime_format_string<Char>;
    using _FmtStrView = ::fmt::basic_string_view<Char>;

    template <typename T>
    using _FmtFmter = ::fmt::formatter<T, Char>;

    using _FmtFmtEx = ::fmt::format_error;

    /// --------------------------------------------------------------------------------------------
    /// Err thrown during formatting error.
    /// --------------------------------------------------------------------------------------------
    class StrFmtErr: public Err
    {
    public:
        StrFmtErr(ErrMsg msg): Err(mov(msg)) {}

        /// ----------------------------------------------------------------------------------------
        /// @TODO Fix this ugly code.
        /// ----------------------------------------------------------------------------------------
    public:
        StrFmtErr(const _FmtFmtEx& fmtEx): Err("Not implemented.") {}
        // StrFmtErr(const _FmtFmtEx& fmtEx):
        // 	Err(CharEncodingConverter<UTF8CharEncoding, CharEncoding>()
        // 		.Convert(UTF8StrView{ (const char8*)fmtEx.what() }.iter()).data()) { }
    };

    /// --------------------------------------------------------------------------------------------
    /// Context to parse format string.
    /// --------------------------------------------------------------------------------------------
    class StrFmtParseCtx
    {
    public:
        constexpr StrFmtParseCtx(_FmtFmtParseCtx& fmtCtx): _fmtCtx{ fmtCtx } {}

    public:
        auto GetRange() -> StrView
        {
            auto range = Range(_fmtCtx.begin(), _fmtCtx.end());
            return StrView(range);
        }

        auto AdvanceTo(ArrIter<Char> it)
        {
            _fmtCtx.advance_to(it.data());
        }

    public:
        _FmtFmtParseCtx& _fmtCtx;
    };

    /// --------------------------------------------------------------------------------------------
    /// Context to write formatted string.
    /// --------------------------------------------------------------------------------------------
    class StrFmtCtx
    {
    public:
        constexpr StrFmtCtx(_FmtFmtCtx& fmtCtx): _fmtCtx{ fmtCtx } {}

        auto Write(Char ch)
        {
            auto out = _fmtCtx.out();
            *out++ = ch;
            _fmtCtx.advance_to(mov(out));
        }

        template <typename TRange>
        auto Write(const TRange& chars)
            requires(RRangeOf<TRange, Char>)
        {
            auto out = _fmtCtx.out();
            for (Char ch : chars)
            {
                *out++ = ch;
            }

            _fmtCtx.advance_to(mov(out));
        }

    public:
        _FmtFmtCtx& _fmtCtx;
    };

    /// --------------------------------------------------------------------------------------------
    /// Parses and writes string representation for types according to the specified format
    /// specifiers.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class StrFmtArgFmterImpl;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    using StrFmtArgFmter = StrFmtArgFmterImpl<TTI::TRemoveCVRef<T>>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TFmter} is {StrFmtArgFmter} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFmter, typename T>
    concept RStrFmtArgFmter =
        requires(TFmter fmter, T arg, StrFmtParseCtx parseCtx, StrFmtCtx ctx) {
            fmter.Parse(parseCtx);
            fmter.Fmt(arg, ctx);
        };

    /// --------------------------------------------------------------------------------------------
    /// StrArgFmtable refers to a type for which exists a valid {StrFmtArgFmter<T>} specialization
    /// which satisfies {RStrFmtArgFmter<StrFmtArgFmter<T>, T>} requirement.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {StrArgFmtable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStrFmtArgFmtable = RStrFmtArgFmter<StrFmtArgFmter<T>, T>;
}

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {StrFmtArgFmter} specialization for {StrView}.
    ///
    /// @INTERNAL
    /// Uses {fmt::fmter<fmt::string_view>} specialization.
    /// --------------------------------------------------------------------------------------------
    template <>
    class StrFmtArgFmterImpl<StrView>
    {
    public:
        auto Parse(StrFmtParseCtx& ctx)
        {
            _FmtFmtParseCtx& fmtCtx = ctx._fmtCtx;

            fmtCtx.advance_to(_fmtFmter.parse(fmtCtx));
        }

        auto Fmt(StrView str, StrFmtCtx& ctx)
        {
            _FmtFmtCtx& fmtCtx = ctx._fmtCtx;

            _FmtStrView fmt_str{ str.data(), str.count() };
            fmtCtx.advance_to(_fmtFmter.format(fmt_str, fmtCtx));
        }

    public:
        _FmtFmter<_FmtStrView> _fmtFmter;
    };

    static_assert(RStrFmtArgFmtable<StrView>, "StrView is not formattable.");

    /// --------------------------------------------------------------------------------------------
    /// {StrFmtArgFmter} specialization for {Char} arr.
    /// --------------------------------------------------------------------------------------------
    template <usize N>
    class StrFmtArgFmterImpl<Char[N]>: public StrFmtArgFmter<StrView>
    {
    public:
        auto Fmt(const Char (&chars)[N], StrFmtCtx& ctx)
        {
            StrView str{ chars, N };
            StrFmtArgFmter<StrView>::Fmt(str, ctx);
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {StrFmtArgFmter} specialization for types which satisfy {RStrViewConvertible} requirement.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
        requires(RStrViewConvertible<T>)
    class StrFmtArgFmterImpl<T>: public StrFmtArgFmter<StrView>
    {
    public:
        constexpr auto Fmt(const T& in, StrFmtCtx& ctx)
        {
            StrFmtArgFmter<StrView>::Fmt(convter.Convert(in), ctx);
        }

    public:
        StrViewConverter<T> convter;
    };

    template <typename T>
    class _FmtFmtfierFilter
    {
    public:
        static constexpr bool Enable = true;
    };

    template <usize N>
    class _FmtFmtfierFilter<Char[N]>
    {
    public:
        static constexpr bool Enable = false;
    };
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// @INTERNAL
    ///
    /// {fmt::fmter} specialization for all types that implement {Atom::StrFmtArgFmter}.
    /// {fmt} uses this type and users specialize {Atom::StrFmtArgFmter}.
    ///
    /// This is specialized for {Char} character type only as {Atom} uses that type for
    /// character representation.
    /// --------------------------------------------------------------------------------------------
    template <Atom::RStrFmtArgFmtable T>
        requires(Atom::_FmtFmtfierFilter<T>::Enable)
    class formatter<T, Atom::Char>
    {
    public:
        auto parse(Atom::_FmtFmtParseCtx& fmtCtx) -> Atom::_FmtFmtParseCtxIter
        {
            Atom::StrFmtParseCtx ctx(fmtCtx);
            this->fmter.Parse(ctx);
            return fmtCtx.begin();
        }

        auto format(const T& in, Atom::_FmtFmtCtx& fmtCtx) -> Atom::_FmtFmtCtxOut
        {
            Atom::StrFmtCtx ctx(fmtCtx);
            this->fmter.Fmt(in, ctx);
            return fmtCtx.out();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// This contains actual implementation.
        /// ----------------------------------------------------------------------------------------
        Atom::StrFmtArgFmter<T> fmter;
    };
}
