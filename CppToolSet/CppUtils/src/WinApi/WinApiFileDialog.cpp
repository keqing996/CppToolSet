
#include "../../include/WinApi/WindowsPlatform.h"
#include "../../include/WinApi/WinApiFileDialog.h"

#include <atlbase.h>
#include <shlobj.h>

namespace WinApi::FileDialog
{
    std::wstring OpenFile()
    {
        TCHAR path[MAX_PATH];
        BROWSEINFO bi = { 0 };
        bi.lpszTitle = _T("ss");
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        bi.lpfn = nullptr;
        bi.lParam = (LPARAM)path;

        LPITEMIDLIST pItemIdList = SHBrowseForFolder(&bi );
        if (pItemIdList != nullptr)
        {
            SHGetPathFromIDList(pItemIdList, path);

            IMalloc* pIMalloc = nullptr;
            if (SUCCEEDED(SHGetMalloc(&pIMalloc)))
            {
                pIMalloc->Free(pItemIdList);
                pIMalloc->Release();
            }
        }

        return std::wstring{ path };
    }
}