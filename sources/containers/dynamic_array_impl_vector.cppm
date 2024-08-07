export module atom_core:containers.dynamic_array_impl_vector;

import std;
import :core;
import :ranges;

namespace atom
{
    template <typename in_value_type, typename in_allocator_type>
    class dynamic_array_impl_vector
    {
        using this_type = dynamic_array_impl_vector;

    public:
        using value_type = in_value_type;
        using allocator_type = in_allocator_type;
        using iterator_type = const value_type*;
        using iterator_end_type = iterator_type;
        using mut_iterator_type = value_type*;
        using mut_iterator_end_type = mut_iterator_type;

    public:
        class copy_tag
        {};

        class move_tag
        {};

        class range_tag
        {};

    public:
        constexpr dynamic_array_impl_vector()
            : _vector{}
        {}

        constexpr dynamic_array_impl_vector(copy_tag, const dynamic_array_impl_vector& that)
            : _vector{ that._vector }
        {}

        constexpr dynamic_array_impl_vector(move_tag, dynamic_array_impl_vector& that)
            : _vector{ std::move(that._vector) }
        {}

        template <typename other_iterator_type, typename other_iterator_end_type>
        constexpr dynamic_array_impl_vector(
            range_tag, other_iterator_type it, other_iterator_end_type it_end)
            : _vector{ std::move(it), std::move(it_end) }
        {}

        constexpr dynamic_array_impl_vector(create_from_raw_tag, const value_type* arr, usize count)
            : _vector{ arr, arr + count }
        {}

        constexpr dynamic_array_impl_vector(create_with_count_tag, usize count)
            : _vector(count)
        {}

        constexpr dynamic_array_impl_vector(
            create_with_count_tag, usize count, const value_type& value)
            : _vector{ count, value }
        {}

        constexpr dynamic_array_impl_vector(create_with_capacity_tag, usize capacity)
            : _vector{}
        {
            _vector.reserve(capacity);
        }

        constexpr ~dynamic_array_impl_vector() {}

    public:
        constexpr auto move_this(this_type& that) -> void
        {
            _vector = std::move(that._vector);
        }

        constexpr auto get_at(usize index) const -> const value_type&
        {
            return _vector[index];
        }

        constexpr auto get_at(usize index) -> value_type&
        {
            return _vector[index];
        }

        constexpr auto get_iterator() const -> iterator_type
        {
            return iterator_type(_vector.data());
        }

        constexpr auto get_iterator_at(usize index) const -> iterator_type
        {
            return iterator_type(_vector.data() + index);
        }

        constexpr auto get_iterator_end() const -> iterator_end_type
        {
            return iterator_end_type(_vector.data() + _vector.size());
        }

        constexpr auto get_iterator() -> mut_iterator_type
        {
            return mut_iterator_type(_vector.data());
        }

        constexpr auto get_iterator_at(usize index) -> mut_iterator_type
        {
            return mut_iterator_type(_vector.data() + index);
        }

        constexpr auto get_iterator_end() -> mut_iterator_end_type
        {
            return mut_iterator_end_type(_vector.data() + _vector.size());
        }

        template <typename other_iterator_type, typename other_iterator_end_type>
        constexpr auto assign_range(other_iterator_type it, other_iterator_end_type it_end)
        {
            _vector.assign(it, it_end);
        }

        template <typename... arg_types>
        constexpr auto emplace_at(usize index, arg_types&&... args)
        {
            _vector.emplace(_vector.begin() + index, forward<arg_types>(args)...);
        }

        template <typename other_iterator_type, typename other_iterator_end_type>
        constexpr auto insert_range_at(
            usize index, other_iterator_type it, other_iterator_end_type it_end) -> usize
        {
            usize old_size = _vector.size();
            _vector.insert(_vector.begin() + index, move(it), move(it_end));

            return _vector.size() - old_size;
        }

        template <typename... arg_types>
        constexpr auto emplace_first(arg_types&&... args)
        {
            emplace_at(0);
        }

        template <typename other_iterator_type, typename other_iterator_end_type>
        constexpr auto insert_range_first(
            other_iterator_type it, other_iterator_end_type it_end) -> usize
        {
            return insert_range_at(0, move(it), move(it_end));
        }

        template <typename... arg_types>
        constexpr auto emplace_last(arg_types&&... args)
        {
            _vector.emplace_back(forward<arg_types>(args)...);
        }

        template <typename other_iterator_type, typename other_iterator_end_type>
        constexpr auto insert_range_last(
            other_iterator_type it, other_iterator_end_type it_end) -> usize
        {
            return insert_range_at(get_count(), move(it), move(it_end));
        }

        constexpr auto remove_at(usize index)
        {
            _vector.erase(_vector.begin() + index);
        }

        constexpr auto remove_first(usize index)
        {
            _vector.erase(_vector.begin());
        }

        constexpr auto remove_last(usize index)
        {
            if (_vector.size() > 0)
                _vector.erase(_vector.begin() + _vector.size() - 1);
        }

        constexpr auto remove_range(usize begin, usize count)
        {
            auto vector_begin = _vector.begin() + begin;
            _vector.erase(vector_begin, vector_begin + count);
        }

        constexpr auto remove_all()
        {
            _vector.clear();
        }

        constexpr auto reserve(usize count)
        {
            _vector.reserve(count);
        }

        constexpr auto reserve_more(usize count)
        {
            _vector.reserve(_vector.size() + count);
        }

        // todo: implement this.
        constexpr auto release_unused_mem() {}

        constexpr auto get_capacity() const -> usize
        {
            return _vector.capacity();
        }

        constexpr auto get_count() const -> usize
        {
            return _vector.size();
        }

        constexpr auto get_data() const -> const value_type*
        {
            return _vector.data();
        }

        constexpr auto get_data() -> value_type*
        {
            return _vector.data();
        }

        constexpr auto get_allocator() const -> const allocator_type&
        {
            return _vector.alloc();
        }

        constexpr auto is_empty() const -> bool
        {
            return _vector.count() == 0;
        }

        constexpr auto is_index_in_range(usize index) const -> bool
        {
            return index < _vector.size();
        }

        constexpr auto is_index_in_range_or_end(usize index) const -> bool
        {
            return index <= _vector.size();
        }

        constexpr auto is_iterator_valid(iterator_type it) const -> bool
        {
            return true;
        }

        constexpr auto get_index_for_iterator(iterator_type it) const -> usize
        {
            isize index = &*it - _vector.data();
            return index < 0 ? nums::get_max_usize() : index;
        }

        constexpr auto is_iterator_in_range(iterator_type it) const -> bool
        {
            return get_index_for_iterator(it) < _vector.size();
        }

        constexpr auto is_iterator_in_range_or_end(iterator_type it) const -> bool
        {
            return get_index_for_iterator(it) <= _vector.size();
        }

    private:
        std::vector<value_type> _vector;
    };
}
