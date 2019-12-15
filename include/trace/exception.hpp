#pragma once

#include <mutex>
#include <stdexcept>
#include <string>

namespace trace
{
    // Retrieve the latest (error) message
    std::string latest();
}

namespace trace::inner
{
    extern std::string _latest_trace_string;
    extern std::mutex _trace_string_mutex;

    // Throw a trace::inner::exception
    void
    throw_exception( const std::string & message, const char * file, unsigned int line );

    // Rethrow (creates a std::nested_exception) an exception, using the Exception class
    // which contains file and line info. The original exception is preserved...
    void rethrow_exception(
        const std::string & message, const char * file, unsigned int line );

    // General exception handler
    void handle_exception( const std::exception & ex, const std::string & function = "" );

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
}