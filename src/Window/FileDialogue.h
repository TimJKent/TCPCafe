#pragma once

#include <nfd.h>


namespace FileDialogue
{
    void Init()
    {
        NFD_Init();
    }

    void Cleanup()
    {
        NFD_Quit();
    }

    std::string GetPathForOpen()
    {
        std::string output = "";
        nfdu8char_t* outPath;
        nfdu8filteritem_t filters[1] = {{ "TCPCafe File", "cafe" }};
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;
        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            if(outPath != nullptr)
            {
                output = std::string(outPath);
            }
            NFD_FreePathU8(outPath);
        }
        
        return output;
    }

    std::string GetPathForSave()
    {
        std::string output = "";
        nfdu8char_t* outPath;
        nfdu8filteritem_t filters[1] = {{ "TCPCafe File", "cafe" }};
        nfdsavedialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;
        args.defaultName = "NetworkNodes.cafe";
        nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            if(outPath != nullptr)
            {
                output = std::string(outPath);
            }
            NFD_FreePathU8(outPath);
        }
        
        return output;
    }
}

