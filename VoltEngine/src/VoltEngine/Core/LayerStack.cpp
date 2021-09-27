#include "LayerStack.h"

#include "Layer.h"

namespace Volt
{
    CLayerStack::~CLayerStack()
    {
        for (CLayer* it : m_layers)
        {
            delete it;
        }
    }

    void CLayerStack::PushLayer(class CLayer* layer)
    {
        m_layers.emplace(begin() + m_layerIndex, layer);
        ++m_layerIndex;
    }

    void CLayerStack::PushOverlay(class CLayer* overlay)
    {
        m_layers.emplace_back(overlay);
    }

    void CLayerStack::PopLayer(class CLayer* layer)
    {
        LayerIterator it = std::find(begin(), end(), layer);
        if (it != end())
        {
            m_layers.erase(it);
            --m_layerIndex;
        }
    }

    void CLayerStack::PopOverlay(class CLayer* overlay)
    {
        LayerConstIterator it = std::find(begin(), end(), overlay);
        if (it != end())
        {
            m_layers.erase(it);
        }
    }
}
