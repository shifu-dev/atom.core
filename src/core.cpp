export module atom.core;
import :std;
import :fmt;
import :wjcrypt;
export import :core;
export import :build_config;
export import :contracts_decl;
export import :static_storage;
export import :time;
export import :tti;
export import :random;
export import :ptr;
export import :mem_ptr;
export import :invokable;
export import :invokable_ptr;
export import :type_list;
export import :array_iter;
export import :range;
export import :memory.legacy_mem_allocator;
export import :memory.default_mem_allocator;
export import :containers.array_view;
export import :containers.static_array;
export import :containers.dynamic_array;
export import :containers.buf_array;
export import :containers.unordered_map;
export import :string_view;
export import :stack_string;
export import :dynamic_string;
export import :buf_string;
export import :string;
export import :default_encoding;
export import :text.utf8_encoding;
export import :text.utf16_encoding;
export import :text.utf32_encoding;

export
{
#include "atom/core_all.h"
}
