#pragma once

#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <string>

namespace PeParser
{
    using Byte = char;

    enum class Result : int
    {
        SUCCESS,
        FILE_OPEN_FAILED,
        MAGIC_HEADER_NO_PE,
        NT_HEADER_NO_PE,
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

        void PrintInfo() const;

    private:
        std::wstring _filePath;
        
        Byte* _pFileContent = nullptr;
        
        long long _fileSize = 0;

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

        PIMAGE_SECTION_HEADER GetSectionHeader() const
        {
            return _pSectionHeader;
        }

        Result Process()
        {
            Result result = LoadFile();
            if (result != Result::SUCCESS)
                return result;
            
            result = LoadPEInfo();
            if (result != Result::SUCCESS)
                return result;

            

            return Result::SUCCESS;
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
            
            _pFileContent = new Byte[_fileSize];
            readStream.seekg(0, std::ios::beg);
            readStream.read(_pFileContent, _fileSize);
            
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

            // 可选PE头
            _pOptionalHeader = &_pNtHeader->OptionalHeader;

            // Section头 -> 紧挨着NT头，是一个数组，长度在fileHeader->NumberOfSections
            _pSectionHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>(
                reinterpret_cast<ULONGLONG>(_pNtHeader) + sizeof(IMAGE_NT_HEADERS)
                );

            return Result::SUCCESS;
        }

        

        void PrintSectionInfo() const;

        void PrintExportTable() const;

        void PrintImportTables() const;

        void PrintBaseRelocTable() const;

        BOOL RVAToFOV(DWORD rva, DWORD* result) const;
    };
}
