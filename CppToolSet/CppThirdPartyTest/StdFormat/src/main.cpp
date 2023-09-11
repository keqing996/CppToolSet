#include <format> // cpp 20
#include <iostream>

struct Product
{
    std::string brand;
    int value;
    std::string name;
};

namespace std
{
    template<>
    struct std::formatter<Product>
    {
        bool detailed { false };

        constexpr auto parse(std::format_parse_context& context)
        {
            auto it = context.begin();
            auto end = context.end();

            if (it != end && *it == 'd')
            {
                detailed = true;
                ++it;
            }

            return it;
        }

        template<class FormatContext>
        auto format(const Product& p, FormatContext& context)
        {
            if (detailed)
            {
                return format_to(context.out(), "product ({}, {}, {})", p.brand, p.name, p.value);
            }
            else
            {
                return format_to(context.out(), "product {}", p.name);
            }
        }
    };
}

int main()
{
    // general string & number
    std::cout << std::format("test: string {}, number {}, {}, {}", "hello world", 1, 2.34, 5.67f) << std::endl;
    std::cout << std::endl;

    // big bracket, same with c sharp
    std::cout << std::format("bracket: {{ {} }}", "hello world") << std::endl;
    std::cout << std::endl;

    // fill & align: number after colon means the output occupies at least that number width
    std::cout << std::format("fill: |{:12}|", 5) << std::endl;
    std::cout << std::endl;

    // fill & align: < for left align, > for right align, ^ for center align
    std::cout << std::format("align: |{:<12}|", 5) << std::endl;
    std::cout << std::format("align: |{:>12}|", 5) << std::endl;
    std::cout << std::format("align: |{:^12}|", 5) << std::endl;
    std::cout << std::endl;

    // fill & align: fill blank with specific char
    std::cout << std::format("align: |{:*<12}|", 5) << std::endl;
    std::cout << std::format("align: |{:@>12}|", 5) << std::endl;
    std::cout << std::format("align: |{:g^12}|", 5) << std::endl;
    std::cout << std::endl;

    // sign: + will give any positive number a sign (include 0)
    std::cout << std::format("sign: {:+}, {:+}, {:+}", 5, -5, 0) << std::endl;
    std::cout << std::endl;

    // system of numeration: # with x,d,o,b
    std::cout << std::format("system of numeration: {:#x}, {:#d}, {:#o}, {:#b}", 255, 255, 255, 255) << std::endl;
    std::cout << std::endl;

    // number precision: "x.y", x for integer, y for decimal
    std::cout << std::format("precision: {:.5f}", 3.141592657) << std::endl;
    std::cout << std::endl;

    // w string
    std::cout << std::format("current cout loc: {}", std::cout.getloc().name()) << std::endl;
    std::cout << std::format("current wcout loc: {}", std::wcout.getloc().name()) << std::endl;

    // set cn
    std::wcout.imbue(std::locale("zh_CN"));
    std::wcout << std::format(L"wstring: {}", L"测试") << std::endl;
    std::cout << std::endl;

    // custom structure
    Product p{ "Brand", 233, "test_name" };

    std::cout << std::format("{:d}", p) << std::endl;
    std::cout << std::format("{}", p) << std::endl;

    return 0;
}
