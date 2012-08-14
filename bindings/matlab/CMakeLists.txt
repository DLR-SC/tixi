# Matlab bindings for tixi
# @author Martin Siggel

find_package(MATLAB)

if(MATLAB_FOUND)

message(STATUS "Found MATLAB. Create matlab bindings.")

set(TMAT_SRC tiximatlab.c common.h)

# add def for exports
if(WIN32)
  set(TMAT_SRC ${TMAT_SRC} exports.def)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src ${MATLAB_INCLUDE_DIR})

add_library(tixi_matlab SHARED ${TMAT_SRC})
target_link_libraries(tixi_matlab ${MATLAB_mat_LIBRARY} ${MATLAB_mx_LIBRARY} ${MATLAB_mex_LIBRARY} TIXI)
set_target_properties(tixi_matlab PROPERTIES SUFFIX ${MATLAB_MEX_SUFFIX})

endif()