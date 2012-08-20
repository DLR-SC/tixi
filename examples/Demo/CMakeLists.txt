# TIXI Library
# @author Martin Siggel

set(DEMO_SRC tixiDemo.c)

include_directories(${PROJECT_SOURCE_DIR}/src)

if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_C_FLAGS "-Wall -fmessage-length=0")
endif()

add_executable(tixiDemo EXCLUDE_FROM_ALL ${DEMO_SRC})
target_link_libraries (tixiDemo TIXI)

add_custom_command(
	TARGET tixiDemo
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/howtoin.xml ${CMAKE_CURRENT_BINARY_DIR}/howtoin.xml
)
