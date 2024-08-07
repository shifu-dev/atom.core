export module atom_core:lock_guard;

import :lockable;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// locks the lock on construction and unlocks at destruction. this_type is done to guarantee
    /// exception safety.
    /// --------------------------------------------------------------------------------------------
    export template <typename lockable_type>
    class lock_guard
    {
        static_assert(is_lockable<lockable_type>);

    public:
        /// ----------------------------------------------------------------------------------------
        /// constructor. locks the lock.
        ///
        /// @param[in] lock lockable to lock.
        ///
        /// @throws unkown_exception exception thrown by {lock.lock()}.
        /// ----------------------------------------------------------------------------------------
        lock_guard(lockable_type& lock)
            : _lock(lock)
        {
            _lock.lock();
        }

        /// ----------------------------------------------------------------------------------------
        /// destructor. unlocks the lock.
        ///
        /// @throws unkown_exception exception thrown by {lock.lock()}.
        /// ----------------------------------------------------------------------------------------
        ~lock_guard()
        {
            _lock.unlock();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// lockable object.
        /// ----------------------------------------------------------------------------------------
        lockable_type& _lock;
    };
}
