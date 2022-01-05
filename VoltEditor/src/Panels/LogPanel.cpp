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
            static bool autoScroll = true;

            // Options menu
            ImGui::Columns(2);
            if (ImGui::Button("Clear"))
                ClearLog();
            ImGui::SameLine();
            ImGui::Checkbox("Auto scroll", &autoScroll);
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

                if (autoScroll && ImGui::GetScrollY() / ImGui::GetScrollMaxY() < 0.95f)
                    ImGui::SetScrollHereY(1.f);
            }
            ImGui::PopStyleVar();
            ImGui::EndChild();
        }

        ImGui::End();
    }

    void CLogPanel::ClearLog()
    {
        VOLT_LOG(Error, "Clearing log...");
    }
}
