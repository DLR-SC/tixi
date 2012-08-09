# units tests makefile

find_package(CUNIT)

if(CUNIT_FOUND)

set(TEST_FUNCTS_SRC 
	addAttribute.c        
	arrays.c
	externaldataOpenMode.c
	getElement.c
	other.c
	uid.c
	xpaths.c
	addComplexElements.c
	closeDocument.c
	internal.c
	removeAttributes.c
	validation.c
	xslt.c
	addElement.c
	createDocument.c
	getAttribute.c
	openDocument.c
	removeElement.c
	vectors.c
)

#	fortranInterface.c

set(TEST_SUITE_SRC CUMain.c addTest.c)

set(CMAKE_C_FLAGS_RELEASE "-O3")

foreach(FILE ${TEST_FUNCTS_SRC})
	set(TEST_SUITE_SRC ${TEST_SUITE_SRC} TestFunctions/${FILE})
endforeach()

include_directories(${PROJECT_SOURCE_DIR}/src ${CUNIT_INCLUDE_DIRS})

add_executable(runtests ${TEST_SUITE_SRC})
target_link_libraries(runtests ${CUNIT_LIBRARIES} TIXI)

add_custom_command(
	TARGET runtests
	POST_BUILD
	COMMAND ${CMAKE_COMMAND}
	ARGS -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/TestData/ ${CMAKE_CURRENT_BINARY_DIR}/TestData
)

endif(CUNIT_FOUND)

