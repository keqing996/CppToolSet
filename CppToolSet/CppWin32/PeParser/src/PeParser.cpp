#include "PeParser.hpp"

namespace PeParser
{

    void PEParser::PrintImportTables() const
    {
        // PIMAGE_IMPORT_DESCRIPTOR的OriginalFirstThunk不为0位有效
        auto IsImportDescriptorValid = [](PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor) -> bool
        {
            return pImportDescriptor->OriginalFirstThunk != 0;
        };

        auto IsThunkDataValid = [](PIMAGE_THUNK_DATA pThunkData) -> bool
        {
            return pThunkData->u1.Function != 0;
        };

        printf("ExportTable: \n");
        IMAGE_DATA_DIRECTORY importTableDataDirectory = _pOptionalHeader->DataDirectory[1];

        DWORD importTableFov;
        if (RVAToFOV(importTableDataDirectory.VirtualAddress, &importTableFov))
        {
            auto pImportDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(importTableFov + _pFileContent);

            // 遍历链表
            while (IsImportDescriptorValid(pImportDescriptor))
            {
                DWORD dllNameFov;
                if (RVAToFOV(pImportDescriptor->Name, &dllNameFov))
                    printf(" - Dll Name: %s\n", dllNameFov + _pFileContent);

                DWORD thunkFov;
                if (RVAToFOV(pImportDescriptor->OriginalFirstThunk, &thunkFov))
                {
                    auto pThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(thunkFov + _pFileContent);

                    // 遍历链表
                    while (IsThunkDataValid(pThunk))
                    {
                        // IMAGE_THUNK_DATA为UNION，最高位为1表示按序号导入，否则为按名称导入
                        if (pThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
                            printf(" - Import By Ordinal: %llu\n", pThunk->u1.Ordinal - IMAGE_ORDINAL_FLAG);
                        else
                        {
                            DWORD dataAddrFov;
                            if (RVAToFOV(pThunk->u1.AddressOfData, &dataAddrFov))
                            {
                                auto pNameImport = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(dataAddrFov +
                                    _pFileContent);
                                printf(" - Import By Name: %s\n", pNameImport->Name);
                            }
                        }

                        pThunk++;
                    }
                }

                pImportDescriptor++;
            }
        }
    }
}
