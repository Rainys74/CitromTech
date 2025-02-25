#include "RendererWindow.h"
#include "Renderer/Renderer.h"

#include "imgui.h"

using namespace Citrom;

void RendererWindow::ImGuiDraw(bool* showWindow)
{
    if (showWindow && !(*showWindow))
        return;

    ImGui::Begin("Renderer", showWindow);

    if (ImGui::Button("Retranspile Shaders To Cache")) // Recompiles all shaders (cache)
    {
    }
    if (ImGui::BeginPopupContextItem())
    {
        // TODO: maybe separate hlslcc, glslcc, dxcompiler etc.?
        if (ImGui::Button("Clear Shader Cache"));
        ImGui::SetItemTooltip("Clears all the cached shaders from ShaderCache/");
        if (ImGui::Button("Prepare Shaders"));
        ImGui::SetItemTooltip("Uses glslcc to transpile glsl shaders to other languages");
        if (ImGui::Button("Compile Shaders"));
        ImGui::SetItemTooltip("Compiles shaders for specific api's such as to DXBC (HLSL5) for DX11, uses HLSLcc to transpile DXBC binaries to other languages.");

        ImGui::EndPopup();
    }

    if (ImGui::CollapsingHeader("Shaders:", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto& shaderMap = Renderer_GetShaders();
        for (const auto& shaderPair : shaderMap)
        {
            if (ImGui::TreeNode(shaderPair.first.c_str()))
            {
                if (ImGui::Button("Reload Shader"))
                    Renderer_RecompileShader(shaderPair.first);

                ImGui::TreePop();
            }
        }
    }
    if (ImGui::CollapsingHeader("Materials:", ImGuiTreeNodeFlags_DefaultOpen))
    {

    }
    if (ImGui::CollapsingHeader("Stats:", ImGuiTreeNodeFlags_DefaultOpen))
    {

    }

    ImGui::End();
}