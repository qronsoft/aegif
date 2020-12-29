#pragma once

#include <type_traits>

namespace aegif
{
template <class OnExit>
class ScopeGuard
{
public:
    ScopeGuard(OnExit&& onExit) : onExit_(std::move(onExit)) {}
    ~ScopeGuard() { onExit_(); }

private:
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard(ScopeGuard&&)                 = delete;
    ScopeGuard& operator=(ScopeGuard&&) = delete;
    OnExit onExit_;
};
}
