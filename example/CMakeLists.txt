# ------------------------------------------
# User config between these lines

# Set project name
set(GAME "game")

# ------------------------------------------

project(${GAME} CXX)

# Define game source files
file(GLOB_RECURSE GAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${GAME} ${GAME_SOURCES})
target_include_directories(${GAME} PRIVATE
	"src"
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/vendor/ruc/src")
target_link_libraries(${GAME} ${ENGINE})

target_precompile_headers(${GAME} REUSE_FROM ${ENGINE})

# ------------------------------------------

# Add 'make run' target
add_custom_target(run
    COMMAND ${GAME}
	WORKING_DIRECTORY ".."
)
