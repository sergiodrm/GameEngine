#pragma once
#include <vector>

#include "Core.h"

namespace Volt
{
    class CLayerStack
    {
        using LayerIterator = std::vector<class CLayer*>::iterator;
        using LayerConstIterator = std::vector<class CLayer*>::const_iterator;
    public:
        CLayerStack() = default;
        ~CLayerStack();

        void PushLayer(class CLayer* layer);
        void PushOverlay(class CLayer* overlay);

        void PopLayer(class CLayer* layer);
        void PopOverlay(class CLayer* overlay);

        LayerIterator begin() { return m_layers.begin(); }
        LayerConstIterator begin() const { return m_layers.begin(); }
        LayerIterator end() { return m_layers.end(); }
        LayerConstIterator end() const { return m_layers.end(); }

    private:
        std::vector<class CLayer*> m_layers;
        uint32_t m_layerIndex {0};
    };
}
