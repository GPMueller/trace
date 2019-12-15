#include "catch.hpp"

#include <trace/trace.hpp>

void should_throw()
{
    trace::initiate( "throwing for test" );
}

TEST_CASE( "Should throwing", "[throw]" )
{
    SECTION( "throw" )
    {
        REQUIRE_THROWS( should_throw() );
    }
}