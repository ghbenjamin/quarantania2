#include <utils/Json.h>

#include <fstream>
#include <sstream>


nlohmann::json utils::json::loadFromPath( std::string const &path)
{
    std::ifstream fstream( path );

    std::stringstream buffer;
    buffer << fstream.rdbuf();

    return nlohmann::json::parse(buffer);
}
