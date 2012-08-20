# Matlab bindings for tixi
# @author Martin Siggel

find_package(MATLAB)

if(MATLAB_FOUND)

message(STATUS "Found MATLAB. Create matlab bindings.")

# c wrapper code
set(TMAT_SRC tiximatlab.c common.h)

# matlab files
set(TMAT_M 
	tixiAddCpacsHeader.m
	tixiAddFloatVector.m
	tixiAddHeader.m
	tixiAddPoint.m
	tixiAddTextElement.m
	tixiAddTextElementAtIndex.m
	tixiCloseDocument.m
	tixiCreateDocument.m
	tixiCreateElement.m
	tixiCreateElementAtIndex.m
	tixiDTDValidate.m
	tixiExportDocumentAsString.m
	tixiGetBooleanElement.m
	tixiGetDoubleElement.m
	tixiGetFloatVector.m
	tixiGetIntegerElement.m
	tixiGetNamedChildrenCount.m
	tixiGetTextElement.m
	tixiGetVectorSize.m
	tixiImportFromString.m
	tixiOpenDocument.m
	tixiOpenDocumentFromHTTP.m
	tixiOpenDocumentRecursive.m
	tixiRemoveElement.m
	tixiSaveAndRemoveDocument.m
	tixiSaveCompleteDocument.m
	tixiSaveDocument.m
	tixiSchemaValidateFromFile.m
	tixiSchemaValidateFromString.m
	tixiUpdateDoubleElement.m
	tixiUpdateTextElement.m
)

foreach(FILE ${TMAT_M})
	set(TMAT_FULL_M ${TMAT_FULL_M} ${CMAKE_CURRENT_SOURCE_DIR}/${FILE})
endforeach()

# add def for exports
if(MSVC)
  set(TMAT_SRC ${TMAT_SRC} exports.def)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src ${MATLAB_INCLUDE_DIR})

add_library(tixi_matlab SHARED ${TMAT_SRC})
target_link_libraries(tixi_matlab ${MATLAB_mat_LIBRARY} ${MATLAB_mx_LIBRARY} ${MATLAB_mex_LIBRARY} TIXI)
set_target_properties(tixi_matlab PROPERTIES SUFFIX ${MATLAB_MEX_SUFFIX} PREFIX "")

endif()