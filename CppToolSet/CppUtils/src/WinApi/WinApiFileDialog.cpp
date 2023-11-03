
#include "../../include/WinApi/WindowsPlatform.h"
#include "../../include/WinApi/WinApiFileDialog.h"

#include <shlobj.h>
#include <shobjidl.h>

namespace WinApi::FileDialog
{
    std::optional<std::wstring> OpenFile(const std::wstring& titleMsg, const std::vector<FileTypeFilter>* pFilter)
    {
        std::wstring result;
        bool getResultSuccess = false;
        IFileDialog* pFileDialog;

        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
                                  IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileDialog));

            if (SUCCEEDED(hr))
            {
                // title
                pFileDialog->SetTitle(titleMsg.c_str());

                // filter
                if (pFilter != nullptr)
                {
                    COMDLG_FILTERSPEC fileTypes[pFilter->size()];
                    for (int i = 0; i < pFilter->size(); i++)
                    {
                        fileTypes[i].pszName = (*pFilter)[i].name.c_str();
                        fileTypes[i].pszSpec = (*pFilter)[i].suffix.c_str();
                    }
                    pFileDialog->SetFileTypes(pFilter->size() * sizeof(COMDLG_FILTERSPEC), fileTypes);
                }

                // show
                hr = pFileDialog->Show(nullptr);
                if (SUCCEEDED(hr))
                {
                    IShellItem* pShellItem;
                    hr = pFileDialog->GetResult(&pShellItem);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR filePath;
                        hr = pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

                        if (SUCCEEDED(hr))
                        {
                            result = filePath;
                            getResultSuccess = true;
                            CoTaskMemFree(filePath);
                        }

                        pShellItem->Release();
                    }
                }

                pFileDialog->Release();
            }
            CoUninitialize();
        }

        if (getResultSuccess)
            return result;

        return std::nullopt;
    }

    std::optional<std::wstring> SaveFile(const std::wstring& titleMsg, const std::wstring& defaultName, const std::vector<FileTypeFilter>* pFilter)
    {
        std::wstring result;
        bool getResultSuccess = false;
        IFileDialog* pFileDialog;

        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL,
                                  IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileDialog));

            if (SUCCEEDED(hr))
            {
                // title
                pFileDialog->SetTitle(titleMsg.c_str());

                // filter
                if (pFilter != nullptr)
                {
                    COMDLG_FILTERSPEC fileTypes[pFilter->size()];
                    for (int i = 0; i < pFilter->size(); i++)
                    {
                        fileTypes[i].pszName = (*pFilter)[i].name.c_str();
                        fileTypes[i].pszSpec = (*pFilter)[i].suffix.c_str();
                    }
                    pFileDialog->SetFileTypes(pFilter->size() * sizeof(COMDLG_FILTERSPEC), fileTypes);
                }

                // save name
                pFileDialog->SetFileName(defaultName.c_str());

                // show
                hr = pFileDialog->Show(nullptr);
                if (SUCCEEDED(hr))
                {
                    IShellItem* pShellItem;
                    hr = pFileDialog->GetResult(&pShellItem);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR filePath;
                        hr = pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

                        if (SUCCEEDED(hr))
                        {
                            result = filePath;
                            getResultSuccess = true;
                            CoTaskMemFree(filePath);
                        }

                        pShellItem->Release();
                    }
                }

                pFileDialog->Release();
            }
            CoUninitialize();
        }

        if (getResultSuccess)
            return result;

        return std::nullopt;
    }

    std::optional<std::wstring> OpenDirectory(const std::wstring& titleMsg)
    {
        TCHAR path[MAX_PATH];
        BROWSEINFO bi = { 0 };
        bi.lpszTitle = titleMsg.c_str();
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