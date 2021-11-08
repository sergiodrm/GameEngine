#pragma once

#include "Type.h"
#include "TypeCast.h"

#define DECLARE_BASE_CLASS() \
    public: \
    static const CType& GetStaticType() { static const CType type; return type; } \
    virtual const CType& GetDynamicType() const { return GetStaticType(); } \
    private:

#define DECLARE_DERIVED_CLASS(BaseClass) \
    public: \
    static const CType& GetStaticType() { static const CType type(BaseClass::GetStaticType()); return type; } \
    virtual const CType& GetDynamicType() const override { return GetStaticType(); } \
    private:
