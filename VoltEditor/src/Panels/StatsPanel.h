#pragma once
#include <cstdint>
#include <vector>

namespace Volt
{
    class CStatsPanel
    {
    public:
        void OnUIRender();
    protected:
        void Add(float sample);
        void DisplaceSamples(uint32_t count);
        float CalculateAverage() const;
    private:
        int32_t m_maxSamples {512};
        std::vector<float> m_samples;
    };
}
