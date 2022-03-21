#include "trace_exception.hpp"

// Initiation point for a backtrace
// This is as deep as the trace will go
#define initiate( message ) inner::throw_exception( message, __FILE__, __LINE__ )

// Propagation point for a backtrace
// The trace will include information on the function in which this is placed
#define propagate( message ) inner::rethrow_exception( message, __FILE__, __LINE__ )

// End point for a backtrace
// This is where the trace info is accumulated and saved,
// it can then be extracted with trace::latest_message
#define handle( ex ) inner::handle_exception( ex, __func__ )
