#include "StatsPanel.h"

#include "imgui.h"
#include "VoltEngine/Core/Application.h"
#include "VoltEngine/Core/Time.h"
#include "VoltEngine/Renderer/Renderer2D.h"

namespace Volt
{
    void CStatsPanel::OnUIRender()
    {
        ImGui::Begin("Stats");

        const CTime& timer = CApplication::Get().GetTimer();
        if (ImGui::TreeNodeEx("Performance"))
        {
            const float ms = timer.GetElapsedTimeMilliseconds();
            Add(ms);
            ImGui::Text("Frame time: %.4f ms", ms);
            if (ImGui::TreeNodeEx("Graph"))
            {
                ImGui::PlotLines("Frame time (ms)", m_samples.data(),
                                 m_samples.size(), 0, nullptr, 0.f, 40.f);
                ImGui::SliderInt("Samples", &m_maxSamples, 10, 1000);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        const SRenderer2DStats stats = CRenderer2D::GetStats();
        if (ImGui::TreeNodeEx("Render"))
        {
            ImGui::BulletText("Draw calls: %d", stats.DrawCalls);
            ImGui::BulletText("Quad count: %d", stats.QuadCount);
            ImGui::BulletText("Vertices:   %d", stats.GetVerticesCount());
            ImGui::BulletText("Indices:    %d", stats.GetIndicesCount());
            ImGui::BulletText("Triangles:  %d", stats.GetTrianglesCount());
            ImGui::TreePop();
        }
        ImGui::End();
    }

    void CStatsPanel::Add(float sample)
    {
        if (m_samples.size() < m_maxSamples)
        {
            m_samples.push_back(sample);
        }
        else
        {
            DisplaceSamples(m_samples.size() - m_maxSamples + 1u);
            m_samples[m_samples.size() - 1] = sample;
        }
    }


    void CStatsPanel::DisplaceSamples(uint32_t count)
    {
        for (uint32_t index = count; index < m_samples.size(); ++index)
        {
            m_samples[index - count] = m_samples[index];
        }
    }

    float CStatsPanel::CalculateAverage() const
    {
        float sum = 0.f;
        for (float it : m_samples)
        {
            sum += it;
        }
        return sum / m_samples.size() < m_maxSamples
                   ? static_cast<float>(m_samples.size())
                   : static_cast<float>(m_maxSamples);
    }
}
