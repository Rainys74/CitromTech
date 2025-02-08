#include "ImGuiToolkit.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

namespace ImToolkit //ImPresets
{
    static void _SetupColumns(int count, bool allowResizing = false, float columnSize = 100.0f)
    {
        ImGui::Columns(count, nullptr, allowResizing);

        if (!allowResizing)
            ImGui::SetColumnWidth(0, columnSize);
    }
    // Label set-up for label-value elements
    static void _SetupLabel(const char* label, bool allowResizing = false, float columnSize = 100.0f)
    {
        _SetupColumns(2, allowResizing, columnSize);

        ImGui::Text(label);

        ImGui::NextColumn();
    }

    static bool _DrawControlElementDragButton(const char* label, const char* invisibleDragLabel, float* value, float speed, const ImVec2& buttonSize, const ImVec4& color, const ImVec4& highlightedColor)
    {
        bool modified = false;

        // Render Rect
        auto cursorPos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(cursorPos,
            ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y), ImGui::GetColorU32(color), ImGui::GetStyle().FrameRounding);

        // Calculate Text State
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos = ImVec2(
            cursorPos.x + (buttonSize.x - textSize.x) * 0.5f,  // Center X
            cursorPos.y + (buttonSize.y - textSize.y) * 0.5f   // Center Y
        );

