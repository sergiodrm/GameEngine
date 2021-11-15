#include "StatsPanel.h"

#include "imgui.h"
#include "VoltEngine/Core/Application.h"
#include "VoltEngine/Core/Time.h"
#include "VoltEngine/Renderer/Renderer2D.h"
#include "VoltEngine/Renderer/Renderer3D.h"

namespace Volt
{
    void CStatsPanel::OnUIRender()
    {
        ImGui::Begin("Stats");

        const CTime& timer = CApplication::Get().GetTimer();
        if (ImGui::TreeNodeEx("Performance", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const float ms = timer.GetElapsedTimeMilliseconds();
            Add(ms);
            ImGui::Text("FPS: %.2f", 1000.f / ms);
            ImGui::Text("Frame time: %.4f ms", ms);
            if (ImGui::TreeNodeEx("Graph"))
            {
                char overlay[32];
                sprintf_s(overlay, "Average : %.4f ms", CalculateAverage());
                const float width = ImGui::GetContentRegionAvailWidth() - 10.f;
                ImGui::PlotLines("Frame time (ms)", m_samples.data(),
                                 m_samples.size(), 0, overlay, 0.f, 40.f, {width, 64.f});
                ImGui::SliderInt("Samples", &m_maxSamples, 10, 1000);
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }


        if (ImGui::TreeNodeEx("Render", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::TreeNodeEx("3D"))
            {
                const SRenderStats& stats = CRenderer3D::GetStats();
                ImGui::Text("Draw calls: %d", stats.DrawCallCount);
                ImGui::Text("Vertices:   %d", stats.VertexCount);
                ImGui::Text("Indices:    %d", stats.GetIndexCount());
                ImGui::Text("Triangles:  %d", stats.TriangleCount);
                ImGui::Text("Vertex used memory: %d bytes", stats.GetVertexUsedMemory());
                ImGui::Text("Index used memory: %d bytes", stats.GetIndexUsedMemory());
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("2D"))
            {
                const SRenderer2DStats& stats = CRenderer2D::GetStats();
                ImGui::Text("Draw calls: %d", stats.DrawCalls);
                ImGui::Text("Quad count: %d", stats.QuadCount);
                ImGui::Text("Vertices:   %d", stats.GetVerticesCount());
                ImGui::Text("Indices:    %d", stats.GetIndicesCount());
                ImGui::Text("Triangles:  %d", stats.GetTrianglesCount());
                ImGui::TreePop();
            }
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
        return sum / static_cast<float>(m_samples.size());
    }
}
