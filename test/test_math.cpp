#include "catch.hpp"

#include <cmath>
#include <utils/Splines.h>
#include <utils/Containers.h>
#include <utils/Math.h>

TEST_CASE( "Splines", "[math]" ) {

    SECTION( "Basic spline use" ) {
        Spline spline;

        // Points (0, 0), (0.5, -0.5), (1.0, 1.0), (1.5, 0.0)

        spline.addSegment( 5.6, 0.0, -2.4, 0.0, 0.5 );
        spline.addSegment( -12.0, 26.4, -15.6, 2.2, 0.5 );
        spline.addSegment( 6.4, -28.8, 39.6, -16.2, 0.5 );


        REQUIRE( std::abs( spline.getValue(0.0) - 0.0 ) < 0.001 );
        REQUIRE( std::abs( spline.getValue(0.25) - -0.5125 ) < 0.001 );
        REQUIRE( std::abs( spline.getValue(0.50) - -0.5 ) < 0.001 );
        REQUIRE( std::abs( spline.getValue(0.75) - 0.2875 ) < 0.001 );
        REQUIRE( std::abs( spline.getValue(1.0) - 1.0 ) < 0.001 );
        REQUIRE( std::abs( spline.getValue(1.25) - 0.8 ) < 0.001 );
        REQUIRE( std::abs( spline.getValue(1.5) - 0.0 ) < 0.001 );
    }
}

TEST_CASE( "Finding quads for width lines", "[math]" ) {
    
    SECTION( "Basic use" ) {
        
        Vector2f a = {0.0f, 0.0f};
        Vector2f b = {2.0f, 0.0f};
        
        auto quads = Math::getQuadForLine(a, b, 1);
    }
    
    SECTION( "Basic use 2" ) {
        
        Vector2f a = {3.0f, 1.0f};
        Vector2f b = {-2.0f, -0.5f};
        
        auto quads = Math::getQuadForLine(a, b, 1);
    }
}