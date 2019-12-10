# User config between these lines
# ------------------------------------------

# Set engine name
set(ENGINE "engine")
# Set project name
set(GAME "game")

# ------------------------------------------

cmake_minimum_required(VERSION 3.1)

# Check if the build should include debugging symbols
option(DEBUG "" ON)
if(DEBUG)
	# cmake -DDEBUG=on .. && make
	message("--- Debug ---")
	set(CMAKE_BUILD_TYPE "Debug")

	# -Og     = Optimizations that do not interfere with debugging
	# -Wall   = All warnings about contructions that are easily avoidable
	# -Wextra = Extra warning flags not covered by -Wall
	# -pg     = Generate profile information for analysis with gprof
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -Wall -Wextra -pg")
	# gprof <GAME> gmon.out > profile-data.txt
else()
	# cmake -DDEBUG=off .. && make
	message("--- Release ---")
	set(CMAKE_BUILD_TYPE "Release")

	# -O3     = Optimizations that increase compilation time and performance
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Include all headers
include_directories(
	"${ENGINE}/vendor/glad/include"
	"${ENGINE}/vendor/glfw/include"
	"${ENGINE}/vendor/glm"
	"${ENGINE}/"
)

# Add source files
file(GLOB_RECURSE GLAD "${ENGINE}/vendor/glad/*.c")
file(GLOB_RECURSE ENGINE_SOURCES "${ENGINE}/src/*.cpp")
file(GLOB_RECURSE GAME_SOURCES "${GAME}/src/*.cpp")
set(SOURCES ${GLAD} ${ENGINE_SOURCES} ${GAME_SOURCES})

# ------------------------------------------

project(${ENGINE})
set(CMAKE_CXX_STANDARD 14)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW target to project
add_subdirectory(engine/vendor/glfw)

add_library(${ENGINE} STATIC ${SOURCES})
target_link_libraries(${ENGINE} glfw)

# ------------------------------------------

project(${GAME})
set(CMAKE_CXX_STANDARD 14)

add_executable(${GAME} ${SOURCES})
target_link_libraries(${GAME} ${ENGINE})
