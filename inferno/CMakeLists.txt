file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

add_library(${ENGINE} ${ENGINE_SOURCES})
target_include_directories(${ENGINE} PRIVATE
	"src"
	"${CMAKE_SOURCE_DIR}/vendor/entt/src"
	"${CMAKE_SOURCE_DIR}/vendor/glad/include"
	"${CMAKE_SOURCE_DIR}/vendor/glfw/include"
	"${CMAKE_SOURCE_DIR}/vendor/glm"
	"${CMAKE_SOURCE_DIR}/vendor/json/include"
	"${CMAKE_SOURCE_DIR}/vendor/lua"
	"${CMAKE_SOURCE_DIR}/vendor/sol2/include"
	"${CMAKE_SOURCE_DIR}/vendor/stb")
target_link_libraries(${ENGINE} ${ENGINE}-dependencies)

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
