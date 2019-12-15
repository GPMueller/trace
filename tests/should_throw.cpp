#include "catch.hpp"

#include <trace/trace.hpp>

void library_function()
{
    trace_throw( "throwing for test" );
}

TEST_CASE( "State", "[state]" )
{
    SECTION( "State setup" )
    {
        REQUIRE_THROWS( library_function() );
    }
}