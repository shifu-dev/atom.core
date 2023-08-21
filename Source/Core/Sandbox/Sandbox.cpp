#include "Atom/Core.h"
#include "Atom/Core/Result.h"

using namespace Atom;

auto PrintCmdArgs(i32 argc, char** argv)
{
    for (i32 i = 1; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }
}

auto main(i32 argc, char** argv) -> i32
{
    PrintCmdArgs(argc, argv);

    Result<void, int> res;

    return 0;
}
