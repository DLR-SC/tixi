# Matlab bindings for tixi
# @author Martin Siggel

find_package(MATLAB)
find_package(PythonInterp)

if(MATLAB_FOUND AND PYTHONINTERP_FOUND)

message(STATUS "Found MATLAB. Create matlab bindings.")

#run python to create the tiglmatlab.c
add_custom_command(
	OUTPUT tiximatlab.c
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/make_tixi_matlab.py ${PROJECT_SOURCE_DIR}/src/tixi.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${PYTHON_EXECUTABLE}
	ARGS ${CMAKE_CURRENT_SOURCE_DIR}/make_tixi_matlab.py
)

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

install(
	CODE "file( GLOB MFILES ${CMAKE_CURRENT_BINARY_DIR}/*.m )"
	CODE "file( INSTALL \${MFILES} DESTINATION \${CMAKE_INSTALL_PREFIX}/share/tixi/matlab )"
	COMPONENT interfaces
)

endif() # matlab found