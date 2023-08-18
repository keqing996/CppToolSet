#pragma once

#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace PeParser
{
    // using byte definition in windows.h, which type define byte as unsigned char

    enum class Result : int
    {
        SUCCESS,
        FILE_OPEN_FAILED,
        MAGIC_HEADER_NO_PE,
        NT_HEADER_NO_PE,
        RVA_TO_FOV_FAILED,
    };
    
    class PEParser
    {
    public:
        explicit PEParser(const std::wstring& filePath)
            : _filePath(filePath)
        {
        }

        ~PEParser()
        {
            if (_pFileContent != nullptr)
            {
                delete[] _pFileContent;
                _pFileContent = nullptr;
            }
        }

    private:

        /* File path */
        std::wstring _filePath;

        /* Pointer to file content */
        byte* _pFileContent = nullptr;

        /* File size in byte */
        long long _fileSize = 0;

        /* PE headers */
        PIMAGE_DOS_HEADER _pDosHeader = nullptr;
        PIMAGE_NT_HEADERS _pNtHeader = nullptr;
        PIMAGE_FILE_HEADER _pFileHeader = nullptr;
        PIMAGE_OPTIONAL_HEADER _pOptionalHeader = nullptr;
        
        /* Section info */
        long long _sectionNum = 0;
        PIMAGE_SECTION_HEADER _pSectionArray = nullptr;

        /* Export table */
        PIMAGE_EXPORT_DIRECTORY _pExportDirectory = nullptr;
        std::string _exportTableFileName {};
        long long _exportTableStartOrdinal = 0;
        
        long long _exportFunctionNum = 0;
        DWORD* _pExportedFunctionAddrArray = nullptr;
        WORD* _pExportedFunctionOrdinalArray = nullptr;
        
        long long _exportFunctionByNameNum = 0;
        DWORD* _pExportedFunctionNameArray = nullptr;
        std::vector<std::string> _pExportedFunctionNameVec {};

    public:
        bool IsFileLoaded() const
        {
            return _pFileContent != nullptr;
        }

        const std::wstring& GetFilePath() const
        {
            return _filePath;
        }
        
        const byte* GetFileContent() const
        {
            return _pFileContent;
        }

        long long GetFileSize() const
        {
            return _fileSize;
        }

        PIMAGE_DOS_HEADER GetDosHeader() const
        {
            return _pDosHeader;
        }

        PIMAGE_NT_HEADERS GetNtHeader() const
        {
            return _pNtHeader;
        }

        PIMAGE_FILE_HEADER GetFileHeader() const
        {
            return _pFileHeader;
        }

        PIMAGE_OPTIONAL_HEADER GetOptionalHeader() const
        {
            return _pOptionalHeader;
        }

        std::pair<PIMAGE_SECTION_HEADER, long long> GetSectionArray() const
        {
            return { _pSectionArray, _sectionNum };
        }

        const std::string& GetExportDirectoryName() const
        {
            return _exportTableFileName;
        }
        
        long long GetExportedFunctionBaseOrdinal() const
        {
            return _exportTableStartOrdinal;
        }

        long long GetExportedFunctionsNum() const
        {
            return _exportFunctionNum;
        }

        long long GetExportedByNameFunctionsNum() const
        {
            return _exportFunctionByNameNum;
        }

        std::pair<DWORD*, long long> GetExportedFunctionsAddrArray() const
        {
            return { _pExportedFunctionAddrArray, _exportFunctionNum };
        }

        std::pair<WORD*, long long> GetExportedFunctionsOrdinalArray() const
        {
            return { _pExportedFunctionOrdinalArray, _exportFunctionNum };
        }

        const std::vector<std::string>& GetExportedFunctionsNameVec() const
        {
            return _pExportedFunctionNameVec;
        }

        Result Process()
        {
            Result result = LoadFile();
            if (result != Result::SUCCESS)
                return result;
            
            result = LoadPEInfo();
            if (result != Result::SUCCESS)
                return result;

            result = ProcessExportTable();
            if (result != Result::SUCCESS)
                return result;

            result = ProcessImportTable();
            if (result != Result::SUCCESS)
                return result;

            return Result::SUCCESS;
        }

        DWORD GetExportFunctionAddrByName(const std::string& name) const
        {
            for (int i = 0; i < _pExportedFunctionNameVec.size(); i++)
            {
                if (_pExportedFunctionNameVec[i] == name)
                {
                    WORD index = *(_pExportedFunctionOrdinalArray + i);
                    return *(_pExportedFunctionAddrArray + index);
                }
            }

            return 0;
        }

        std::pair<bool, unsigned long> RVAToFOV(unsigned long rva) const
        {
            for (int i = 0; i < _pNtHeader->FileHeader.NumberOfSections; i++)
            {
                // 找rva落在哪个section区间内
                if (rva >= _pSectionArray[i].VirtualAddress
                    && rva < (_pSectionArray[i].VirtualAddress + _pSectionArray[i].SizeOfRawData))
                {
                    unsigned long result = _pSectionArray[i].PointerToRawData + (rva - _pSectionArray[i].VirtualAddress);
                    return { true, result };
                }
            }
            
            return { false, 0 };
        }

    private:
        Result LoadFile()
        {
            std::ifstream readStream;
            readStream.open(_filePath, std::ios::in | std::ios::binary);

            if (!readStream.is_open())
                return Result::FILE_OPEN_FAILED;

            readStream.seekg(0, std::ios::end);
            _fileSize = readStream.tellg();
            
            char* pFileContentInChar = new char[_fileSize];
            readStream.seekg(0, std::ios::beg);
            readStream.read(pFileContentInChar, _fileSize);
            _pFileContent = reinterpret_cast<byte*>(pFileContentInChar);
            
            readStream.close();

            return Result::SUCCESS;
        }
        
        Result LoadPEInfo()
        {
            // 文件开头就是DOS头
            _pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(_pFileContent);
            if (_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE /* MZ 5a 4d */)
                return Result::MAGIC_HEADER_NO_PE;

            // Dos头的最后是Nt头的偏移
            _pNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(_pFileContent + _pDosHeader->e_lfanew);
            if (_pNtHeader->Signature != IMAGE_NT_SIGNATURE /* PE00 0x00004550 */)
                return Result::NT_HEADER_NO_PE;

            // 标准PE头
            _pFileHeader = &_pNtHeader->FileHeader;
            _sectionNum = _pFileHeader->NumberOfSections;

            // 可选PE头
            _pOptionalHeader = &_pNtHeader->OptionalHeader;

            // Section头 -> 紧挨着NT头，是一个数组，长度在fileHeader->NumberOfSections
            _pSectionArray = reinterpret_cast<PIMAGE_SECTION_HEADER>(
                reinterpret_cast<ULONGLONG>(_pNtHeader) + sizeof(IMAGE_NT_HEADERS)
                );

            return Result::SUCCESS;
        }

        Result ProcessExportTable()
        {
            IMAGE_DATA_DIRECTORY exportTableDataDirectory = _pOptionalHeader->DataDirectory[0];
            auto exportTableFovPair = RVAToFOV(exportTableDataDirectory.VirtualAddress);
            if (!exportTableFovPair.first)
                return Result::RVA_TO_FOV_FAILED;

            auto exportTableFov = exportTableFovPair.second;
            _pExportDirectory = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(
                _pFileContent + exportTableFov
                );

            // export table file name
            auto fileNameFovPair = RVAToFOV(_pExportDirectory->Name);
            if (!fileNameFovPair.first)
                return Result::RVA_TO_FOV_FAILED;

            auto fileNameFov = fileNameFovPair.second;
            const char* fileName = reinterpret_cast<const char*>(_pFileContent + fileNameFov);
            _exportTableFileName = std::string{fileName};

            // total number of exported functions
            _exportFunctionNum = _pExportDirectory->NumberOfFunctions;

            // export function start ordinal
            _exportTableStartOrdinal = _pExportDirectory->Base;

            // exported functions addr table
            auto functionAddrFovPair = RVAToFOV(_pExportDirectory->AddressOfFunctions);
            if (!functionAddrFovPair.first)
                return Result::RVA_TO_FOV_FAILED;

            _pExportedFunctionAddrArray = reinterpret_cast<DWORD*>(
                _pFileContent + functionAddrFovPair.second
                );

            // exported functions name table
            auto ordinalAddrFovPair = RVAToFOV(_pExportDirectory->AddressOfNameOrdinals);
            if (!ordinalAddrFovPair.first)
                return Result::RVA_TO_FOV_FAILED;

            _pExportedFunctionOrdinalArray = reinterpret_cast<WORD*>(
                _pFileContent + ordinalAddrFovPair.second
                );

            // total number of name-exported functions
            _exportFunctionByNameNum = _pExportDirectory->NumberOfNames;

            // exported functions name table 
            auto functionNameTableFovPair = RVAToFOV(_pExportDirectory->AddressOfNames);
            if (!functionNameTableFovPair.first)
                return Result::RVA_TO_FOV_FAILED;

            _pExportedFunctionNameArray = reinterpret_cast<DWORD*>(
                _pFileContent + functionNameTableFovPair.second
                );

            for (int i = 0; i < _exportFunctionByNameNum; i++)
            {
                DWORD functionNameRva = *(_pExportedFunctionNameArray + i);
                auto functionNameFovPair = RVAToFOV(functionNameRva);
                if (!functionNameFovPair.first)
                    return Result::RVA_TO_FOV_FAILED;

                char* pName = reinterpret_cast<char*>(_pFileContent + functionNameFovPair.second);
                _pExportedFunctionNameVec.emplace_back(pName);
            }

            return Result::SUCCESS;
        }

        Result ProcessImportTable()
        {
            return Result::SUCCESS;
        }

    private:
        static bool IsImportDescriptorValid(PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor)
        {
            return pImportDescriptor->OriginalFirstThunk != 0;
        }

        static bool IsThunkDataValid(PIMAGE_THUNK_DATA pThunkData)
        {
            return pThunkData->u1.Function != 0;
        }
    };
}
