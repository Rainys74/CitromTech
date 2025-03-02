#include "NativeFileDialog.h"
#include "CitromAssert.h"
#include "Profiling/Profiler.h"

#include "nfd.h"

namespace Citrom
{
    NativeFileDialog::NativeFileDialog()
    {
        CT_PROFILE_SCOPE("NFD_Init()");
        NFD_Init();
    }

    NativeFileDialog::~NativeFileDialog()
    {
        CT_PROFILE_SCOPE("NFD_Quit()");
        NFD_Quit();
    }

    std::string NativeFileDialog::OpenFile(const NativeFileDialogFilter filters[], const uint32 filterCount)
    {
        std::string path;

        nfdu8char_t* outPath;
        //nfdu8filteritem_t nfdFilters[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
        CTL::DArray<nfdu8filteritem_t> nfdFilters(filterCount);
        for (uint32 i = 0; i < filterCount; i++)
        {
            nfdFilters.PushBack(nfdu8filteritem_t{filters[i].name.c_str(), filters[i].fileNamesSpec.c_str()});
        }
        nfdopendialogu8args_t args = { 0 };
        args.filterList = nfdFilters.Data();
        args.filterCount = nfdFilters.Count();
        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            path = outPath;
            NFD_FreePathU8(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            return std::string();
        }
        else
        {
            CT_CORE_FATAL("nativefiledialog-extended (NFD) failed! {}", NFD_GetError());
            CT_CORE_ASSERT(false, "NFD failure!");
        }

        return path;
    }

    std::string NativeFileDialog::SaveFile(const NativeFileDialogFilter filters[], const uint32 filterCount)
    {
        std::string path;

        nfdu8char_t* outPath;
        //nfdu8filteritem_t nfdFilters[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
        CTL::DArray<nfdu8filteritem_t> nfdFilters(filterCount);
        for (uint32 i = 0; i < filterCount; i++)
        {
            nfdFilters.PushBack(nfdu8filteritem_t{ filters[i].name.c_str(), filters[i].fileNamesSpec.c_str() });
        }
        nfdsavedialogu8args_t args = { 0 };
        args.filterList = nfdFilters.Data();
        args.filterCount = nfdFilters.Count();
        nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            path = outPath;
            NFD_FreePathU8(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            return std::string();
        }
        else
        {
            CT_CORE_FATAL("nativefiledialog-extended (NFD) failed! {}", NFD_GetError());
            CT_CORE_ASSERT(false, "NFD failure!");
        }

        return path;
    }
}