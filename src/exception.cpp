#include <trace/exception.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

namespace trace
{
    std::string global_latest_message = "";

    // Rethrow (creates a std::nested_exception) an exception, using the Exception
    // class which contains file and line info. The original exception is
    // preserved...
    void rethrow(const std::string & message, const char * file, unsigned int line) try
    {
        std::rethrow_exception(std::current_exception());
    }
    catch (...)
    {
        std::throw_with_nested(exception(message, file, line));
    }

    // Backtrace an exception by recursively unwrapping the nested exceptions
    void backtrace(const std::exception & ex) try
    {
        global_latest_message += std::string(ex.what()) + "\n";
        // std::cerr << ex.what() << std::endl;
        rethrow_if_nested(ex);
    }
    catch (const std::exception & nested_ex)
    {
        backtrace(nested_ex);
    }

    // General Exception handler
    void Handle_Exception(const std::exception & ex, const std::string & function) try
    {
        global_latest_message = "";
        if (function != "")
            global_latest_message =
                std::string("Library API: Exception caught in function \'") + function +
                "\'\n";
        // std::cerr << "Library API: Exception caught in function \'" << function
        // << "\'" << std::endl;
        global_latest_message += "Backtrace:\n";
        // std::cerr << "Backtrace:" << std::endl;
        backtrace(ex);
    }
    catch (...)
    {
        std::cerr << "Something went super-wrong! TERMINATING!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}