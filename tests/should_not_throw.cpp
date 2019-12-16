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

void throwing_layer()
{
    trace::initiate( "initiation point of the backtrace" );
}

void middle_layer_2() try
{
    throwing_layer();
}

catch( const std::exception & ex )
{
    trace::propagate( "mid layer 2 propagation point of the backtrace" );
}

void middle_layer_1() try
{
    middle_layer_2();
}

catch( const std::exception & ex )
{
    trace::propagate( "mid layer 1 propagation point of the backtrace" );
}

void top_layer() try
{
    middle_layer_1();
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

    SECTION( "layered" )
    {
        REQUIRE_THROWS( middle_layer_1() );
        REQUIRE_NOTHROW( top_layer() );
        std::string backtrace = "";
        REQUIRE_NOTHROW( backtrace = trace::latest() );
        REQUIRE_THROWS( throwing_layer() );
    }
}