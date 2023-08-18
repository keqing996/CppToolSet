
#include <stdio.h>

#include "PeParser.hpp"

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
	
	

	return 0;
}