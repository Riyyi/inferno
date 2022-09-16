# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add GLFW target to project
add_subdirectory(vendor/glfw)

# Define engine source files
file(GLOB_RECURSE GLAD "vendor/glad/*.c")
file(GLOB LUA "vendor/lua/lua/*.c")
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua/lua/onelua.c") # Do not compile single file variant
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua/lua/ltests.c") # Do not compile internal debugging
list(REMOVE_ITEM LUA "${CMAKE_CURRENT_SOURCE_DIR}/vendor/lua/lua/lua.c")    # Do not compile interpreter
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
set(ENGINE_SOURCES ${GLAD} ${LUA} ${ENGINE_SOURCES})

add_library(${ENGINE} ${ENGINE_SOURCES})
target_include_directories(${ENGINE} PRIVATE
	"src"
	"vendor/entt/src"
	"vendor/glad/include"
	"vendor/glfw/include"
	"vendor/glm"
	"vendor/json/include"
	"vendor/lua"
	"vendor/sol2/include"
	"vendor/stb")
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
