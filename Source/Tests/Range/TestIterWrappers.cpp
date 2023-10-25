// #include "catch2/catch_all.hpp"
// #include "Atom/Range/IterWrappers.h"
// #include "Atom/Range/ArrIter.h"
//
// using namespace Atom;
//
// TEST_CASE("Atom.Range.IterWrappers", "[.compilation]")
// {
//     using IterWrapMock = IterWrap<MutArrIter<i32>>;
//     using MutIterWrapMock = MutIterWrap<MutArrIter<i32>>;
//     using FwdIterWrapMock = FwdIterWrap<MutArrIter<i32>>;
//     using MutFwdIterWrapMock = MutFwdIterWrap<MutArrIter<i32>>;
//     using BidiIterWrapMock = BidiIterWrap<MutArrIter<i32>>;
//     using MutBidiIterWrapMock = MutBidiIterWrap<MutArrIter<i32>>;
//     using JumpIterWrapMock = JumpIterWrap<MutArrIter<i32>>;
//     using MutJumpIterWrapMock = MutJumpIterWrap<MutArrIter<i32>>;
//
//     STATIC_CHECK(RIter<IterWrapMock>);
//     STATIC_CHECK_FALSE(RMutIter<IterWrapMock>);
//     STATIC_CHECK_FALSE(RFwdIter<IterWrapMock>);
//
//     STATIC_CHECK(RMutIter<MutIterWrapMock>);
//     STATIC_CHECK_FALSE(RMutFwdIter<IterWrapMock>);
//
//     STATIC_CHECK(RFwdIter<FwdIterWrapMock>);
//     STATIC_CHECK_FALSE(RMutFwdIter<FwdIterWrapMock>);
//     STATIC_CHECK_FALSE(RBidiIter<IterWrapMock>);
//
//     STATIC_CHECK(RMutFwdIter<MutFwdIterWrapMock>);
//     STATIC_CHECK_FALSE(RMutBidiIter<IterWrapMock>);
//
//     STATIC_CHECK(RBidiIter<BidiIterWrapMock>);
//     STATIC_CHECK_FALSE(RMutBidiIter<BidiIterWrapMock>);
//     STATIC_CHECK_FALSE(RJumpIter<IterWrapMock>);
//
//     STATIC_CHECK(RMutBidiIter<MutBidiIterWrapMock>);
//     STATIC_CHECK_FALSE(RMutJumpIter<IterWrapMock>);
//
//     STATIC_CHECK(RJumpIter<JumpIterWrapMock>);
//     STATIC_CHECK_FALSE(RMutJumpIter<JumpIterWrapMock>);
//     STATIC_CHECK_FALSE(RMutArrIter<IterWrapMock>);
//
//     STATIC_CHECK(RMutJumpIter<MutJumpIterWrapMock>);
//     STATIC_CHECK_FALSE(RMutArrIter<IterWrapMock>);
// }
//
// TEST_CASE("Atom.Range.IterWrappers", "[.interface]")
// {
//     class CustomIter: MutArrIter<i32>
//     {
//     public:
//         using Base = MutArrIter<i32>;
//
//     public:
//         constexpr CustomIter() { }
//
//         constexpr CustomIter(i32, char, f32, const char*) { }
//
//         constexpr CustomIter(const Base& base):
//             Base{ base } { }
//     };
//
//     SECTION("")
//     {
//         MutArrIter<i32> iter;
//
//         IterWrap iterWrap{ iter };
//         MutIterWrap mutIterWrap{ iter };
//         FwdIterWrap fwdIterWrap{ iter };
//         MutFwdIterWrap mutFwdIterWrap{ iter };
//         BidiIterWrap bidiIterWrap{ iter };
//         MutBidiIterWrap mutBidiIterWrap{ iter };
//         JumpIterWrap jumpIterWrap{ iter };
//         MutJumpIterWrap mutJumpIterWrap{ iter };
//     }
//
//     SECTION("")
//     {
//         using IterWrapMock = IterWrap<CustomIter>;
//         using MutIterWrapMock = MutIterWrap<CustomIter>;
//         using FwdIterWrapMock = FwdIterWrap<CustomIter>;
//         using MutFwdIterWrapMock = MutFwdIterWrap<CustomIter>;
//         using BidiIterWrapMock = BidiIterWrap<CustomIter>;
//         using MutBidiIterWrapMock = MutBidiIterWrap<CustomIter>;
//         using JumpIterWrapMock = JumpIterWrap<CustomIter>;
//         using MutJumpIterWrapMock = MutJumpIterWrap<CustomIter>;
//
//         IterWrapMock iterWrap{ 0, 'a', 0.0f, "hello" };
//         MutIterWrapMock mutIterWrap{ 0, 'a', 0.0f, "hello" };
//         FwdIterWrapMock fwdIterWrap{ 0, 'a', 0.0f, "hello" };
//         MutFwdIterWrapMock mutFwdIterWrap{ 0, 'a', 0.0f, "hello" };
//         BidiIterWrapMock bidiIterWrap{ 0, 'a', 0.0f, "hello" };
//         MutBidiIterWrapMock mutBidiIterWrap{ 0, 'a', 0.0f, "hello" };
//         JumpIterWrapMock jumpIterWrap{ 0, 'a', 0.0f, "hello" };
//         MutJumpIterWrapMock mutJumpIterWrap{ 0, 'a', 0.0f, "hello" };
//     }
// }