        // Render Selectable
        ImGui::PushStyleColor(ImGuiCol_Header, color);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, highlightedColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, color);
        ImGui::Selectable(invisibleDragLabel, false, 0, buttonSize);
        if (ImGui::IsItemActive())
        {
            float dragDelta = ImGui::GetIO().MouseDelta.x * speed; // Drag affects X value
            value[0] += dragDelta;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        // Render Text
        ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), label);

        return modified;
    }
    static bool _DrawControlElementDragInput(const char* label, float* value)
    {
        bool modified = false;

        modified = ImGui::DragFloat(label, value, 1.0f, 0.0f, 0.0f, "%.2f"); // TODO: figure out how to format this: Unity, Godot, Unreal (.1 default), Torque3D: dynamic.
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            ImGui::SetKeyboardFocusHere(-1); // Set to the item before (DragFloat)

        return modified;
    }

    static bool _DrawVector1ControlElements() {}
    bool DrawVector3Control(const char* label, float values[3], float speed, float resetValue)
    {
        /* // Fresh Start
        bool modified = false;

        ImGui::PushID(label);

        ImGui::Columns(2);

        // Column 1
        ImGui::SetColumnWidth(0, 100.0f);
        ImGui::Text(label);

        ImGui::NextColumn();
        // --------

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // TODO: buttons and sliders should be close, each values should be spaced out.

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = ImVec2{lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        if (ImGui::Button("X", buttonSize));
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        modified = ImGui::DragFloat("##X", &values[0], speed);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
        if (ImGui::Button("Y", buttonSize));
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        modified = ImGui::DragFloat("##Y", &values[1], speed);
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
        if (ImGui::Button("Z", buttonSize));
        ImGui::PopStyleColor(2);

        ImGui::SameLine();
        modified = ImGui::DragFloat("##Z", &values[2], speed);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        return modified;
        */
        bool modified = false;

        ImGui::PushID(label);

        _SetupLabel(label);

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // TODO: buttons and sliders should be close, each values should be spaced out.

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = ImVec2{ lineHeight + 3.0f, lineHeight };

        // X
        modified |= _DrawControlElementDragButton("X", "##X_SelectableDrag", &values[0], speed, buttonSize, ImVec4(0.8f, 0.1f, 0.15f, 1.0f), ImVec4(0.9f, 0.2f, 0.2f, 1.0f));

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##X", &values[0]);
        ImGui::PopItemWidth();

        // Y
        ImGui::SameLine();
        modified |= _DrawControlElementDragButton("Y", "##Y_SelectableDrag", &values[1], speed, buttonSize, ImVec4(0.2f, 0.7f, 0.2f, 1.0f), ImVec4(0.3f, 0.8f, 0.3f, 1.0f));

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##Y", &values[1]);
        ImGui::PopItemWidth();

        // Z
        ImGui::SameLine();
        modified |= _DrawControlElementDragButton("Z", "##Z_SelectableDrag", &values[2], speed, buttonSize, ImVec4(0.1f, 0.25f, 0.8f, 1.0f), ImVec4(0.2f, 0.35f, 0.9f, 1.0f));

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##Z", &values[2]);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        return modified;
    }
    bool DrawVector4Control(const char* label, float values[4], float speed, float resetValue)
    {
        bool modified = false;

        ImGui::PushID(label);

        _SetupLabel(label);

        ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // TODO: buttons and sliders should be close, each values should be spaced out.

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = ImVec2{ lineHeight + 3.0f, lineHeight };

        // X
        modified |= _DrawControlElementDragButton("X", "##X_SelectableDrag", &values[0], speed, buttonSize, ImVec4(0.8f, 0.1f, 0.15f, 1.0f), ImVec4(0.9f, 0.2f, 0.2f, 1.0f));

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##X", &values[0]);
        ImGui::PopItemWidth();

        // Y
        ImGui::SameLine();
        modified |= _DrawControlElementDragButton("Y", "##Y_SelectableDrag", &values[1], speed, buttonSize, ImVec4(0.2f, 0.7f, 0.2f, 1.0f), ImVec4(0.3f, 0.8f, 0.3f, 1.0f));

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##Y", &values[1]);
        ImGui::PopItemWidth();

        // Z
        ImGui::SameLine();
        modified |= _DrawControlElementDragButton("Z", "##Z_SelectableDrag", &values[2], speed, buttonSize, ImVec4(0.1f, 0.25f, 0.8f, 1.0f), ImVec4(0.2f, 0.35f, 0.9f, 1.0f));

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##Z", &values[2]);
        ImGui::PopItemWidth();

        // W
        ImGui::SameLine();
        //modified |= _DrawControlElementDragButton("W", "##W_SelectableDrag", &values[3], speed, buttonSize, ImVec4(0.9f, 0.9f, 0.0f, 1.0f), ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // White or yellow?
        modified |= _DrawControlElementDragButton("W", "##W_SelectableDrag", &values[3], speed, buttonSize, ImVec4(0.7f, 0.7f, 0.0f, 1.0f), ImVec4(0.8f, 0.8f, 0.0f, 1.0f)); // White or yellow?

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##W", &values[3]);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        return modified;
    }
    bool DrawVector2Control(const char* label, float values[2], float speed)
    {
        bool modified = false;

        ImGui::PushID(label);

        _SetupLabel(label);

        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // TODO: buttons and sliders should be close, each values should be spaced out.

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = ImVec2{ lineHeight + 3.0f, lineHeight };

        // X
        modified |= _DrawControlElementDragButton("X", "##X_SelectableDrag", &values[0], speed, buttonSize, ImVec4(0.8f, 0.1f, 0.15f, 1.0f), ImVec4(0.9f, 0.2f, 0.2f, 1.0f));

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##X", &values[0]);
        ImGui::PopItemWidth();

        // Y
        ImGui::SameLine();
        modified |= _DrawControlElementDragButton("Y", "##Y_SelectableDrag", &values[1], speed, buttonSize, ImVec4(0.2f, 0.7f, 0.2f, 1.0f), ImVec4(0.3f, 0.8f, 0.3f, 1.0f));

        ImGui::SameLine();
        modified |= _DrawControlElementDragInput("##Y", &values[1]);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();

        return modified;
    }
    bool DrawFloatControl(const char* label, float* value, float speed, float min, float max, const char* format)
    {
        bool modified = false;

        ImGui::PushID(label);

        _SetupLabel(label); //, false, 200.0f);

        modified |= ImGui::DragFloat("##FLOAT", value, speed, min, max, format);

        ImGui::Columns(1);

        ImGui::PopID();

        return modified;
    }
}