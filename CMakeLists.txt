
cmake_minimum_required (VERSION 2.6)
project (unified_vector)

set (COMMON_COMPILE_FLAGS "-Werror -Wfatal-errors -Wall -Wno-deprecated-declarations -Wno-strict-aliasing -Wno-int-to-pointer-cast -Wno-unused-private-field -Wno-reorder -Wno-unused-variable -Wno-overloaded-virtual -std=c++14 -pedantic -Wextra" )

set(CMAKE_CXX_FLAGS "-Ofast ${COMMON_COMPILE_FLAGS}" )

include_directories ( ${unified_vector_SOURCE_DIR}/unified_vector/ )

add_executable (unified_vector_benchmark
                ${unified_vector_SOURCE_DIR}/benchmark/unified_vector_benchmark.cc)


