#pragma once

#include <Windows.h>
#include <stdio.h>

namespace PeParser
{
    class PEParser
    {
    public:
        explicit PEParser();

        ~PEParser();

        BOOL LoadFile(const char* path);

        void PrintInfo() const;

    private:
        char* _fileBuffer;
        DWORD _fileSize;

        PIMAGE_DOS_HEADER _pDosHeader;
        PIMAGE_NT_HEADERS _pNtHeader;
        PIMAGE_FILE_HEADER _pFileHeader;
        PIMAGE_OPTIONAL_HEADER _pOptionalHeader;
        PIMAGE_SECTION_HEADER _pSectionHeader;

    private:
        BOOL LoadPEInfo();

        void PrintFileHeader() const;

        void PrintOptionalHeader() const;

        void PrintSectionInfo() const;

        void PrintExportTable() const;

        void PrintImportTables() const;

        void PrintBaseRelocTable() const;

        BOOL RVAToFOV(DWORD rva, DWORD* result) const;
    };
}
