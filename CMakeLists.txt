######### CMake Version ###################################
cmake_minimum_required(VERSION 3.10)
###########################################################


######### Info ############################################
message( STATUS ">> CMAKE_C_COMPILER:     ${CMAKE_C_COMPILER}"   )
message( STATUS ">> CMAKE_CXX_COMPILER:   ${CMAKE_CXX_COMPILER}" )
###########################################################


######### Project Name ####################################
project( trace )
set( LIBRARY_NAME trace )
###########################################################


######### Prevent messing up the source tree ##############
set( CMAKE_DISABLE_SOURCE_CHANGES  ON )
set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
###########################################################


######### Flags ###########################################
set( TRACE_COMPILE_FLAGS  )
set( TRACE_LINK_FLAGS     )
set( TRACE_FLAGS_COVERAGE )
###########################################################


######### trace library ###################################
set(TRACE_HEADER_FILES
    include/trace/trace.hpp
    include/trace/exception.hpp
)

set(TRACE_SOURCE_FILES
    src/exception.cpp
)

add_library( ${LIBRARY_NAME} ${TRACE_SOURCE_FILES} )

set_target_properties( ${LIBRARY_NAME} PROPERTIES
    CXX_STANDARD          17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF )

target_include_directories( ${LIBRARY_NAME} PUBLIC  ${PROJECT_SOURCE_DIR}/include)
# target_include_directories( ${LIBRARY_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/thirdparty)

target_compile_options( ${LIBRARY_NAME} PRIVATE
                        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                            -Werror -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion >
                        $<$<CXX_COMPILER_ID:MSVC>:
                            /WX /W4 >)
###########################################################


######### Tests ###########################################
enable_testing( )

set( TEST_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )

set( TEST_SOURCES
    tests/main.cpp
    tests/should_throw.cpp )

# Executable
add_executable( test_trace ${TEST_SOURCES} )
target_link_libraries( test_trace ${LIBRARY_NAME} )

# Properties
set_target_properties( test_trace PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_RUNTIME_OUTPUT_DIRECTORY}
    CXX_STANDARD            17
    CXX_STANDARD_REQUIRED   ON
    CXX_EXTENSIONS          OFF
    COMPILE_FLAGS           "${TRACE_COMPILE_FLAGS} ${TRACE_FLAGS_COVERAGE}"
    LINK_FLAGS              "${TRACE_LINK_FLAGS}    ${TRACE_FLAGS_COVERAGE}" )

# Add the test
add_test( NAME              test_trace
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          COMMAND           test_trace --use-colour=yes )
###########################################################


######### Examples ########################################
# add_executable( ${EXECUTABLE_NAME} main.cpp )

# set_target_properties( ${EXECUTABLE_NAME} PROPERTIES
#     CXX_STANDARD          11
#     CXX_STANDARD_REQUIRED ON
#     CXX_EXTENSIONS        OFF )

# target_link_libraries( ${EXECUTABLE_NAME} PUBLIC trace)
###########################################################