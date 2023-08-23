#include <format> // cpp 20
#include <iostream>

int main()
{
	// general string & number
	std::cout << std::format("test: string {}, number {}, {}, {}", "hello world", 1, 2.34, 5.67f) << std::endl;

	// big bracket, same with c sharp
	std::cout << std::format("bracket: {{ {} }}", "hello world") << std::endl;

	// fill & align: number after colon means the output occupies at least that number width
	std::cout << std::format("fill: |{:12}|", 5) << std::endl;

	// fill & align: < for left align, > for right align, ^ for center align
	std::cout << std::format("align: |{:<12}|", 5) << std::endl;
	std::cout << std::format("align: |{:>12}|", 5) << std::endl;
	std::cout << std::format("align: |{:^12}|", 5) << std::endl;

	// fill & align: fill blank with specific char
	std::cout << std::format("align: |{:*<12}|", 5) << std::endl;
	std::cout << std::format("align: |{:@>12}|", 5) << std::endl;
	std::cout << std::format("align: |{:g^12}|", 5) << std::endl;

	// sign: + will give any positive number a sign (include 0)
	std::cout << std::format("sign: {:+}, {:+}, {:+}", 5, -5, 0) << std::endl;

	// system of numeration: # with x,d,o,b
	std::cout << std::format("system of numeration: {:#x}, {:#d}, {:#o}, {:#b}", 255, 255, 255, 255) << std::endl;

	// number precision: "x.y", x for integer, y for decimal
	std::cout << std::format("precision: {:.5f}", 3.141592657) << std::endl;
	
	return 0;
}