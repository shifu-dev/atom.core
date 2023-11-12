#pragma once
#include "Atom/Core/Primitives.h"

namespace Atom
{
    // clang-format off

    template <typename TPtr>
    concept RConstPtr = requires(TPtr ptr)
    {
        typename TPtr::TVal;

        { ptr.craw() } -> RSameAs<const typename TPtr::TVal*>;
    };

    template <typename TPtr>
    concept RPtr = requires(TPtr ptr)
    {
        requires RConstPtr<TPtr>;

        { ptr.raw() } -> RSameAs<typename TPtr::TVal*>;
    };

    template <typename TPtr, typename T>
    concept RPtrOf = requires(TPtr ptr)
    {
        requires RPtr<TPtr>;
        requires RSameAs<T, typename TPtr::TVal>;
    };

    // clang-format on

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TVal_>
    class ConstPtr
    {
        using This = ConstPtr<TVal_>;

    public:
        using TVal = TVal_;

    public:
        constexpr ConstPtr():
            _ptr{ nullptr }
        {}

        constexpr ConstPtr(const This&) = default;
        constexpr ConstPtr& operator=(const This&) = default;

        constexpr ConstPtr(This&&) = default;
        constexpr ConstPtr& operator=(This&&) = default;

        constexpr ConstPtr(std::nullptr_t):
            _ptr{ nullptr } { }

        constexpr ConstPtr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        template <typename TPtr>
        constexpr ConstPtr(const TPtr& ptr)
            requires(RConstPtr<TPtr>) and (RConvertibleTo<const typename TPtr::TVal*, const TVal*>)
            : _ptr{ static_cast<const TVal*>(ptr.craw()) }
        {}

        template <typename TPtr>
        constexpr ConstPtr& operator=(const TPtr& ptr)
            requires(RConstPtr<TPtr>) and (RConvertibleTo<const typename TPtr::TVal*, const TVal*>)
        {
            _ptr = static_cast<const TVal*>(ptr.craw());
            return *this;
        }

        constexpr ConstPtr(const TVal* ptr):
            _ptr{ ptr }
        {}

        constexpr ConstPtr& operator=(const TVal* ptr)
        {
            _ptr = ptr;
            return *this;
        }

        constexpr ~ConstPtr() = default;

    public:
        constexpr auto cval() const -> const TVal&
        {
            return *_ptr;
        }

        constexpr auto val() const -> const TVal&
        {
            return *_ptr;
        }

        constexpr auto operator*() const -> const TVal&
        {
            return *_ptr;
        }

        constexpr auto operator->() const -> const TVal*
        {
            return _ptr;
        }

        constexpr auto operator[](isize n) const -> const TVal&
        {
            return _ptr[n.val()];
        }

        constexpr auto operator[](usize n) const -> const TVal&
        {
            return _ptr[n.val()];
        }

        constexpr auto craw() const -> const TVal*
        {
            return _ptr;
        }

        constexpr auto raw() const -> const TVal*
        {
            return _ptr;
        }

        template <typename T>
        constexpr auto reinterpretCast() const -> ConstPtr<T>
        {
            return reinterpret_cast<T*>(_ptr);
        }

    protected:
        const TVal* _ptr;
    };

    template <typename T>
    ConstPtr(const T* ptr) -> ConstPtr<T>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <>
    class ConstPtr<void>
    {
        using This = ConstPtr<void>;

    public:
        using TVal = void;

    public:
        constexpr ConstPtr():
            _ptr{ nullptr }
        {}

        constexpr ConstPtr(const This&) = default;
        constexpr ConstPtr& operator=(const This&) = default;

        constexpr ConstPtr(This&&) = default;
        constexpr ConstPtr& operator=(This&&) = default;

        constexpr ConstPtr(std::nullptr_t):
            _ptr{ nullptr } { }

        constexpr ConstPtr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        template <typename TPtr>
        constexpr ConstPtr(const TPtr& ptr)
            requires(RConstPtr<TPtr>)
            : _ptr{ static_cast<const void*>(ptr.craw()) }
        {}

        template <typename TPtr>
        constexpr ConstPtr& operator=(const TPtr& ptr)
            requires(RConstPtr<TPtr>)
        {
            _ptr = static_cast<const void*>(ptr.craw());
            return *this;
        }

        template <typename T>
        constexpr ConstPtr(const T* ptr):
            _ptr{ static_cast<const void*>(ptr) }
        {}

        template <typename T>
        constexpr ConstPtr& operator=(const T* ptr)
        {
            _ptr = static_cast<const void*>(ptr);
            return *this;
        }

        constexpr ~ConstPtr() = default;

    public:
        constexpr auto craw() const -> const void*
        {
            return _ptr;
        }

        constexpr auto raw() const -> const void*
        {
            return _ptr;
        }

        constexpr auto byteRaw() const -> const byte*
        {
            return static_cast<const byte*>(_ptr);
        }

