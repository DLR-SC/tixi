# units tests makefile

FILE (GLOB_RECURSE test_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (test_LIBS gtest TIXI)
SET (test_BIN ${PROJECT_NAME}-unittests)

IF (NOT CMAKE_CROSSCOMPILING)
    LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
    ADD_EXECUTABLE(${test_BIN} ${test_SRCS} )
    TARGET_LINK_LIBRARIES(${test_BIN} ${test_LIBS})
    ADD_CUSTOM_TARGET(check ALL "${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
    ADD_CUSTOM_TARGET(test "${test_BIN}" DEPENDS ${test_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${test_SRCS})
ENDIF (NOT CMAKE_CROSSCOMPILING)

#execute_process(COMMAND ${CMAKE_PROGRAM} -E copy_if_different ${PROJECT_SOURCE_DIR}/tests/TestData tests/Testdata)
file(COPY ${PROJECT_SOURCE_DIR}/tests/TestData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
