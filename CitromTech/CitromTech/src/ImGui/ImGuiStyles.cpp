#include "ImGuiStyles.h"

#include "CitromAssert.h"

namespace Citrom
{
    CTL::DArray<ImGuiStyleObj> ImGuiStyles::s_ImStyles;

    void ImGuiStyles::Push(const ImGuiStyleObj& style)
    {
        CT_CORE_ASSERT(style.callback, "ImGui Style callback is null!");

        s_ImStyles.PushBack(style);
    }

    CTL::DArray<ImGuiStyleObj>& ImGuiStyles::GetStyles()
    {
        return s_ImStyles;
    }
}