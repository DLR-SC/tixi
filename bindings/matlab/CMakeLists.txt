# Matlab bindings for tixi
# @author Martin Siggel

find_package(MATLAB QUIET)
find_package(PythonInterp)

if(PYTHONINTERP_FOUND)
message(STATUS "TiXI MATLAB interface files are created (*.m, tiximatlab.c).")

#run python to create the tiglmatlab.c
add_custom_command(
	OUTPUT tiximatlab.c
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/make_tixi_matlab.py ${PROJECT_SOURCE_DIR}/src/tixi.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${PYTHON_EXECUTABLE}
	ARGS ${CMAKE_CURRENT_SOURCE_DIR}/make_tixi_matlab.py
)

add_custom_target(tixi_mex_interface ALL
	COMMENT "Create MATLAB mex source for tixi" VERBATIM
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tiximatlab.c
)

configure_file(Makefile.in ${CMAKE_CURRENT_BINARY_DIR}/Makefile @ONLY)

install(
	CODE "file( GLOB MFILES ${CMAKE_CURRENT_BINARY_DIR}/*.m )"
	CODE "file( INSTALL \${MFILES} DESTINATION \${CMAKE_INSTALL_PREFIX}/share/tixi/matlab )"
	COMPONENT interfaces
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/tiximatlab.c
	${CMAKE_CURRENT_SOURCE_DIR}/common.h
	${CMAKE_CURRENT_SOURCE_DIR}/Readme.md
	${CMAKE_CURRENT_BINARY_DIR}/Makefile
	DESTINATION share/tixi/matlab
	COMPONENT interfaces)


if(MATLAB_FOUND)
	# If matlab is found, the tiximatlab.c file is compiled into a
	# mex file and is installed

	message(STATUS "MATLAB found: TiXI mex file will be build.")

	# c wrapper code
	set(TMAT_SRC tiximatlab.c common.h)

	# add def for exports
	if(MSVC)
	  set(TMAT_SRC ${TMAT_SRC} exports.def)
	endif()

	include_directories(${PROJECT_SOURCE_DIR}/src ${MATLAB_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

	add_library(tixi_matlab SHARED ${TMAT_SRC})
	target_link_libraries(tixi_matlab ${MATLAB_mat_LIBRARY} ${MATLAB_mx_LIBRARY} ${MATLAB_mex_LIBRARY} TIXI)
	set_target_properties(tixi_matlab PROPERTIES SUFFIX ${MATLAB_MEX_SUFFIX} PREFIX "")

	install(TARGETS tixi_matlab DESTINATION share/tixi/matlab
		COMPONENT interfaces)
else(MATLAB_FOUND)
	message(STATUS "MATLAB not found: The TiXI mex is not created. It can still be build using the generated tiximatlab.c file.")
endif(MATLAB_FOUND)
	
endif() # python found
