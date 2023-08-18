#include "PeParser.hpp"

namespace PeParser
{
    

    void PEParser::PrintInfo() const
    {
        PrintSectionInfo();
        printf("\n");

        PrintExportTable();
        printf("\n");

        PrintImportTables();
        printf("\n");

        PrintBaseRelocTable();
        printf("\n");
    }

    void PEParser::PrintSectionInfo() const
    {
        printf("SectionTable: \n");
        for (int i = 0; i < _pFileHeader->NumberOfSections; i++)
        {
            PIMAGE_SECTION_HEADER pSingleSectionHeader = _pSectionHeader + i;

            // Section头可能填充满，没有\0
            char sectionName[9]{'\0'};
            memcpy_s(sectionName, 8, pSingleSectionHeader->Name, 8);
            printf(" - %s\n", sectionName);
            printf("   - VirtualAddress: 0x%lX\n", pSingleSectionHeader->VirtualAddress);
            printf("   - VirtualAddress: %lu\n", pSingleSectionHeader->SizeOfRawData);
            printf("   - VirtualAddress: 0x%lX\n", pSingleSectionHeader->PointerToRawData);
        }
    }

    void PEParser::PrintExportTable() const
    {
        printf("ExportTable: \n");
        IMAGE_DATA_DIRECTORY exportTableDataDirectory = _pOptionalHeader->DataDirectory[0];
        DWORD exportTableFov;
        if (RVAToFOV(exportTableDataDirectory.VirtualAddress, &exportTableFov))
        {
            auto pExportDirectory = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(exportTableFov + _pFileContent);

            // 导出表文件名
            DWORD fileNameFov;
            if (RVAToFOV(pExportDirectory->Name, &fileNameFov))
            {
                const char* fileName = _pFileContent + fileNameFov;
                printf(" - Name: %s\n", fileName);
            }

            // 函数起始序号
            printf(" - Function Order Base: %lu\n", pExportDirectory->Base);

            // 导出函数的个数
            printf(" - Function Number: %lu\n", pExportDirectory->NumberOfFunctions);

            // 导出函数的个数
            printf(" - Name Export Number: %lu\n", pExportDirectory->NumberOfNames);

            // 地址表（长度 = NumberOfFunctions
            DWORD functionAddrFov;
            if (RVAToFOV(pExportDirectory->AddressOfFunctions, &functionAddrFov))
            {
                auto ptr = reinterpret_cast<const DWORD*>(functionAddrFov + _pFileContent);
                for (DWORD i = 0; i < pExportDirectory->NumberOfFunctions; i++)
                    printf(" - FunctionAddr: 0x%lx\n", *(ptr + i));
            }

            // 序号表（长度 = NumberOfFunctions
            DWORD ordinalAddrFov;
            if (RVAToFOV(pExportDirectory->AddressOfNameOrdinals, &ordinalAddrFov))
            {
                auto ptr = reinterpret_cast<const WORD*>(ordinalAddrFov + _pFileContent);
                for (DWORD i = 0; i < pExportDirectory->NumberOfFunctions; i++)
                    printf(" - FunctionOrdinal: %u\n", *(ptr + i));
            }

            // 名称表（长度 = NumberOfNames
            DWORD functionNameTableFov;
            if (RVAToFOV(pExportDirectory->AddressOfNames, &functionNameTableFov))
            {
                auto pFunctionNameTable = reinterpret_cast<DWORD*>(functionNameTableFov + _pFileContent);
                for (DWORD i = 0; i < pExportDirectory->NumberOfNames; i++)
                {
                    DWORD functionNameRva = *(pFunctionNameTable + i);
                    DWORD functionNameFov;
                    if (RVAToFOV(functionNameRva, &functionNameFov))
                        printf(" - FunctionName: %s\n", functionNameFov + _pFileContent);
                }
            }
        }
    }

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

    void PEParser::PrintBaseRelocTable() const
    {
    }

    BOOL PEParser::RVAToFOV(DWORD rva, DWORD* result) const
    {
        for (int i = 0; i < _pNtHeader->FileHeader.NumberOfSections; i++)
        {
            // 找rva落在哪个section区间内
            if (rva >= _pSectionHeader[i].VirtualAddress && rva < (_pSectionHeader[i].VirtualAddress +
                _pSectionHeader[i].SizeOfRawData))
            {
                *result = _pSectionHeader[i].PointerToRawData + (rva - _pSectionHeader[i].VirtualAddress);
                return TRUE;
            }
        }

        *result = 0;
        return FALSE;
    }
}
