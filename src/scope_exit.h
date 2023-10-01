// SPDX-License-Identifier: MIT
// Copyright(C) 2023 Edouard Gomez

#pragma once

namespace
{

/** RAII scope guard */
template <typename T> struct scope_exit
{
  public:
    /** ctor
     * @param[in] f Functor to execute on scope exit
     */
    explicit scope_exit(T&& f) : m_functor(std::forward<T>(f))
    {
    }
    ~scope_exit()
    {
        if (!m_disabled)
        {
            m_functor();
        }
    }
    void
    drop() const
    {
        m_disabled = true;
    }

    /* disable copy, makes no sense */
    scope_exit(scope_exit const&) = delete;
    scope_exit&
    operator=(scope_exit const&) = delete;

  private:
    T m_functor;                     /**< functor executed on scope exit through the dtor */
    mutable bool m_disabled = false; /**< the scope exit object may be disabled by the user */
};

/** scope_exit creator
 * @param[in] f Functor to execute on scope exit
 */
template <typename T> scope_exit<T>
make_scope_exit(T&& f)
{
    return scope_exit<T>(std::forward<T>(f));
}

#define DETAIL_SCOPE_EXIT_CONCATX(a, b) a##b
#define DETAIL_SCOPE_EXIT_CONCAT(a, b) DETAIL_SCOPE_EXIT_CONCATX(a, b)

/** Macro helper to define a named scope_exit object. Use this if it is
 * required to drop the guard eventually in a sort of init like codepath
 * reaching its ending, and willing to disarm all guards defined along
 * the codepath to handle its cleanup
 * @param[in] varname Variable name to be defined
 * @param[in] f Functor to be executed on scope exit
 */
#define SCOPE_EXIT_NAMED(varname, f) const auto& varname = make_scope_exit(f)

/** Macro helper to define a scope_exit object. Use this if the scope_exit
 * object is not to be disarmed and its name is of no value.
 * Its variable name will automatically be set to scope_exit_ ## __LINE__
 * to avoid any conflicts within a same compilation unit.
 * @param[in] varname Variable name to be defined
 * @param[in] f Functor to be executed on scope exit
 */
#define SCOPE_EXIT(f) SCOPE_EXIT_NAMED(DETAIL_SCOPE_EXIT_CONCAT(scope_exit_, __LINE__), f)

} // namespace
