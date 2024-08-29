# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW target to project
add_subdirectory(glfw)

# Add RUC target to project
add_subdirectory(ruc)

# assimp options
option(ASSIMP_BUILD_ASSIMP_VIEW OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_INSTALL OFF)
option(ASSIMP_INSTALL_PDB OFF)
option(ASSIMP_WARNINGS_AS_ERRORS OFF)

# Add assimp target to project
add_subdirectory(assimp)

# Define engine source files
file(GLOB_RECURSE GLAD "glad/*.c")
file(GLOB LUA "lua/lua/*.c")
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/lua/lua/onelua.c") # Do not compile single file variant
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/lua/lua/ltests.c") # Do not compile internal debugging
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/lua/lua/lua.c")    # Do not compile interpreter
set(ENGINE_SOURCES ${GLAD} ${LUA})

add_library(${ENGINE}-dependencies ${ENGINE_SOURCES})
target_include_directories(${ENGINE}-dependencies PUBLIC
	"assimp/include"
	"glad/include"
	"lua")
target_link_libraries(${ENGINE}-dependencies glfw ruc ruc-test assimp)
target_compile_options(${ENGINE}-dependencies PRIVATE ${COMPILE_FLAGS_DEPS})
