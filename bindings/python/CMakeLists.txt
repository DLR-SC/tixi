
# work around wrong python detection script of cmake (happens with old python versions)
if(PYTHON_EXECUTABLE)
    execute_process(COMMAND "${PYTHON_EXECUTABLE}" --version
                    ERROR_VARIABLE _VERSION
                    RESULT_VARIABLE _PYTHON_VERSION_RESULT
                    OUTPUT_QUIET
                    ERROR_STRIP_TRAILING_WHITESPACE)
    if(_PYTHON_VERSION_RESULT)
        execute_process(COMMAND "${PYTHON_EXECUTABLE}" -V
                        ERROR_VARIABLE _VERSION
                        RESULT_VARIABLE _PYTHON_VERSION_RESULT
                        OUTPUT_QUIET
                        ERROR_STRIP_TRAILING_WHITESPACE)
    endif(_PYTHON_VERSION_RESULT)
    if(NOT _PYTHON_VERSION_RESULT AND _VERSION MATCHES "^Python [0-9]+\\.[0-9]+.*")
        string(REPLACE "Python " "" PYTHON_VERSION_STRING "${_VERSION}")
        string(REGEX REPLACE "^([0-9]+)\\.[0-9]+.*" "\\1" PYTHON_VERSION_MAJOR "${PYTHON_VERSION_STRING}")
        string(REGEX REPLACE "^[0-9]+\\.([0-9])+.*" "\\1" PYTHON_VERSION_MINOR "${PYTHON_VERSION_STRING}")
        if(PYTHON_VERSION_STRING MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+.*")
            string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" PYTHON_VERSION_PATCH "${PYTHON_VERSION_STRING}")
        endif()
    endif()
endif(PYTHON_EXECUTABLE)

if(PYTHONINTERP_FOUND AND NOT PYTHON_VERSION_STRING VERSION_LESS "2.5" )
    #run python to create the wrapper
        add_custom_command(
                        OUTPUT ${LIBRARY_OUTPUT_PATH}/${TIXI_LIB_NAME}wrapper.py
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/make_tixi_python.py ${PROJECT_SOURCE_DIR}/src/tixi.h
            WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
            COMMAND ${PYTHON_EXECUTABLE}
            ARGS ${CMAKE_CURRENT_SOURCE_DIR}/make_tixi_python.py
        )

        add_custom_target(tixiPython ALL
            COMMENT "Create python interface for tixi" VERBATIM 
                        DEPENDS ${LIBRARY_OUTPUT_PATH}/${TIXI_LIB_NAME}wrapper.py
        )
        
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/wrappertests.py DESTINATION ${PROJECT_BINARY_DIR}/tests)
        
                install(FILES ${LIBRARY_OUTPUT_PATH}/${TIXI_LIB_NAME}wrapper.py
                                DESTINATION share/${TIXI_LIB_NAME}/python
                COMPONENT interfaces)
else()
    message(WARNING "Python could not be found or the python installation is too old ( < 2.5). Python bindings will not be build!")
endif()