        template <typename T>
        constexpr auto reinterpretCast() const -> ConstPtr<T>
        {
            return reinterpret_cast<const T*>(_ptr);
        }

    protected:
        const void* _ptr;
    };

    ConstPtr(const void*) -> ConstPtr<const void>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TVal_, typename _TBase = void>
    class Ptr: public ConstPtr<TVal_>
    {
        using This = Ptr<TVal_>;
        using Base = ConstPtr<TVal_>;

    public:
        using TVal = Base::TVal;

    public:
        constexpr Ptr() = default;

        constexpr Ptr(const Ptr&) = default;
        constexpr Ptr& operator=(const Ptr&) = default;

        constexpr Ptr(Ptr&&) = default;
        constexpr Ptr& operator=(Ptr&&) = default;

        constexpr Ptr(std::nullptr_t):
            Base(nullptr) { }

        constexpr Ptr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        template <typename TPtr>
        constexpr Ptr(TPtr& ptr)
            requires(RPtr<TPtr>) and (RConvertibleTo<typename TPtr::TVal*, TVal*>)
            : Base(static_cast<TVal*>(ptr.raw()))
        {}

        template <typename TPtr>
        constexpr Ptr(TPtr&& ptr)
            requires(RPtr<TPtr>) and (RConvertibleTo<typename TPtr::TVal*, TVal*>)
            : Base(static_cast<TVal*>(ptr.raw()))
        {}

        template <typename TPtr>
        constexpr Ptr& operator=(TPtr& ptr)
            requires(RPtr<TPtr>) and (RConvertibleTo<typename TPtr::TVal*, TVal*>)
        {
            _ptr = static_cast<TVal*>(ptr.raw());
            return *this;
        }

        constexpr Ptr(TVal* ptr):
            Base(ptr)
        {}

        constexpr Ptr& operator=(TVal* ptr)
        {
            _ptr = ptr;
            return *this;
        }

        constexpr ~Ptr() = default;

    public:
        constexpr auto val() -> TVal&
        {
            return *_mutPtr();
        }

        constexpr auto operator*() -> TVal&
        {
            return *_mutPtr();
        }

        constexpr auto operator->() -> TVal*
        {
            return _mutPtr();
        }

        constexpr auto operator[](isize n) -> TVal&
        {
            return _mutPtr()[n.val()];
        }

        constexpr auto operator[](usize n) -> TVal&
        {
            return _mutPtr()[n.val()];
        }

        constexpr auto raw() -> TVal*
        {
            return _mutPtr();
        }

        using Base::raw;

    private:
        constexpr auto _mutPtr() -> TVal*
        {
            return const_cast<TVal*>(_ptr);
        }

    protected:
        using Base::_ptr;
    };

    template <typename T>
    Ptr(T* ptr) -> Ptr<T>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename _TBase>
    class Ptr<void, _TBase>: public ConstPtr<void>
    {
        using This = Ptr<void>;
        using Base = ConstPtr<void>;

    public:
        constexpr Ptr() = default;

        constexpr Ptr(const Ptr&) = default;
        constexpr Ptr& operator=(const Ptr&) = default;

        constexpr Ptr(Ptr&&) = default;
        constexpr Ptr& operator=(Ptr&&) = default;

        constexpr Ptr(std::nullptr_t):
            Base(nullptr) { }

        constexpr Ptr& operator=(std::nullptr_t)
        {
            _ptr = nullptr;
            return *this;
        }

        template <typename TPtr>
        constexpr Ptr(TPtr& ptr)
            requires(RPtr<TPtr>)
            : Base(static_cast<void*>(ptr.raw()))
        {}

        template <typename TPtr>
        constexpr Ptr(TPtr&& ptr)
            requires(RPtr<TPtr>)
            : Base(static_cast<void*>(ptr.raw()))
        {}

        template <typename TPtr>
        constexpr Ptr& operator=(TPtr& ptr)
            requires(RPtr<TPtr>)
        {
            _ptr = static_cast<void*>(ptr.raw());
            return *this;
        }

        template <typename T>
        constexpr Ptr(T* ptr)
            requires(not RConst<T>)
            : Base(static_cast<void*>(ptr))
        {}

        template <typename T>
        constexpr Ptr& operator=(T* ptr)
            requires(not RConst<T>)
        {
            _ptr = static_cast<void*>(ptr);
            return *this;
        }

        constexpr ~Ptr() = default;

    public:
        constexpr auto raw() -> void*
        {
            return _mutPtr();
        }

        using Base::raw;

        constexpr auto byteRaw() -> byte*
        {
            return static_cast<byte*>(_mutPtr());
        }

        template <typename T>
        constexpr auto reinterpretCast() -> Ptr<T>
        {
            return reinterpret_cast<T*>(_mutPtr());
        }

    private:
        constexpr auto _mutPtr() -> void*
        {
            return const_cast<void*>(_ptr);
        }

    protected:
        using Base::_ptr;
    };

    Ptr(void*) -> Ptr<void>;
}