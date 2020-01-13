#include <utils/Json.h>
#include <utils/Assert.h>

#include <fstream>
#include <sstream>

rapidjson::Document JsonUtils::loadFromPath(std::string const &path)
{
    std::ifstream fstream( path );

    std::stringstream buffer;
    buffer << fstream.rdbuf();

    rapidjson::Document doc;
    doc.Parse( buffer.str().c_str() );

    Assert( doc.IsObject() );
    Assert( !doc.ObjectEmpty() );

    return doc;
}
