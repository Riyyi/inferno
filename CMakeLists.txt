# User config between these lines
# ------------------------------------------

# Set engine name
set(ENGINE "inferno")
# Set project name
set(GAME "game")
# Set debugging, ON/OFF
set(DEBUG "ON")

# ------------------------------------------

# Add 'make run' target
add_custom_target(run
    COMMAND ${GAME}
)

# ------------------------------------------

cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Check if the build should include debugging symbols
option(DEBUG "" ${DEBUG})
if(DEBUG)
	# cmake -DDEBUG=on .. && make
	message("--- Debug ---")
	set(CMAKE_BUILD_TYPE "Debug")

	# -Og     = Optimizations that do not interfere with debugging
	# -Wall   = All warnings about contructions that are easily avoidable
	# -Wextra = Extra warning flags not covered by -Wall
	# -g      = Produce debugging information in OS's native format
	# -pg     = Generate profile information for analysis with gprof
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -Wall -Wextra -g -pg")
	# gprof <GAME> gmon.out > profile-data.txt
else()
	# cmake -DDEBUG=off .. && make
	message("--- Release ---")
	set(CMAKE_BUILD_TYPE "Release")

	# -O3     = Optimizations that increases compilation time and performance
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Include all headers
include_directories(
	"${ENGINE}/src"
	"${ENGINE}/vendor/entt/src"
	"${ENGINE}/vendor/glad/include"
	"${ENGINE}/vendor/glfw/include"
	"${ENGINE}/vendor/glm"
	"${ENGINE}/vendor/json/include"
	"${ENGINE}/vendor/lua"
	"${ENGINE}/vendor/sol2/include"
	"${ENGINE}/vendor/stb"
)

# Define engine source files
file(GLOB_RECURSE GLAD "${ENGINE}/vendor/glad/*.c")
file(GLOB LUA "${ENGINE}/vendor/lua/lua/*.c")
list(REMOVE_ITEM LUA "${CMAKE_SOURCE_DIR}/${ENGINE}/vendor/lua/lua/lua.c") # Do not compile stand-alone main()
file(GLOB_RECURSE ENGINE_SOURCES "${ENGINE}/src/${ENGINE}/*.cpp")
set(ENGINE_SOURCES ${GLAD} ${LUA} ${ENGINE_SOURCES})

# Define game source files
file(GLOB_RECURSE GAME_SOURCES "${GAME}/src/*.cpp")
set(GAME_SOURCES ${GAME_SOURCES})

# ------------------------------------------

project(${ENGINE})
set(CMAKE_CXX_STANDARD 17)

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW target to project
add_subdirectory(${ENGINE}/vendor/glfw)

add_library(${ENGINE} STATIC ${ENGINE_SOURCES})
target_link_libraries(${ENGINE} glfw)

# ------------------------------------------

project(${GAME})
set(CMAKE_CXX_STANDARD 17)

add_executable(${GAME} ${GAME_SOURCES})
target_link_libraries(${GAME} ${ENGINE})

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

target_precompile_headers(${GAME} REUSE_FROM ${ENGINE})
