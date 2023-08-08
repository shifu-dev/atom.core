#pragma once
#include "IterReq.h"
#include "IterReqChecks.h"
#include "IterReqMocks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template<tname TWrap>
    class _BasicMutIterWrap extends TWrap
    {
        pub using Base = TWrap;
        pub using TElem = tname Base::TElem;

        pub using Base::Base;

        pub using Base::op*;
        pub using Base::op->;

        pub cexpr fn op*() noex -> TElem&
        {
            return &self->iter;
        }

        pub cexpr fn op->() noex -> TElem*
        {
            return &self->iter;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TWrap>
    requires (!RIter<TWrap>)
    class _BasicMutIterWrap<TWrap> extends TWrap { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    class IterWrap
    {
        pub using TElem = TTI::TEnableIf<RIter<TIter>, tname TIter::TElem>;

        pub cexpr ctor IterWrap(TIter iter) noex:
            iter{ iter } { }

        pub template <tname... TArgs>
        requires RConstructible<TIter, TArgs...>
        cexpr ctor IterWrap(TArgs&&... args) noex:
            iter{ fwd(args)... } { }

        pub cexpr fn op*() const noex -> const TElem&
        {
            return self->iter;
        }

        pub cexpr fn op->() const noex -> const TElem*
        {
            return &self->iter;
        }

        pub cexpr fn op++(i32) noex -> IterWrap&
        {
            self.iter++;
            return self;
        }

        pub template <tname TIterEnd>
        cexpr fn op==(const IterWrap<TIterEnd>& end) const noex -> bool
        {
            return self.iter == end.iter;
        }

        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires (!RIter<TIter>)
    class IterWrap<TIter>
    {
        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    class MutIterWrap extends _BasicMutIterWrap<IterWrap<TIter>>
    {
        pub using Base = _BasicMutIterWrap<IterWrap<TIter>>;
        pub using Base::Base;

        pub cexpr ctor MutIterWrap(TIter iter) noex:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    class FwdIterWrap extends IterWrap<TIter>,
        public MultiPassIterTag
    {
        pub using Base = IterWrap<TIter>;
        pub using Base::Base;

        pub cexpr ctor FwdIterWrap(TIter iter) noex:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RMutFwdIter<TIter>
    class MutFwdIterWrap extends _BasicMutIterWrap<FwdIterWrap<TIter>>
    {
        pub using Base = _BasicMutIterWrap<FwdIterWrap<TIter>>;
        pub using Base::Base;

        pub cexpr ctor MutFwdIterWrap(TIter iter) noex:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RBidiIter<TIter>
    class BidiIterWrap extends FwdIterWrap<TIter>
    {
        pub using Base = FwdIterWrap<TIter>;
        pub using Base::Base;

        pub cexpr ctor BidiIterWrap(TIter iter) noex:
            Base{ iter } { }

        pub cexpr fn op--(i32) noex -> BidiIterWrap&
        {
            self.iter--;
            return self;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RMutBidiIter<TIter>
    class MutBidiIterWrap extends _BasicMutIterWrap<BidiIterWrap<TIter>>
    {
        pub using Base = _BasicMutIterWrap<BidiIterWrap<TIter>>;
        pub using Base::Base;

        pub cexpr ctor MutBidiIterWrap(TIter iter) noex:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RJumpIter<TIter>
    class JumpIterWrap extends BidiIterWrap<TIter>
    {
        pub using Base = BidiIterWrap<TIter>;
        pub using Base::Base;

        pub cexpr ctor JumpIterWrap(TIter iter) noex:
            Base{ iter } { }

        pub cexpr fn op+(isize steps) const noex -> JumpIterWrap
        {
            return JumpIterWrap{ self.iter + steps };
        }

        pub cexpr fn op-(isize steps) const noex -> JumpIterWrap
        {
            return JumpIterWrap{ self.iter - steps };
        }

        pub cexpr fn op+=(isize steps) noex -> JumpIterWrap&
        {
            self.iter += steps;
            return self;
        }

        pub cexpr fn op-=(isize steps) noex -> JumpIterWrap&
        {
            self.iter -= steps;
            return self;
        }

        template <tname TIter2>
        pub cexpr fn op-(const TIter2& iter2) const noex -> isize
        {
            return self.iter - iter2;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RMutJumpIter<TIter>
    class MutJumpIterWrap extends _BasicMutIterWrap<JumpIterWrap<TIter>>
    {
        pub using Base = _BasicMutIterWrap<JumpIterWrap<TIter>>;
        pub using Base::Base;

        pub cexpr ctor MutJumpIterWrap(TIter iter) noex:
            Base{ iter } { }

        pub cexpr fn op+(isize steps) const noex -> MutJumpIterWrap
        {
            return MutJumpIterWrap{ self.iter + steps };
        }

        pub cexpr fn op-(isize steps) const noex -> MutJumpIterWrap
        {
            return MutJumpIterWrap{ self.iter - steps };
        }

        template <tname TIter2>
        pub cexpr fn op-(const TIter2& iter2) const noex -> isize
        {
            return self.iter - iter2;
        }
    };
}