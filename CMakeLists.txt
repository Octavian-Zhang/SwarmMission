cmake_minimum_required(VERSION 3.23)
project(SwarmMission)

# add compiler option to use C++11 features
set (CMAKE_CXX_STANDARD 11)

# set preprocessor flag to include pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# include the mission algorithm include directory
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

# glob source code files
file(GLOB_RECURSE SwarmMission_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

# add source to this project's shared library.
add_library (SwarmMission ${SwarmMission_SRC})

# Define target link libraries
target_link_libraries (SwarmMission rt Threads::Threads)

# state that anybody linking to us needs to include the current source dir
# to find necessary headers, while we don't.
target_include_directories (SwarmMission INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)