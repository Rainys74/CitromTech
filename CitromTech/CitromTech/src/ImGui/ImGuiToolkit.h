#pragma once

#include "imgui.h"
#include "NativeFileDialog/NativeFileDialog.h"
#include "CTL/HashMap.h"
#include "CTL/HashSet.h"

#include <string>

#define CT_EDITOR_ENUMERABLE_DROPDOWN(NAME)

namespace ImToolkit //ImPresets
{
    bool DrawVector3Control(const char* label, float values[3], float speed = 0.25f, float resetValue = 0.0f);
    bool DrawVector4Control(const char* label, float values[4], float speed = 0.25f, float resetValue = 0.0f);
    bool DrawVector2Control(const char* label, float values[2], float speed = 0.25f);

    bool DrawFloatControl(const char* label, float* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f");

    bool PathPicker(const char* label, std::string* pathOutput, Citrom::NativeFileDialogFilter* filters = nullptr, size_t filterLength = 0); // InputLocation? BrowseField? BrowseInput? FileSelector? LocationInput? PathPicker? InputWithBrowse? BrowseField?

    bool DrawStringSetSelector(const char* label, std::string* textOutput, const CTL::StdStrHashSet& stringSet);

    template<typename T, const char* (*TToString)(T)>
    bool ComboEnum(const char* label, T& currentValue, const T* values, int valuesCount, const int valuesBegin = 0)
    {
        bool valueChanged = false;
        const char* previewValue = TToString(currentValue);

        if (ImGui::BeginCombo(label, previewValue))
        {
            for (int n = valuesBegin; n < valuesCount; n++)
            {
                bool selected = (currentValue == values[n]);
                if (ImGui::Selectable(TToString(values[n]), selected))
                {
                    currentValue = values[n];
                    valueChanged = true;
                }
                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        return valueChanged;
    }
}