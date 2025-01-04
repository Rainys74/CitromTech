#pragma once

#include "Core.h"

#include "CTL/DArray.h"
#include "CTL/String.h"

namespace Citrom
{
	struct ImGuiStyleObj
	{
		std::string name;
		ImGuiStyles::CallbackFN callback;
	};

	class ImGuiStyles
	{
	public:
		using CallbackFN = void(*)(void);

		static void Push(const ImGuiStyleObj& style);

		static CTL::DArray<ImGuiStyleObj>& GetStyles();
	private:
		static CTL::DArray<ImGuiStyleObj> s_ImStyles;
	};
}