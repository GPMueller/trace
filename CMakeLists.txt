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
