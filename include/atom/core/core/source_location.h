#pragma once
#include "atom/core/core/build_config.h"

#include <source_location>
#include <string_view>

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// info about the source code line.
    /// --------------------------------------------------------------------------------------------
    class source_location
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// get the source line info of the calling site.
        ///
        /// @note this_t default argument is used to capture the source line info of the calling
        ///     site. do not pass any argument for this.
        /// ----------------------------------------------------------------------------------------
#if defined(ATOM_COMPILER_CLANG) && !defined(__cpp_lib_source_location)
        static consteval auto current(const char* file_name = __builtin_FILE(),
            const char* func_name = __builtin_FUNCTION(), u32 line = __builtin_LINE(),
            u32 column = __builtin_COLUMN()) -> source_location
        {
            return source_location{
                .line = line, .column = column, .func_name = func_name, .file_name = file_name
            };
        }
#else
        static consteval auto current(std::source_location src = std::source_location::current())
            -> source_location
        {
            return source_location(src.file_name(), src.line(), src.column(), src.function_name());
        }
#endif

    public:
        constexpr source_location(std::string_view file_name, unsigned int line,
            unsigned int column, std::string_view func_name)
            : file_name(file_name)
            , line(line)
            , column(column)
            , func_name(func_name)
        {}

    public:
        std::string_view file_name;
        unsigned int line;
        unsigned int column;
        std::string_view func_name;
    };
}
