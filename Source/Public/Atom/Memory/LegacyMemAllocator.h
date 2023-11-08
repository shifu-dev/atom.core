#pragma once
#include "Atom/Core.h"

#include <memory>

namespace Atom
{
    class LegacyMemAllocator
    {
    public:
        auto Alloc(usize size) -> MemPtr<void>
        {
            return std::malloc(size);
        }

        auto Realloc(MemPtr<void> mem, usize size) -> MemPtr<void>
        {
            return std::realloc(mem.raw(), size.val());
        }

        auto Dealloc(MemPtr<void> mem)
        {
            std::free(mem.raw());
        }
    };
}
