#include <trace/exception.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

namespace trace
{
    std::string latest()
    {
        std::scoped_lock<std::mutex> sl( inner::_trace_string_mutex );
        std::string ret = inner::_latest_trace_string;
        return ret;
    }
}

namespace trace::inner
{
    std::string _latest_trace_string = "";
    std::mutex _trace_string_mutex   = std::mutex();

    // Backtrace an exception by recursively unwrapping the nested exceptions
    void backtrace( const std::exception & ex ) try
    {
        std::scoped_lock<std::mutex> sl( _trace_string_mutex );
        _latest_trace_string += std::string( ex.what() ) + "\n";
        std::rethrow_if_nested( ex );
    }
    catch( const std::exception & nested_ex )
    {
        backtrace( nested_ex );
    }

    // TODO
    void
    throw_exception( const std::string & message, const char * file, unsigned int line )
    {
        throw exception( message, file, line );
    }

    // Rethrow (creates a std::nested_exception) an exception, using the Exception
    // class which contains file and line info. The original exception is
    // preserved...
    void rethrow_exception(
        const std::string & message, const char * file, unsigned int line ) try
    {
        std::rethrow_exception( std::current_exception() );
    }
    catch( ... )
    {
        std::throw_with_nested( exception( message, file, line ) );
    }

    // General Exception handler
    void handle_exception( const std::exception & ex, const std::string & function ) try
    {
        {
            std::scoped_lock<std::mutex> sl( _trace_string_mutex );

            _latest_trace_string = "";
            if( function != "" )
                _latest_trace_string =
                    std::string( "Library API: Exception caught in function \'" ) +
                    function + "\'. ";
            _latest_trace_string += "Backtrace:\n";
        }
        backtrace( ex );
    }
    catch( ... )
    {
        std::cerr << "Something went super-wrong! TERMINATING!" << std::endl;
        std::exit( EXIT_FAILURE );
    }
}