
#include <stdio.h>

#include "PeParser.h"

int main()
{
	PeParser::PEParser peParser{ L"sss" };

	PeParser::Result result = peParser.Process();
	if (result != PeParser::Result::SUCCESS)
		return 1;

	// print file header
	auto _pFileHeader = peParser.GetFileHeader();
	printf("FileHeader: \n");
	printf(" - Machine: 0x%hx\n", _pFileHeader->Machine);
	printf(" - Section Number: %d\n", _pFileHeader->NumberOfSections);
	printf(" - Optional Header Size: %d\n", _pFileHeader->SizeOfOptionalHeader);
	printf("\n");

	// print optional header
	auto _pOptionalHeader = peParser.GetOptionalHeader();
	printf("OptionalHeader: \n");
	printf(" - AddressOfEntryPoint: 0x%lx\n", _pOptionalHeader->AddressOfEntryPoint);
	printf(" - ImageBase: 0x%llx\n", _pOptionalHeader->ImageBase);
	printf(" - SectionAlignment: 0x%lx\n", _pOptionalHeader->SectionAlignment);
	printf(" - FileAlignment: 0x%lx\n", _pOptionalHeader->FileAlignment);
	printf(" - SizeOfImage: %lu\n", _pOptionalHeader->SizeOfImage);
	printf(" - SizeOfHeaders: %lu\n", _pOptionalHeader->SizeOfHeaders);
	printf(" - NumberOfRvaAndSizes: %lu\n", _pOptionalHeader->NumberOfRvaAndSizes);
	printf("\n");

	// print section info
	printf("SectionTable: \n");
	auto [pSectionAry, sectionLength] = peParser.GetSectionArray();
	for (int i = 0; i < sectionLength; i++)
	{
		PIMAGE_SECTION_HEADER pSectionHeader = pSectionAry + i;
		
		// Section头可能填充满，没有\0
		char sectionName[9]{'\0'};
		memcpy_s(sectionName, 8, pSectionHeader->Name, 8);
		printf(" - %s\n", sectionName);
		printf("   - VirtualAddress: 0x%lX\n", pSectionHeader->VirtualAddress);
		printf("   - VirtualAddress: %lu\n", pSectionHeader->SizeOfRawData);
		printf("   - VirtualAddress: 0x%lX\n", pSectionHeader->PointerToRawData);
	}
	printf("\n");

	// export table
	printf("ExportTable: \n");
	printf(" - Name: %s\n", peParser.GetExportDirectoryName().c_str());
	printf(" - Function Order Base: %lld\n", peParser.GetExportedFunctionBaseOrdinal());
	printf(" - Function Number: %lld\n", peParser.GetExportedFunctionsNum());
	printf(" - Name Export Number: %lld\n", peParser.GetExportedByNameFunctionsNum());

	auto [pExportedFunAddrArray, pExportedFunAddrArraySize] = peParser.GetExportedFunctionsAddrArray();
	for (int i = 0; i < pExportedFunAddrArraySize; i++)
	{
		printf(" - FunctionAddr %d: 0x%lx\n", i, *(pExportedFunAddrArray + i));
	}

	auto [pExportedFunOrdinalArray, pExportedFunOrdinalArraySize] = peParser.GetExportedFunctionsOrdinalArray();
	for (int i = 0; i < pExportedFunOrdinalArraySize; i++)
	{
		printf(" - FunctionOrdinal %d: %u\n", i, *(pExportedFunOrdinalArray + i));
	}

	const auto& exportedFunNameVec = peParser.GetExportedFunctionsNameVec();
	for (int i = 0; i < exportedFunNameVec.size(); i++)
	{
		printf(" - FunctionName %d: %s\n", i, exportedFunNameVec[i].c_str());
	}

	return 0;
}