#include "catch.hpp"

#include <trace/trace.hpp>

void should_not_throw() try
{
    trace::initiate( "throwing for test" );
}
catch( const std::exception & ex )
{
    trace::handle( ex );
}

TEST_CASE( "Should not throw", "[nothrow]" )
{
    SECTION( "nothrow" )
    {
        REQUIRE_NOTHROW( should_not_throw() );
        std::string backtrace = "";
        REQUIRE_NOTHROW( backtrace = trace::latest() );
        REQUIRE_FALSE( backtrace == "" );
    }
}