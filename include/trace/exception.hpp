#pragma once

#include <stdexcept>
#include <string>

namespace trace
{
    extern std::string global_latest_message;

    // Custom exception class to be used for more practical throwing
    class exception : public std::runtime_error
    {
      public:
        exception( const std::string & message, const char * file, unsigned int line )
                : std::runtime_error( message )
        {
            _message =
                std::string( file ) + ":" + std::to_string( line ) + " : " + message;
        }

        ~exception() throw() {}

        const char * what() const throw()
        {
            return _message.c_str();
        }

      private:
        std::string _message;
    };

    // Rethrow (creates a std::nested_exception) an exception, using the Exception class
    // which contains file and line info. The original exception is preserved...
    void rethrow( const std::string & message, const char * file, unsigned int line );

    // General Exception handler
    void handle_exception( const std::exception & ex, const std::string & function = "" );
}

// Shorthand for throwing an Exception with file and line info using macros
#define trace_throw( message ) throw trace::exception( message, __FILE__, __LINE__ );

// Shorthand for rethrowing and Exception with file and line info using macros
#define trace_rethrow( message ) trace::rethrow( message, __FILE__, __LINE__ );

// Shorthand for handling an exception, including a backtrace
#define trace_handle_exception( ex ) trace::handle_exception( ex, __func__ );