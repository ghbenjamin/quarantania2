#include <utils/Compression.h>
#include <zlib.h>
#include <sstream>

// Adapted from
// https://panthema.net/2007/0328-ZLibString.html

std::string zlib_decompress( std::string const& data )
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if ( inflateInit(&zs) != Z_OK )
    {
        throw(std::runtime_error("inflateInit failed while decompressing."));
    }

    zs.next_in = (Bytef*)data.data();
    zs.avail_in = data.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    do
    {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out)
        {
            outstring.append( outbuffer, zs.total_out - outstring.size() );
        }

    }
    while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END)
    {
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw( std::runtime_error( oss.str() ) );
    }

    return outstring;
}