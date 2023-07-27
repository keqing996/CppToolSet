
#include <string>
#include <iostream>

#include "cmdline/cmdline.h"

int main(int argc, char* argv[])
{
    CmdLine::Parser a;

    // add specified type of variable.
    // 1st argument is long name
    // 2nd argument is short name (no short name if '\0' specified)
    // 3rd argument is description
    // 4th argument is mandatory (optional. default is false)
    // 5th argument is default value  (optional. it used when mandatory is false)
    a.Add<std::string>("host", 'h', "host name", true, "");

    // 6th argument is extra constraint.
    // Here, port number must be 1 to 65535 by cmdline::range().
    a.Add<int>("port", 'p', "port number", false, 80, CmdLine::Range(1, 65535));

    // cmdline::oneof() can restrict options.
    a.Add<std::string>("type", 't', "protocol type", false, "http", CmdLine::OneOf<std::string>("http", "https", "ssh", "ftp"));

    // Boolean flags also can be defined.
    // Call add method without a type parameter.
    a.Add("gzip", '\0', "gzip when transfer");

    // Run parser.
    // It returns only if command line arguments are valid.
    // If arguments are invalid, a parser output error msgs then exit program.
    // If help flag ('--help' or '-?') is specified, a parser output usage message then exit program.
    a.ParseCheck(argc, argv);

    // use flag values
    std::cout << a.Get<std::string>("type") << "://"
         << a.Get<std::string>("host") << ":"
         << a.Get<int>("port") << std::endl;

    system("pause");

    return 0;
}