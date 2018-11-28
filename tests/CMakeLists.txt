# units tests makefile

FILE (GLOB_RECURSE test_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (test_BIN ${PROJECT_NAME}-unittests)

add_executable(${test_BIN} ${test_SRCS} )

if(WIN32)
    target_compile_definitions(${test_BIN} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif(WIN32)

target_link_libraries(${test_BIN} PRIVATE gtest ${TIXI_LIB_NAME} tixicpp tixi_internal_api)


if (VLD_FOUND)
    target_link_libraries(${test_BIN} PRIVATE vld)
endif(VLD_FOUND)

add_test(
    NAME unittests
    COMMAND ${test_BIN} --gtest_output=xml
)

#execute_process(COMMAND ${CMAKE_PROGRAM} -E copy_if_different ${PROJECT_SOURCE_DIR}/tests/TestData tests/Testdata)
file(COPY ${PROJECT_SOURCE_DIR}/tests/TestData DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/runtests.sh.in"
    "${CMAKE_CURRENT_BINARY_DIR}/runtests.sh"
)
