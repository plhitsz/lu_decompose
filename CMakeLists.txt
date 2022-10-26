cmake_minimum_required(VERSION 3.22)

project(lu_library)

# Settings and Checking
# build binay in the root of source dir IS NOT ALLOWED
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
    FATAL: In-source builds are not allowed.
           You should create a separate directory for build files.
           ")
endif()

# c++17 checking
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if (COMPILER_SUPPORTS_CXX17)
    message(STATUS "c++17 supported")
else ()
    message(FATAL_ERROR "
            FATAL: gcc or g++ do not support c++17")
endif ()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Release default
# change it to Debug by `cmake -DCMAKE_BUILD_TYPE=Debug ..`
option(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -O3 -fPIC")
###########################################################

# more compile info dump if set it to true
set(CMAKE_VERBOSE_MAKEFILE false)

# google test case
if (NOT CMAKE_CROSSCOMPILING)
    include(GoogleTest)
    enable_testing()
    add_custom_target(all_test ${CMAKE_CTEST_COMMAND} -V)
    set(CTEST_OUTPUT_ON_FAILURE true)
    set(GTEST_COLOR true)
endif (NOT CMAKE_CROSSCOMPILING)

# library dir for installing
set(CMAKE_BUILD_DIR ${CMAKE_BINARY_DIR}/_deps)
include_directories(src)
include_directories(${CMAKE_BUILD_DIR}/googletest-src/googletest/include)

set(CMAKE_C_VISIBILITY_PRESET default)
set(CMAKE_CXX_VISIBILITY_PRESET default)

#Generate the shared library from the library sources #STATIC
add_library(lu_library SHARED
    src/lu.cc
)

add_library(math::lu_library ALIAS lu_library)

target_include_directories(lu_library
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
# link the new lu_library target with the test_binary target
target_link_libraries(lu_library
)

# Dependency
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)