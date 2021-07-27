#include <utils/Json.h>

#include <fstream>
#include <sstream>
#include <filesystem>

#include <utils/Assert.h>


nlohmann::json utils::json::loadFromPath( std::string const &path)
{
    AssertMsg( std::filesystem::exists(path), "Json file does not exist: " + path );


    std::ifstream fstream( path );

    std::stringstream buffer;
    buffer << fstream.rdbuf();

    return nlohmann::json::parse(buffer);
}
