#include "catch.hpp"

#include <utils/BBCode.h>

TEST_CASE( "BBCode parsing", "[utils]" ) {
    
    SECTION( "Plain string" ) {
        std::string str = "The quick brown fox jumps over the lazy dog";
        auto doc = BBCode::parseText( str );
        
        REQUIRE(doc.nodes().size() == 1 );
        REQUIRE(doc.nodes().front().text == str );
        REQUIRE( !doc.nodes().front().colour.has_value() );
    }
    
    SECTION( "Simple bbcode" ) {
        std::string str = "The <c:Red>quick brown fox</c> jumps over the lazy dog";
        auto doc = BBCode::parseText( str );
        
        REQUIRE(doc.nodes().size() == 3 );
    }
    
    SECTION( "More complicated bbcode" ) {
        std::string str = "The <c:red>quick brown fox</c><c:#00FF00> jumps</c> over the lazy dog";
        auto doc = BBCode::parseText( str );
        
        REQUIRE(doc.nodes().size() == 4 );
    }

    SECTION( "Example from testing" ) {
        std::string str = "<c:#ff0000>Gwendlyn</c> <c:#0000ff>Di</c> <c:#0af22d>Corci</c>";
        auto doc = BBCode::parseText( str );

        REQUIRE(doc.nodes().size() == 5 );
    }

    SECTION( "Tag is entire string" ) {
        std::string str = "<c:red>quick brown fox jumps over the lazy dog</c>";
        auto doc = BBCode::parseText( str );

        REQUIRE(doc.nodes().size() == 1 );
    }

    SECTION( "Iterating over a bbdoc" ) {
        std::string str = "The <c:red>quick brown fox</c><c:#00FF00> jumps</c> over the lazy dog";
        auto doc = BBCode::parseText( str );
       
        auto it = doc.begin();
        REQUIRE( it->first == 'T' );
        REQUIRE( !it->second.has_value() );
        
        ++it;
        REQUIRE( it->first == 'h' );
        ++it;
        REQUIRE( it->first == 'e' );
        ++it;
        REQUIRE( it->first == ' ' );
        REQUIRE( !it->second.has_value() );
        ++it;
        REQUIRE( it->first == 'q' );
        REQUIRE( it->second.has_value() );
    }
    
    SECTION ( "BBDoc for loop" ) {
        std::string str = "The <c:red>quick brown fox</c><c:#00FF00> jumps</c> over the lazy dog";
        auto doc = BBCode::parseText( str );
        
        std::string out;
        for ( auto it = doc.begin(); it != doc.end(); ++it )
        {
            out.push_back( it->first );
        }
        
        REQUIRE( out == "The quick brown fox jumps over the lazy dog" );
    }
}