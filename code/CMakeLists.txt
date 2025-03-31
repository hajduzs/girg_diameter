cmake_minimum_required(VERSION 3.10)

project(LocalityHeterogeneity)

# setting the build type to debug when in the debug directory and to
# release otherwise
get_filename_component(build_dir ${CMAKE_CURRENT_BINARY_DIR} NAME)
if (${build_dir} MATCHES "debug")
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

# c++ version 20
set(CMAKE_CXX_STANDARD 20)

# export compile_commands.json, which can be used by editors for auto
# completion (e.g., using lsp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# helping the compiler to find header files
include_directories(include lib)

# header, source and executable files
set(HEADERS
  lib/CLI11.hpp
  include/framework/app.hpp
  include/framework/bucket_heap.hpp
  include/framework/graph.hpp
  include/framework/graph.hpp
  include/framework/graph_parser.hpp
  include/framework/graph_writer.hpp
  include/framework/graph_operations.hpp
  include/framework/component_structure.hpp
  include/framework/timer.hpp
  include/framework/random.hpp
  include/framework/range.hpp
  include/stats/ecc_stats.hpp
  include/bfs/bfs.hpp
  include/bfs/bfs_bi.hpp
  include/bfs/bfs_bi_balanced.hpp
  include/bfs/bfs_bi_always_swap.hpp
  include/generator/random_graph.hpp
  include/diameter/bounds.hpp
  include/diameter/eccentricity.hpp
  include/diameter/exact.hpp
  include/diameter/sweep.hpp
  )

set(SOURCE_FILES
  src/framework/bucket_heap.cpp
  src/framework/graph.cpp
  src/framework/graph_parser.cpp
  src/framework/graph_writer.cpp
  src/framework/graph_operations.cpp
  src/framework/component_structure.cpp
  src/framework/timer.cpp
  src/framework/random.cpp
  src/stats/ecc_stats.cpp
  src/bfs/bfs.cpp
  src/bfs/bfs_bi.cpp
  src/bfs/bfs_bi_balanced.cpp
  src/bfs/bfs_bi_always_swap.cpp
  src/generator/random_graph.cpp
  src/diameter/bounds.cpp
  src/diameter/eccentricity.cpp
  src/diameter/exact.cpp
  src/diameter/sweep.cpp
  )

set(EXECUTABLE_FILES
  cli/branching.cpp
  cli/chung_lu.cpp
  cli/prepare_graph_formats.cpp
  cli/random_graph.cpp
  cli/diameter_bounds.cpp
  cli/diameter_exact.cpp
  cli/girg.cpp
  cli/ecc_samples.cpp
  )

# create a target for every executable linking all source files
add_library(code STATIC ${SOURCE_FILES} ${HEADERS})
foreach (file ${EXECUTABLE_FILES})
    get_filename_component(TARGET ${file} NAME_WE)
    add_executable(${TARGET} ${file})
    target_link_libraries(${TARGET} code)

    # show all warnings
    if(MSVC)
      target_compile_options(${TARGET} PRIVATE /W4 /WX)
    else()
      target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach ()

# use girgs library
set(OPTION_BUILD_CLI OFF)
set(CMAKE_POLICY_DEFAULT_CMP0120 OLD)
add_subdirectory(../girgs girgs)

target_include_directories(girg PRIVATE ../girgs/source/girgs/include)
target_link_libraries(girg girgs)
