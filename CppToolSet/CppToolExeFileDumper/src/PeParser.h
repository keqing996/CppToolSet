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
        explicit PEParser(const std::wstring& filePath);

        ~PEParser();

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
        bool IsFileLoaded() const;

        const std::wstring& GetFilePath() const;

        const byte* GetFileContent() const;

        long long GetFileSize() const;

        PIMAGE_DOS_HEADER GetDosHeader() const;

        PIMAGE_NT_HEADERS GetNtHeader() const;

        PIMAGE_FILE_HEADER GetFileHeader() const;

        PIMAGE_OPTIONAL_HEADER GetOptionalHeader() const;

        std::pair<PIMAGE_SECTION_HEADER, long long> GetSectionArray() const;

        const std::string& GetExportDirectoryName() const;

        long long GetExportedFunctionBaseOrdinal() const;

        long long GetExportedFunctionsNum() const;

        long long GetExportedByNameFunctionsNum() const;

        std::pair<DWORD*, long long> GetExportedFunctionsAddrArray() const;

        std::pair<WORD*, long long> GetExportedFunctionsOrdinalArray() const;

        const std::vector<std::string>& GetExportedFunctionsNameVec() const;

        Result Process();

        DWORD GetExportFunctionAddrByName(const std::string& name) const;

        std::pair<bool, unsigned long> RVAToFOV(unsigned long rva) const;

    private:
        Result LoadFile();

        Result LoadPEInfo();

        Result ProcessExportTable();

        Result ProcessImportTable();

    private:
        static bool IsImportDescriptorValid(PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor);

        static bool IsThunkDataValid(PIMAGE_THUNK_DATA pThunkData);
    };
}
