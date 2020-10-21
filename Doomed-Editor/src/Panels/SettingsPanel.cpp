#include "SettingsPanel.h"

#include <imgui/imgui.h>

namespace Eternal {

	void SettingsPanel::OnImGuiRender()
	{
        ImGui::Begin("Settings");

        auto stats = Renderer2D::GetStats();
        ImGui::Text("Batch Renderer Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quad Count: %d", stats.QuadCount);
        ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

        ImGui::End();
	}
}