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
        ImGui::PushID("Shaders:");

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

        ImGui::PopID();
    }
    if (ImGui::CollapsingHeader("Materials:", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PushID("Materials:");

        auto& materialMap = Renderer_GetMaterials();
        for (const auto& materialPair : materialMap)
        {
            Material& mat = *materialPair.second;

            if (ImGui::TreeNode(materialPair.first.c_str()))
            {
                ImGui::Text("Shader Name: %s", mat.GetShader()->GetName().c_str());

                if (ImGui::TreeNode("Material Properties"))
                {
                    for (const auto& obj : mat.GetProperties())
                    {
                        if (ImGui::TreeNode(obj.name.c_str()))
                        {
                            ImGui::Text("Property Format: %i", (int)obj.propertyFormat);

                            switch (obj.propertyFormat)
                            {
                                default:
                                    ImGui::Text("Unknown Format");
                                    break;

                                case MaterialFormat::Float32:
                                    ImGui::Text("Value: %f", *static_cast<float*>(obj.dataPtr));
                                    break;
                                case MaterialFormat::Float32x3:
                                    ImGui::Text("Value0: %f", static_cast<float*>(obj.dataPtr)[0]);
                                    ImGui::Text("Value1: %f", static_cast<float*>(obj.dataPtr)[1]);
                                    ImGui::Text("Value2: %f", static_cast<float*>(obj.dataPtr)[2]);
                                    break;
                                case MaterialFormat::Float32x4:
                                    ImGui::Text("Value0: %f", static_cast<float*>(obj.dataPtr)[0]);
                                    ImGui::Text("Value1: %f", static_cast<float*>(obj.dataPtr)[1]);
                                    ImGui::Text("Value2: %f", static_cast<float*>(obj.dataPtr)[2]);
                                    ImGui::Text("Value3: %f", static_cast<float*>(obj.dataPtr)[3]);
                                    break;
                                case MaterialFormat::Float32x4x4:
                                    //ImGui::Text("Value0_0" ": %f", static_cast<float**>(obj.dataPtr)[0][0]);
                                    //ImGui::Text("Value1_0" ": %f", static_cast<float**>(obj.dataPtr)[1][0]);
                                    //ImGui::Text("Value2_0" ": %f", static_cast<float**>(obj.dataPtr)[2][0]);
                                    //ImGui::Text("Value3_0" ": %f", static_cast<float**>(obj.dataPtr)[3][0]);

                                    for (int i = 0; i < 4; ++i)
                                        for (int j = 0; j < 4; ++j)
                                            ImGui::Text("Value%d_%d: %f", i, j, static_cast<float**>(obj.dataPtr)[i][j]);
                                    break;

                                case MaterialFormat::Int32:
                                    ImGui::Text("Value: %i", *static_cast<int32*>(obj.dataPtr));
                                    break;
                                case MaterialFormat::UInt32:
                                    ImGui::Text("Value: %u", *static_cast<uint32*>(obj.dataPtr));
                                    break;
                            }

                            ImGui::TreePop();
                        }
                    }

                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
        }

        ImGui::PopID();
    }
    if (ImGui::CollapsingHeader("Stats:", ImGuiTreeNodeFlags_DefaultOpen))
    {

    }

    ImGui::End();
}