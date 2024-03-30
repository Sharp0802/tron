#ifndef OOP_H
#define OOP_H

#include "pch.h"

#define setter_(p) void set__##p(decltype(p) value)
#define getter_(p) decltype(p) get__##p()

#define setter_decl_(t, p) void t::set__##p(decltype(t::p) value)
#define getter_decl_(t, p) decltype(t::p) t::get__##p()

#define mutable_(t, p)                               \
    __declspec(property(get=get__##p, put=set__##p)) \
    t p;                                             \
    setter_(p);                                      \
    getter_(p)

#define immutable_(t, p)               \
    __declspec(property(get=get__##p)) \
    t p;                               \
    getter_(p)

#define static_immutable_(t, p)        \
    __declspec(property(get=get__##p)) \
    static t p;                        \
    static getter_(p)

using TypeId = std::uint64_t;

template<typename T>
constexpr TypeId GetType()
{
    return typeid(T).hash_code();
}

#endif //OOP_H
