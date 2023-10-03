cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 99)
set(C_VERSION_NAME "c_std_${CMAKE_C_STANDARD}")
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# TOGGLE TESTING.
set(MAIN_TEST OFF)

# TOGGLE EXAMPLES.
set(EXAMPLES OFF)

# SETTING PROJECT VERSION.
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_TWEAK 0)
set(PROJECT_VERSION_SUFFIX "a")
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK})
set(PROJECT_FULL_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}.${PROJECT_VERSION_SUFFIX})

# CREATING PROJECT.
project(C-DURLIB-LIBRARY VERSION ${PROJECT_VERSION} LANGUAGES C)

# RUN SETUP.PY TO ASSIGN BUILD TYPE.
# execute_process(
# COMMAND python "${PROJECT_SOURCE_DIR}/Tools/build.py" "-cag" "build_type"
# OUTPUT_VARIABLE BUILD_TYPE_SETUP_OUTPUT
# RESULT_VARIABLE BUILD_TYPE_SETUP_RESULT
# OUTPUT_STRIP_TRAILING_WHITESPACE
# )
# set(CMAKE_BUILD_TYPE ${BUILD_TYPE_SETUP_OUTPUT})
set(CMAKE_BUILD_TYPE "Debug")

# SET INSTALLATION PATH BASED ON BUILD TYPE.
# SETTING BUILD TYPE.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(INSTALL_PATH_SUFFIX "Debug")
        set(CMAKE_BUILD_TYPE Debug)

        # TOGGLE CUSTOM VERBOSITY.
        set(VERBOSE ON CACHE INTERNAL "Verbosity on")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(INSTALL_PATH_SUFFIX "Release")

        # TOGGLE CUSTOM VERBOSITY.
        set(VERBOSE OFF CACHE INTERNAL "Verbosity off")
else()
        set(INSTALL_PATH_SUFFIX "Unknown")

        # TOGGLE CUSTOM VERBOSITY.
        set(VERBOSE ON CACHE INTERNAL "Verbosity on")
endif()

# PATH TO INSTALLATION DIRECTORY.
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/Install/${CMAKE_BUILD_TYPE}" CACHE PATH "Installation Directory" FORCE)

if(${VERBOSE})
        message(STATUS "INSTALLATION SETUP PROCESS STARTING...")
        message(STATUS "DEFAULT INSTALLATION DIRECTORY: \"${CMAKE_INSTALL_PREFIX}\".")
endif()

# CHECKING PLATFORM.
set(CURRENT_PLATFORM "${CMAKE_SYSTEM_NAME}")

if(${VERBOSE})
        message(STATUS "CURRENT PLATFORM: ${CMAKE_SYSTEM_NAME}")
endif()

# DOUBLE CHECKS IF THE SYSTEM IS RUNNING ON WINDOWS-X64 SYSTEM.
# THIS ALSO FAILS IF THE COMPILER IS NOT 64-BIT.
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "C-DURLIB SUPPORTS X64 SYSTEMS ONLY.")
        return()
endif()

# SETTING UP COMPILER.
if(${VERBOSE})
        message(STATUS "SETTING UP COMPILER...")
endif()

string(TOLOWER "${CMAKE_C_COMPILER_ID}" LOWER_COMPILER_ID)

if(LOWER_COMPILER_ID STREQUAL "msvc")
        set(CURRENT_COMPILER "MSVC")
elseif(LOWER_COMPILER_ID STREQUAL "gnu")
        set(CURRENT_COMPILER "GNU")
elseif(LOWER_COMPILER_ID STREQUAL "clang")
        set(CURRENT_COMPILER "CLANG")
else()
        message(FATAL_ERROR "COMPILER NOT SUPPORTED.")
endif()

string(TOLOWER "${CMAKE_SYSTEM_NAME}" LOWER_SYSTEM_NAME)

if(LOWER_SYSTEM_NAME STREQUAL "windows")
        set(COMPILATION_PLATFORM "WIN32")
elseif(LOWER_SYSTEM_NAME STREQUAL "linux")
        set(COMPILATION_PLATFORM "UNIX")
elseif(LOWER_SYSTEM_NAME STREQUAL "darwin")
        set(COMPILATION_PLATFORM "UNIX")
else()
        message(FATAL_ERROR "SYSTEM NOT SUPPORTED.")
endif()

if(${VERBOSE})
        message(STATUS "CURRENT COMPILER PATH: ${CMAKE_C_COMPILER}")
        message(STATUS "CURRENT COMPILER: ${CMAKE_C_COMPILER_ID}")
        message(STATUS "CURRENT COMPILER VERSION: ${CMAKE_C_COMPILER_VERSION}")
        message(STATUS "COMPILER SETUP COMPLETE.")
endif()

# DEPENDENCY MANAGEMENT.

# ADDING TESTING ENVIRONMENT.
if({MAIN_TEST})
        if(${VERBOSE})
                message(STATUS "ADDING TESTING ENVIRONMENT...")
        endif()

        include(CTest)
        enable_testing()
        set(CPACK_PROJECT_NAME ${PROJECT_NAME})
        set(CPACK_PROJECT_VERSION ${PROJECT_FULL_VERSION})
        include(CPack)

        if(${VERBOSE})
                message(STATUS "TESTING ENVIRONMENT ADDED.")
        endif()
