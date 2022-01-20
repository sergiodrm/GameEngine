#pragma once

#include "Type.h"
#include "TypeCast.h"

#define DECLARE_BASE_CLASS(Class) \
    public: \
    inline static const CType& GetStaticType() { static const CType type(#Class); return type; } \
    inline virtual const CType& GetDynamicType() const { return GetStaticType(); } \
    template <typename T> \
    inline bool IsA() const \
    { \
        const CType& type = GetDynamicType(); \
        return type.IsSubTypeOf<T>(); \
    } \
    inline static bool IsA(const CType& type) {return type.IsSubTypeOf(GetStaticType()); } \
    const std::string& GetTypeName() const { return GetDynamicType().GetName(); } \
    private:

#define DECLARE_DERIVED_CLASS(Class, BaseClass) \
    public: \
    inline static const CType& GetStaticType() { static const CType type(#Class, &BaseClass::GetStaticType()); return type; } \
    inline virtual const CType& GetDynamicType() const override { return GetStaticType(); } \
    private:
