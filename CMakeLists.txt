######### CMake Version ###################################
cmake_minimum_required(VERSION 3.10)
###########################################################


######### Options #########################################
option( TRACE_TEST_COVERAGE "Build trace tests with coverage flags" OFF )
###########################################################


######### Project Name ####################################
project( Trace LANGUAGES CXX VERSION 0.1 )
set( LIBRARY_NAME trace )
###########################################################


######### Info ############################################
message( STATUS ">> CMAKE_CXX_COMPILER:   ${CMAKE_CXX_COMPILER}" )
###########################################################


######### Prevent messing up the source tree ##############
set( CMAKE_DISABLE_SOURCE_CHANGES  ON )
set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
### Output paths for multi-config builds (e.g. MSVC)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_BINARY_DIR} )
endforeach()
###########################################################


######### Flags ###########################################
set( TRACE_COMPILE_FLAGS  )
set( TRACE_LINK_FLAGS     )
set( TRACE_FLAGS_COVERAGE )

if( TRACE_TEST_COVERAGE )
    set( TRACE_FLAGS_COVERAGE
        ${CMAKE_CXX_FLAGS_DEBUG}
        --coverage
        -fno-inline
        -fno-inline-small-functions
        -fno-default-inline )
    string( REPLACE ";" " " TRACE_FLAGS_COVERAGE "${TRACE_FLAGS_COVERAGE}" )
    message( STATUS ">> Trace coverage flags: ${TRACE_FLAGS_COVERAGE}" )
endif()
###########################################################


######### trace library ###################################
set( TRACE_HEADER_FILES
    include/trace/trace.hpp
    include/trace/trace_exception.hpp
)

set( TRACE_SOURCE_FILES
    src/trace_exception.cpp
)

add_library( ${LIBRARY_NAME} ${TRACE_SOURCE_FILES} )

include( GNUInstallDirs )

set_target_properties( ${LIBRARY_NAME} PROPERTIES
    CXX_STANDARD          17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
    COMPILE_FLAGS         "${TRACE_COMPILE_FLAGS} ${TRACE_FLAGS_COVERAGE}"
    LINK_FLAGS            "${TRACE_LINK_FLAGS}    ${TRACE_FLAGS_COVERAGE}"
    VERSION               "${${PROJECT_NAME}_VERSION}"
    PUBLIC_HEADER         "${TRACE_HEADER_FILES}" )

target_include_directories( ${LIBRARY_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                   "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>" )

target_compile_options( ${LIBRARY_NAME} PRIVATE
                        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                            -Werror -Wall -Wextra -Wpedantic -pedantic-errors -Wconversion -Wsign-conversion >
                        $<$<CXX_COMPILER_ID:MSVC>:
                            /WX /W4 > )

######### installation ####################################

install( TARGETS       ${LIBRARY_NAME}
         EXPORT        ${PROJECT_NAME}Targets
         LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shared_lib
         ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT static_lib
         RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT executable
         PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_NAME}"        COMPONENT headers)

set( ${PROJECT_NAME}_CMAKE_FILES_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_NAME}" )

install( EXPORT      ${PROJECT_NAME}Targets
         NAMESPACE   ${PROJECT_NAME}::
         FILE        ${PROJECT_NAME}Targets.cmake
         DESTINATION "${${PROJECT_NAME}_CMAKE_FILES_DESTINATION}" )

file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/Config.cmake.in" [[
@PACKAGE_INIT@

include ( "${CMAKE_CURRENT_LIST_DIR}/TraceTargets.cmake" )
]] )

include( CMakePackageConfigHelpers )

configure_package_config_file(
    "${CMAKE_CURRENT_BINARY_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${${PROJECT_NAME}_CMAKE_FILES_DESTINATION}"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO )

write_basic_package_version_file (
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION       "${${PROJECT_NAME}_VERSION}"
  COMPATIBILITY AnyNewerVersion
)

install( FILES       "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                     "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
         DESTINATION "${${PROJECT_NAME}_CMAKE_FILES_DESTINATION}" )

export( EXPORT ${PROJECT_NAME}Targets
        FILE   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake" )
###########################################################


######### Tests ###########################################
option( BUILD_TESTING ON )
if ( BUILD_TESTING )
    enable_testing()

    set( TEST_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )

    set( TEST_SOURCES
        tests/main.cpp
        tests/should_not_throw.cpp
        tests/should_throw.cpp )

    # Executable
    add_executable( test_trace ${TEST_SOURCES} )
    target_link_libraries( test_trace ${LIBRARY_NAME} )

    # Properties
    set_target_properties( test_trace PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_RUNTIME_OUTPUT_DIRECTORY}
        CXX_STANDARD          17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS        OFF
        COMPILE_FLAGS         "${TRACE_COMPILE_FLAGS} ${TRACE_FLAGS_COVERAGE}"
        LINK_FLAGS            "${TRACE_LINK_FLAGS}    ${TRACE_FLAGS_COVERAGE}" )

    # Add the test
    add_test( NAME              test_trace
              WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
              COMMAND           test_trace --use-colour=yes )
endif()
###########################################################


######### Examples ########################################
# add_executable( ${EXECUTABLE_NAME} main.cpp )

# set_target_properties( ${EXECUTABLE_NAME} PROPERTIES
#     CXX_STANDARD          11
#     CXX_STANDARD_REQUIRED ON
#     CXX_EXTENSIONS        OFF )

# target_link_libraries( ${EXECUTABLE_NAME} PUBLIC trace )
###########################################################
