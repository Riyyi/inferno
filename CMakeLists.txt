# ------------------------------------------
# User config between these lines

# Set engine name
set(ENGINE "inferno")

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(INFERNO_STANDALONE TRUE)
endif()

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(INFERNO_BUILD_EXAMPLES "Build the Inferno example programs" ${INFERNO_STANDALONE})

# ------------------------------------------

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(${ENGINE} CXX C)

# ------------------------------------------
# Setup C++ compiler

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags used for all build types
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
# -Wall      = All warnings about contructions that are easily avoidable
# -Wextra    = Extra warning flags not covered by -Wall
# -Wpedantic = Warnings for compiler extensions not part of the standard

# Set default build type if not specified
set(DEFAULT_BUILD_TYPE Release)
if(EXISTS ${CMAKE_SOURCE_DIR}/.git)
	set(DEFAULT_BUILD_TYPE Debug)
endif()
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE})
endif()

# Set build type specific compiler flags
message("--- ${CMAKE_BUILD_TYPE} ---")
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g -pg")
	# -Og = Optimizations that do not interfere with debugging
	# -g  = Produce debugging information in OS's native format
	# -pg = Generate profile information for analysis with gprof
	# $ gprof <PROJECT> gmon.out > profile-data.txt
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	# -O3 = Optimizations that increase compilation time and performance
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------
# Engine target

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW target to project
add_subdirectory(${ENGINE}/vendor/glfw)

# Define engine source files
file(GLOB_RECURSE GLAD "${ENGINE}/vendor/glad/*.c")
file(GLOB LUA "${ENGINE}/vendor/lua/lua/*.c")
list(REMOVE_ITEM LUA "${CMAKE_SOURCE_DIR}/${ENGINE}/vendor/lua/lua/onelua.c") # Do not compile single file variant
list(REMOVE_ITEM LUA "${CMAKE_SOURCE_DIR}/${ENGINE}/vendor/lua/lua/ltests.c") # Do not compile internal debugging
list(REMOVE_ITEM LUA "${CMAKE_SOURCE_DIR}/${ENGINE}/vendor/lua/lua/lua.c")    # Do not compile interpreter
file(GLOB_RECURSE ENGINE_SOURCES "${ENGINE}/src/${ENGINE}/*.cpp")
set(ENGINE_SOURCES ${GLAD} ${LUA} ${ENGINE_SOURCES})

add_library(${ENGINE} ${ENGINE_SOURCES})
target_include_directories(${ENGINE} PRIVATE
	"${ENGINE}/src"
	"${ENGINE}/vendor/entt/src"
	"${ENGINE}/vendor/glad/include"
	"${ENGINE}/vendor/glfw/include"
	"${ENGINE}/vendor/glm"
	"${ENGINE}/vendor/json/include"
	"${ENGINE}/vendor/lua"
	"${ENGINE}/vendor/sol2/include"
	"${ENGINE}/vendor/stb")
target_link_libraries(${ENGINE} glfw)

# ------------------------------------------

target_precompile_headers(${ENGINE} PRIVATE
	"$<$<COMPILE_LANGUAGE:CXX>:<algorithm$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<array$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<csignal$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<cstddef$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<cstdint$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<functional$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<iostream$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<memory$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<ostream$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<sstream$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<string$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<string_view$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<unordered_map$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<utility$<ANGLE-R>>"
	"$<$<COMPILE_LANGUAGE:CXX>:<vector$<ANGLE-R>>"
)

# ------------------------------------------
# Examples target

if (INFERNO_BUILD_EXAMPLES)
	# Add examples target to project
	add_subdirectory("${CMAKE_SOURCE_DIR}/example")
endif()
