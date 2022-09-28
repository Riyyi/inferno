file(GLOB_RECURSE ENGINE_SOURCES "${ENGINE}/*.cpp")

add_library(${ENGINE} ${ENGINE_SOURCES})
target_include_directories(${ENGINE} PUBLIC
	"."
	"../vendor/entt/src"
	"../vendor/glfw/include"
	"../vendor/glm"
	"../vendor/json/include"
	"../vendor/ruc/src"
	"../vendor/sol2/include"
	"../vendor/stb")
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
