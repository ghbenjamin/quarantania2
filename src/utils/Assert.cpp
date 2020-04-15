#include <utils/Assert.h>
#include <string>

template <typename T>
void assert_with_msg(const char* expr_str, bool expr, const char* file, int line, T msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
                  << "Expected:\t" << expr_str << "\n"
                  << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}

template void assert_with_msg<std::string>(const char* expr_str, bool expr, const char* file, int line, std::string msg);
template void assert_with_msg<const char*>(const char* expr_str, bool expr, const char* file, int line, const char* msg);

void assert_without_msg(const char* expr_str, bool expr, const char* file, int line)
{
    if (!expr)
    {
        std::cerr << "Assert failed.\n"
                  << "Expected:\t" << expr_str << "\n"
                  << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}