endif()

# ADDITIONAL INFORMATION.
if(${VERBOSE})
        message(STATUS "CURRENT BUILD TYPE: ${CMAKE_BUILD_TYPE}")
        message(STATUS "PROJECT VERSION: \"${PROJECT_FULL_VERSION}\".")
        message(STATUS "TOOLCHAIN STATUS: \"${CMAKE_TOOLCHAIN_FILE}\".")
        message(STATUS "CMAKE INCLUDE CURRENT DIRECTORY: \"${CMAKE_INCLUDE_CURRENT_DIR}\".")

        if({MAIN_TEST})
                message(STATUS "CTEST STATUS: \"${CPACK_PROJECT_NAME}\" VERSION \"${CPACK_PROJECT_VERSION}\".")
        endif()
endif()

# CHECK IF SYSTEMS IS RUNNING WINDOWS AND ADD NECESSARY ATTRIBUTES.
# EVEN THOUGH CURRENTLY ONLY WINDOWS IS SUPPORTED, ADDING THIS CHECK WILL MAKE IT EASIER TO ADD SUPPORT FOR OTHER SYSTEMS IN THE FUTURE.
if(COMPILATION_PLATFORM STREQUAL "WIN32")
        # ADD SOURCE FILES
        # C-DURLIB .C FILES
        file(GLOB_RECURSE C-DURLIB_C
                "${PROJECT_SOURCE_DIR}/c-durlib/src/log/*.c"
        )

        # C-DURLIB .H FILES
        file(GLOB_RECURSE C-DURLIB_H
                "${PROJECT_SOURCE_DIR}/c-durlib/src/log/*.h"
        )

        # C-DURLIB TESTING .C FILES
        file(GLOB_RECURSE C-DURLIB-TESTING_C

                "${PROJECT_SOURCE_DIR}/c-durlib/src/testing/*.c"
        )

        # C-DURLIB TESTING .H FILES
        file(GLOB_RECURSE C-DURLIB-TESTING_H

                "${PROJECT_SOURCE_DIR}/c-durlib/src/testing/*.h"
        )

        if(${VERBOSE})
                message(STATUS "C-DURLIB SOURCE FILES ADDED.")
        endif()

        # add_library(C-DURLIB STATIC ${C-DURLIB_C})
        add_executable(C-DURLIB

                ${C-DURLIB_C} ${C-DURLIB_H}
                ${C-DURLIB-TESTING_C} ${C-DURLIB-TESTING_H}
                "${PROJECT_SOURCE_DIR}/c-durlib/src/main.c")
        target_compile_features(C-DURLIB PUBLIC ${C_VERSION_NAME})
        set_target_properties(C-DURLIB PROPERTIES VERSION ${PROJECT_FULL_VERSION})

        set_target_properties(C-DURLIB PROPERTIES OUTPUT_NAME "C-DURLIB")
        target_link_libraries(C-DURLIB PUBLIC
        )

        target_precompile_headers(C-DURLIB PUBLIC

                # "${PROJECT_SOURCE_DIR}/c-durlib/src/PCH.h"
        )

        target_include_directories(C-DURLIB PUBLIC
                ${C-DURLIB_H}
                "${PROJECT_SOURCE_DIR}/c-durlib/src/"
        )

        # COMPILER DEFINITIONS FOR C-DURLIB.
        target_compile_definitions(C-DURLIB PUBLIC C_DURLIB_PLATFORM_WINDOWS)
        target_compile_definitions(C-DURLIB PUBLIC C_DURLIB_STATIC_LIBRARY)
        target_compile_definitions(C-DURLIB PUBLIC C_DURLIB_ENABLE_ASSERTS)
        target_compile_definitions(C-DURLIB PUBLIC C_DURLIB_COMPILER_${CURRENT_COMPILER})

        # ENABLE PROFILING FOR DEBUG BUILDS.
        if(CMAKE_BUILD_TYPE STREQUAL Debug)
                target_compile_definitions(C-DURLIB PUBLIC C_DURLIB_DEBUG_PROFILING)
                target_compile_definitions(C-DURLIB PUBLIC C_DURLIB_ENABLE_DEBUG_LOG)
        endif(CMAKE_BUILD_TYPE STREQUAL Debug)

        if(${VERBOSE})
                get_target_property(DEFINITIONS_LIST C-DURLIB COMPILE_DEFINITIONS)
                message(STATUS "C-DURLIB COMPILE DEFINITIONS: ${DEFINITIONS_LIST}")
        endif()

        if(${MAIN_TEST})
                add_subdirectory("${PROJECT_SOURCE_DIR}/tests")
        endif()

        if(${EXAMPLES})
                add_subdirectory("${PROJECT_SOURCE_DIR}/examples")
        endif()

        # INSTALLATION PROCEDURE.
        install(TARGETS C-DURLIB
                LIBRARY DESTINATION bin
                ARCHIVE DESTINATION bin
                RUNTIME DESTINATION bin)

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        endif()
endif()

message(STATUS "C-DURLIB/CMAKE SUCCESSFULLY FINISHED.")
