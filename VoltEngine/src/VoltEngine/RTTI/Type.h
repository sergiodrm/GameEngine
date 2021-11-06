#pragma once

namespace Volt
{
    class CType
    {
    public:
        CType(const CType* baseType = nullptr);
        ~CType() = default;

        bool IsSubTypeOf(const CType& type) const;

        template <typename T>
        bool IsSubTypeOf() const
        {
            return IsSubTypeOf(T::GetStaticClass());
        }

        bool operator ==(const CType& other) const { return this == &other; }
        bool operator ==(CType& other) { return this == &other; }

    private:
        const CType* m_baseType;
    };
}
