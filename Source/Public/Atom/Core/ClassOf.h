#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T, typename... Ts>
    class ClassOf: public ClassOf<Ts...>
    {
    public:
        T value;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ClassOf<T>
    {
    public:
        T value;
    };
}