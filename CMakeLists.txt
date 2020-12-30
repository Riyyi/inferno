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

cmake_minimum_required(VERSION 3.1)

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
	"${ENGINE}/vendor/freetype/include"
	"${ENGINE}/vendor/glad/include"
	"${ENGINE}/vendor/glfw/include"
	"${ENGINE}/vendor/glm"
	"${ENGINE}/vendor/json/include"
	"${ENGINE}/vendor/stb"
	"${ENGINE}/src"
)

# Define engine source files
file(GLOB_RECURSE GLAD "${ENGINE}/vendor/glad/*.c")
file(GLOB_RECURSE ENGINE_SOURCES "${ENGINE}/src/${ENGINE}/*.cpp")
set(ENGINE_SOURCES ${GLAD} ${ENGINE_SOURCES})

# Define game source files
file(GLOB_RECURSE GAME_SOURCES "${GAME}/src/*.cpp")
set(GAME_SOURCES ${GAME_SOURCES})

# ------------------------------------------

project(${ENGINE})
set(CMAKE_CXX_STANDARD 17)

# GLFW
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
