#include "Type.h"

namespace Volt
{
    CType::CType(const CType* baseType)
        : m_baseType(baseType) {}

    bool CType::IsSubTypeOf(const CType& type) const
    {
        const CType* current = this;
        while (current)
        {
            if (current == &type)
            {
                return true;
            }
            current = current->m_baseType;
        }
        return false;
    }
}
