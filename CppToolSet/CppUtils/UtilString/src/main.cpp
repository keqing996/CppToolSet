#include <iomanip>
#include <iostream>

#include "../include/utilString.hpp"


int main()
{
	// Split test
	std::string str1 {"asd,qwe,zxf,wsd"};
	auto str1Split = StringUtil::Split(str1, ',');
	for (auto str : str1Split)
		std::cout << str << std::endl;

	auto joinStr = StringUtil::Join<char>(str1Split, "_");
	std::cout << joinStr << std::endl;
	return 0;
}