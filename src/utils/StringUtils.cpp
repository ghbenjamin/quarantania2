#include <utils/StringUtils.h>

bool stringContains(std::string const& str, std::string const& substr)
{
    return str.find(substr) != std::string::npos;
}
