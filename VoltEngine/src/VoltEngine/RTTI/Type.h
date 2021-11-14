#pragma once
#include <string>

namespace Volt
{
    class CType
    {
    public:
        CType(const std::string& name = {}, const CType* baseType = nullptr);
        ~CType() = default;

        bool IsSubTypeOf(const CType& type) const;

        template <typename T>
        bool IsSubTypeOf() const
        {
            return IsSubTypeOf(T::GetStaticType());
        }

        bool operator ==(const CType& other) const { return this == &other; }
        bool operator ==(CType& other) { return this == &other; }

        const std::string& GetName() const { return m_name; }

    private:
        std::string m_name;
        const CType* m_baseType;
    };
}
