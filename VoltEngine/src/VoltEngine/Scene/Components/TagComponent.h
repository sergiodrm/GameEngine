#pragma once
#include <string>

#include "VoltEngine/Scene/Component.h"

namespace Volt
{
    class CTagComponent : public CComponent
    {
    DECLARE_DERIVED_CLASS(CTagComponent, CComponent)
    public:
        CTagComponent(const std::string& tag)
            : m_tag(tag) {}

        const std::string& GetTag() const { return m_tag; }

    private:
        std::string m_tag;
    };
}
