#include "StatsPanel.h"

#include "imgui.h"
#include "VoltEngine/Core/Application.h"
#include "VoltEngine/Core/Time.h"
#include "VoltEngine/Renderer/Renderer2D.h"
#include "VoltEngine/Renderer/Renderer3D.h"

namespace Volt
{
    static void BeginDoubleColumn()
    {
        ImGui::Columns(2);
    }

    static void EndDoubleColumn()
    {
        ImGui::Columns(1);
    }

    static void PrintTextValue(const std::string& text, float value, const char* valueFormat = "%.2f")
    {
        ImGui::Text("%s", text.c_str());
        ImGui::NextColumn();
        ImGui::Text(valueFormat, value);
        ImGui::NextColumn();
    }

    static void PrintTextValue(const std::string& text, uint32_t value, const char* valueFormat = "%d")
    {
        ImGui::Text("%s", text.c_str());
        ImGui::NextColumn();
        ImGui::Text(valueFormat, value);
        ImGui::NextColumn();
    }

    void CStatsPanel::OnUIRender()
    {
        ImGui::Begin("Stats");

        const CTime& timer = CApplication::Get().GetTimer();
        if (ImGui::TreeNodeEx("Performance", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const float ms = timer.GetElapsedTimeMilliseconds();
            Add(ms);

            BeginDoubleColumn();
            PrintTextValue("FPS", 1000.f / ms);
            PrintTextValue("Frame time", ms, "%.4f ms");
            EndDoubleColumn();

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
            if (ImGui::TreeNodeEx("3D", ImGuiTreeNodeFlags_DefaultOpen))
            {
                const SRenderStats& stats = CRenderer3D::GetStats();
                BeginDoubleColumn();
                PrintTextValue("Batch size", stats.BatchSize, "%d triangles");
                PrintTextValue("Draw calls", stats.DrawCallCount);
                PrintTextValue("Vertices", stats.VertexCount);
                PrintTextValue("Indices", stats.GetIndexCount());
                PrintTextValue("Triangles", stats.TriangleCount);
                PrintTextValue("Vertex used memory", stats.GetVertexUsedMemory(), "%d bytes");
                PrintTextValue("Index used memory", stats.GetIndexUsedMemory(), "%d bytes");
                EndDoubleColumn();
                ImGui::TreePop();
            }
            if (ImGui::TreeNodeEx("2D"))
            {
                const SRenderer2DStats& stats = CRenderer2D::GetStats();
                BeginDoubleColumn();
                PrintTextValue("Draw calls", stats.DrawCalls);
                PrintTextValue("Quad count", stats.QuadCount);
                PrintTextValue("Vertices", stats.GetVerticesCount());
                PrintTextValue("Indices", stats.GetIndicesCount());
                PrintTextValue("Triangles", stats.GetTrianglesCount());
                EndDoubleColumn();
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
