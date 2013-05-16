# define macro for adding tests

FUNCTION(SETUP_TARGET_FOR_VALGRIND _targetname _testbinname _outputname)

    # Check prereqs
    FIND_PROGRAM( VALGRIND_PATH valgrind )
    MESSAGE(STATUS "${VALGRIND_PATH")
    
    # add test case
    if(USE_VALGRIND)
            # no valgrind support in MSVC 
            if(NOT MSVC)
                    # add valgrind as a custom target
                    add_custom_target( ${_targetname} 
                            COMMAND valgrind
                                    --leak-check=full
                                    --show-reachable=no
                                    --track-fds=yes
                                    --error-exitcode=1
                                    --track-origins=yes
                                    --log-file=${CMAKE_CURRENT_BINARY_DIR}/${_outputname}.log 
                                    --xml=yes --xml-file=${CMAKE_CURRENT_BINARY_DIR}/${_outputname}.xml 
                                    ${CMAKE_CURRENT_BINARY_DIR}/tests/${_testbinname}
                            WORKING_DIRECTORY
                                    ${CMAKE_CURRENT_BINARY_DIR}/tests
                    )
            endif(NOT MSVC)
    endif(USE_VALGRIND)


ENDFUNCTION() # SETUP_TARGET_FOR_COVERAGE
