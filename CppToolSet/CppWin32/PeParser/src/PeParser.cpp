#include "PeParser.h"

namespace PeParser
{
    PEParser::PEParser()
    {
        _fileBuffer = nullptr;
        _fileSize = 0;

        _pDosHeader = nullptr;
        _pNtHeader = nullptr;
        _pFileHeader = nullptr;
        _pOptionalHeader = nullptr;
        _pSectionHeader = nullptr;
    }

    PEParser::~PEParser()
    {
        if (_fileBuffer != nullptr)
        {
            delete[] _fileBuffer;
            _fileBuffer = nullptr;
        }
    }

    BOOL PEParser::LoadFile(const char* path)
    {
        HANDLE hFile = CreateFileA(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            printf("file open fail, error code = %lu \n", GetLastError());
            return FALSE;
        }

        _fileSize = GetFileSize(hFile, nullptr);
        _fileBuffer = new char[_fileSize]{0};

        DWORD realReadBytes;
        BOOL readFileSuccess = ReadFile(hFile, _fileBuffer, _fileSize, &realReadBytes, nullptr);
        if (!readFileSuccess)
        {
            delete[] _fileBuffer;
            _fileBuffer = nullptr;
            return FALSE;
        }

        CloseHandle(hFile);

        return LoadPEInfo();
    }

    void PEParser::PrintInfo() const
    {
        PrintFileHeader();
        printf("\n");

        PrintOptionalHeader();
        printf("\n");

        PrintSectionInfo();
        printf("\n");

        PrintExportTable();
        printf("\n");

        PrintImportTables();
        printf("\n");

        PrintBaseRelocTable();
        printf("\n");
    }

    BOOL PEParser::LoadPEInfo()
    {
        // 文件开头就是DOS头
        _pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(_fileBuffer);
        if (_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE /* MZ 5a 4d */)
            return FALSE;

        // Dos头的最后是Nt头的偏移
        _pNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(_fileBuffer + _pDosHeader->e_lfanew);
        if (_pNtHeader->Signature != IMAGE_NT_SIGNATURE /* PE00 0x00004550 */)
            return FALSE;

        // 标准PE头
        _pFileHeader = &_pNtHeader->FileHeader;

        // 可选PE头
        _pOptionalHeader = &_pNtHeader->OptionalHeader;

        // Section头 -> 紧挨着NT头，是一个数组，长度在fileHeader->NumberOfSections
        _pSectionHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>(reinterpret_cast<ULONGLONG>(_pNtHeader) + sizeof(
            IMAGE_NT_HEADERS));

        return TRUE;
    }

    void PEParser::PrintFileHeader() const
    {
        printf("FileHeader: \n");
        printf(" - Machine: 0x%hx\n", _pFileHeader->Machine);
        printf(" - Section Number: %d\n", _pFileHeader->NumberOfSections);
        printf(" - Optional Header Size: %d\n", _pFileHeader->SizeOfOptionalHeader);
    }

    void PEParser::PrintOptionalHeader() const
    {
        printf("OptionalHeader: \n");
        printf(" - AddressOfEntryPoint: 0x%lx\n", _pOptionalHeader->AddressOfEntryPoint);
        printf(" - ImageBase: 0x%llx\n", _pOptionalHeader->ImageBase);
        printf(" - SectionAlignment: 0x%lx\n", _pOptionalHeader->SectionAlignment);
        printf(" - FileAlignment: 0x%lx\n", _pOptionalHeader->FileAlignment);
        printf(" - SizeOfImage: %lu\n", _pOptionalHeader->SizeOfImage);
        printf(" - SizeOfHeaders: %lu\n", _pOptionalHeader->SizeOfHeaders);
        printf(" - NumberOfRvaAndSizes: %lu\n", _pOptionalHeader->NumberOfRvaAndSizes);
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
            auto pExportDirectory = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(exportTableFov + _fileBuffer);

            // 导出表文件名
            DWORD fileNameFov;
            if (RVAToFOV(pExportDirectory->Name, &fileNameFov))
            {
                const char* fileName = _fileBuffer + fileNameFov;
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
                auto ptr = reinterpret_cast<const DWORD*>(functionAddrFov + _fileBuffer);
                for (DWORD i = 0; i < pExportDirectory->NumberOfFunctions; i++)
                    printf(" - FunctionAddr: 0x%lx\n", *(ptr + i));
            }

            // 序号表（长度 = NumberOfFunctions
            DWORD ordinalAddrFov;
            if (RVAToFOV(pExportDirectory->AddressOfNameOrdinals, &ordinalAddrFov))
            {
                auto ptr = reinterpret_cast<const WORD*>(ordinalAddrFov + _fileBuffer);
                for (DWORD i = 0; i < pExportDirectory->NumberOfFunctions; i++)
                    printf(" - FunctionOrdinal: %u\n", *(ptr + i));
            }

            // 名称表（长度 = NumberOfNames
            DWORD functionNameTableFov;
            if (RVAToFOV(pExportDirectory->AddressOfNames, &functionNameTableFov))
            {
                auto pFunctionNameTable = reinterpret_cast<DWORD*>(functionNameTableFov + _fileBuffer);
                for (DWORD i = 0; i < pExportDirectory->NumberOfNames; i++)
                {
                    DWORD functionNameRva = *(pFunctionNameTable + i);
                    DWORD functionNameFov;
                    if (RVAToFOV(functionNameRva, &functionNameFov))
                        printf(" - FunctionName: %s\n", functionNameFov + _fileBuffer);
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
            auto pImportDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(importTableFov + _fileBuffer);

            // 遍历链表
            while (IsImportDescriptorValid(pImportDescriptor))
            {
                DWORD dllNameFov;
                if (RVAToFOV(pImportDescriptor->Name, &dllNameFov))
                    printf(" - Dll Name: %s\n", dllNameFov + _fileBuffer);

                DWORD thunkFov;
                if (RVAToFOV(pImportDescriptor->OriginalFirstThunk, &thunkFov))
                {
                    auto pThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(thunkFov + _fileBuffer);

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
                                    _fileBuffer);
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
