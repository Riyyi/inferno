add_custom_target(${ENGINE}-assets
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/copy-assets.cmake
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_dependencies(${ENGINE} ${ENGINE}-assets)
