#include "SettingsPanel.h"

#include <imgui/imgui.h>

#include "Eternal/Core/Timestep.h"

namespace Eternal {

    void SettingsPanel::OnImGuiRender()
	{
        ImGui::Begin("Settings");

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Batch Renderer Stats:");

        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::SameLine();
        ImGui::Text("Quad Count: %d", stats.QuadCount);
        ImGui::SameLine();
        ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
        ImGui::SameLine();
        ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
        //For renderer vendor etc. include graphics context -> see OpenGL context
        ImGui::Text("Frame Time: %d", 1000 / (int)m_timestep.GetMilliseconds());

        ImGui::End();
	}
}