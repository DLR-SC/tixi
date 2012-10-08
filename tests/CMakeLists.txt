# units tests makefile

FILE (GLOB_RECURSE test_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (test_LIBS gtest tixi)
SET (test_BIN ${PROJECT_NAME}-unittests)

IF (NOT CMAKE_CROSSCOMPILING)
    LINK_DIRECTORIES(${MAINFOLDER}/lib)
    ADD_EXECUTABLE(${test_BIN} ${test_SRCS})
    TARGET_LINK_LIBRARIES(${test_BIN} ${test_LIBS})
    ADD_CUSTOM_TARGET(check ALL "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
    ADD_CUSTOM_TARGET(test "${MAINFOLDER}/bin/${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
ENDIF (NOT CMAKE_CROSSCOMPILING)

# find_package(CUNIT)
# 
# if(CUNIT_FOUND)
# 
# set(TEST_FUNCTS_SRC 
# 	addAttribute.c        
# 	arrays.c
# 	externaldataOpenMode.c
# 	getElement.c
# 	other.c
# 	uid.c
# 	xpaths.c
# 	addComplexElements.c
# 	closeDocument.c
# 	internal.c
# 	removeAttributes.c
# 	validation.c
# 	xslt.c
# 	addElement.c
# 	createDocument.c
# 	getAttribute.c
# 	openDocument.c
# 	removeElement.c
# 	vectors.c
# )
# 
# #	fortranInterface.c
# 
# set(TEST_SUITE_SRC CUMain.c addTest.c)
# 
# foreach(FILE ${TEST_FUNCTS_SRC})
# 	set(TEST_SUITE_SRC ${TEST_SUITE_SRC} TestFunctions/${FILE})
# endforeach()
# 
# include_directories(${PROJECT_SOURCE_DIR}/src ${CUNIT_INCLUDE_DIRS} ${XML_INCLUDE_DIRS})
# 
# add_executable(runtests ${TEST_SUITE_SRC})
# target_link_libraries(runtests ${CUNIT_LIBRARIES} TIXI)
# 
# add_custom_command(
# 	TARGET runtests
# 	POST_BUILD
# 	COMMAND ${CMAKE_COMMAND}
# 	ARGS -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/TestData/ ${CMAKE_CURRENT_BINARY_DIR}/TestData
# )
# 
# endif(CUNIT_FOUND)

