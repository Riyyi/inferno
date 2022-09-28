# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW target to project
add_subdirectory(glfw)

# Add RUC target to project
add_subdirectory(ruc)

# Define engine source files
file(GLOB_RECURSE GLAD "glad/*.c")
file(GLOB LUA "lua/lua/*.c")
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/lua/lua/onelua.c") # Do not compile single file variant
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/lua/lua/ltests.c") # Do not compile internal debugging
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/lua/lua/lua.c")    # Do not compile interpreter
set(ENGINE_SOURCES ${GLAD} ${LUA})

add_library(${ENGINE}-dependencies ${ENGINE_SOURCES})
target_include_directories(${ENGINE}-dependencies PUBLIC
	"glad/include"
	"lua")
target_link_libraries(${ENGINE}-dependencies glfw ruc)
