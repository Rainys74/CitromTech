#pragma once

#include "Core.h"

#include "CTL/DArray.h"
#include "CTL/String.h"

namespace Citrom
{
	using ImGuiStyleCallbackFN = void(*)(void);

	struct ImGuiStyleObj
	{
		std::string name;
		ImGuiStyleCallbackFN callback;
	};

	class ImGuiStyles
	{
	public:
		static void Push(const ImGuiStyleObj& style);

		static CTL::DArray<ImGuiStyleObj>& GetStyles();
	private:
		static CTL::DArray<ImGuiStyleObj> s_ImStyles;
	};
}