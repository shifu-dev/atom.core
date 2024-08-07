export module atom_core:atomic;

import std;

namespace atom
{
    export template <typename value_type>
    using atomic = std::atomic<value_type>;
}
