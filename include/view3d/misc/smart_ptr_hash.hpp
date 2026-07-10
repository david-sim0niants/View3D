#pragma once

#include <unordered_set>

namespace view3d::misc {

template<typename SmartPtr, typename T>
struct SmartPtrHash {
    using is_transparent = void;

    size_t operator()(const SmartPtr& p) const
    {
        return std::hash<const T*>{}(p.get());
    }

    size_t operator()(const T* p) const
    {
        return std::hash<const T*>{}(p);
    }
};

template<typename SmartPtr, typename T>
struct SmartPtrEq {
    using is_transparent = void;

    bool operator()(const SmartPtr& a, const SmartPtr& b) const
    {
        return a.get() == b.get();
    }
    bool operator()(const SmartPtr& a, const T* b) const
    {
        return a.get() == b;
    }
    bool operator()(const T* a, const SmartPtr& b) const
    {
        return a == b.get();
    }
};

template<template<typename> class SmartPtr, typename T>
using SmartPtrSet =
    std::unordered_set<SmartPtr<T>, SmartPtrHash<SmartPtr<T>, T>,
                       SmartPtrEq<SmartPtr<T>, T>>;

} // namespace view3d::misc
