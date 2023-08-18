#pragma once

#include <Windows.h>
#include <stdio.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace PeParser
{
    class PEParser
    {
        using Byte = char;

        enum class Error : int
        {
            FILE_OPEN_FAILED,
        };
        
    public:
        explicit PEParser(const std::wstring& filePath)
            : _filePath(filePath)
        {
            std::ifstream readStream;
            readStream.open(_filePath, std::ios::in | std::ios::binary);

            if (!readStream.is_open())
                return;

            readStream.seekg(0, std::ios::end);
            _fileSize = readStream.tellg();
            
            _pFileContent = new Byte[_fileSize];
            readStream.seekg(0, std::ios::beg);
            readStream.read(_pFileContent, _fileSize);
            
            readStream.close();

            Process();
        }

        ~PEParser()
        {
            if (_pFileContent != nullptr)
            {
                delete[] _pFileContent;
                _pFileContent = nullptr;
            }
        }

        void PrintInfo() const;

    private:
        std::wstring _filePath;
        
        Byte* _pFileContent = nullptr;
        
        unsigned long _fileSize = 0;

        PIMAGE_DOS_HEADER _pDosHeader = nullptr;
        PIMAGE_NT_HEADERS _pNtHeader = nullptr;
        PIMAGE_FILE_HEADER _pFileHeader = nullptr;
        PIMAGE_OPTIONAL_HEADER _pOptionalHeader = nullptr;
        PIMAGE_SECTION_HEADER _pSectionHeader = nullptr;

    public:
        bool IsFileLoaded() const
        {
            return _pFileContent != nullptr;
        }

        const std::wstring& GetFilePath() const
        {
            return _filePath;
        }
        
        const Byte* GetFileContent() const
        {
            return _pFileContent;
        }

    private:
        void Process()
        {
            LoadPEInfo();
        }
        
        BOOL LoadPEInfo()
        {
            // 文件开头就是DOS头
            _pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(_pFileContent);
            if (_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE /* MZ 5a 4d */)
                return FALSE;

            // Dos头的最后是Nt头的偏移
            _pNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(_pFileContent + _pDosHeader->e_lfanew);
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

        void PrintFileHeader() const;

        void PrintOptionalHeader() const;

        void PrintSectionInfo() const;

        void PrintExportTable() const;

        void PrintImportTables() const;

        void PrintBaseRelocTable() const;

        BOOL RVAToFOV(DWORD rva, DWORD* result) const;
    };
}
