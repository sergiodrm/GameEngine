#pragma once
#include <string>

namespace Volt
{
    class CLayer
    {
    public:
        CLayer(const std::string& debugName = "Layer")
            : m_debugName(debugName) {}

        virtual ~CLayer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float elapsedSeconds) {}
        virtual void OnUIRender() {}
        virtual bool OnEvent(class CEvent& e) { return false; }

        const std::string& GetName() const { return m_debugName; }

    private:
        std::string m_debugName;
    };
}
