#pragma once

#include "Type.h"
#include "TypeCast.h"

#define DECLARE_BASE_CLASS() \
    public: \
    static const CType& GetStaticType() { static const CType type; return type; } \
    private:

#define DECLARE_DERIVED_CLASS(BaseClass) \
    public: \
    static const CType& GetStaticType() { static const CType type(BaseClass); return type; } \
    private:
