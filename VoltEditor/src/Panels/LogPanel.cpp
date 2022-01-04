#include "LogPanel.h"
#include <imgui.h>
#include <vector>

#include "VoltEngine/Core/Log.h"

namespace Volt
{
    void CLogPanel::OnUIRender()
    {
        const std::vector<CLog::SLogRequest>& logs = CLog::GetLogRegister();

        ImGui::Begin("Logger", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
        {
            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                if (ImGui::MenuItem("Clear"))
                    ClearLog();
                ImGui::EndPopup();
            }

            ImGui::Columns(2);
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::NextColumn();
            ImGui::Text("%d/%d", static_cast<uint32_t>(logs.size()), CLog::GetLogRegisterMaxSize());
            ImGui::NextColumn();
            ImGui::Columns(1);

            ImGui::Separator();

            ImGui::BeginChild("LogSpace", {}, true, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            {
                static const ImVec4 colors[] = {
                        {0.8f, 0.7f, 0.2f, 1.f},
                        {0.8f, 0.2f, 0.2f, 1.f},
                        {0.8f, 0.2f, 0.2f, 1.f},
                        {0.8f, 0.8f, 0.8f, 1.f},
                        {0.2f, 0.8f, 0.2f, 1.f}
                    };
                for (const CLog::SLogRequest& it : logs)
                {
                    ImGui::TextColored(colors[static_cast<uint32_t>(it.LogType)], "%s", it.Log.c_str());
                }
            }
            ImGui::PopStyleVar();
            ImGui::EndChild();
        }

        ImGui::End();
    }

    void CLogPanel::ClearLog() {}
}
