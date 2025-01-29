#pragma once

#include "CTL/String.h"

namespace Citrom
{
	struct NativeFileDialogFilter
	{
		std::string name;
		std::string fileNamesSpec;
	};
	class NativeFileDialog
	{
	public:
		NativeFileDialog();
		~NativeFileDialog();

		std::string OpenFile(const NativeFileDialogFilter filters[], const uint32 filterCount);
		std::string SaveFile(const NativeFileDialogFilter filters[], const uint32 filterCount);
	};
}