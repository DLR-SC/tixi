
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
        OUTPUT ${LIBRARY_OUTPUT_PATH}/${TIXI_LIB_NAME}.f90
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/make_tixi_fortran03.py
                ${PROJECT_SOURCE_DIR}/src/tixi.h
                ${PROJECT_SOURCE_DIR}/bindings/bindings_generator/cheader_parser.py
                ${PROJECT_SOURCE_DIR}/bindings/bindings_generator/fortran03_generator.py
        WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
        COMMAND ${PYTHON_EXECUTABLE}
        ARGS ${CMAKE_CURRENT_SOURCE_DIR}/make_tixi_fortran03.py
    )

    add_custom_target(tixiFortran03 ALL
        COMMENT "Create Fortran 2003 interface for tixi" VERBATIM
        DEPENDS ${LIBRARY_OUTPUT_PATH}/${TIXI_LIB_NAME}.f90
    )

    if(TIXI_BUILD_TESTS AND NOT CMAKE_Fortran_COMPILER STREQUAL CMAKE_Fortran_COMPILER-NOTFOUND AND CMAKE_Fortran_COMPILER)
      enable_language(Fortran)
      
      add_executable(wrappertestsF03 wrappertests.F90 ${LIBRARY_OUTPUT_PATH}/${TIXI_LIB_NAME}.f90)
      target_compile_definitions(wrappertestsF03 PRIVATE TIXI_VERSION="${TIXI_VERSION}")
      target_link_libraries(wrappertestsF03 ${TIXI_LIB_NAME})
      set_target_properties(wrappertestsF03 PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests
      )
    endif()

    install(FILES ${LIBRARY_OUTPUT_PATH}/${TIXI_LIB_NAME}.f90
            DESTINATION share/${TIXI_LIB_NAME}/fortran03
            COMPONENT interfaces
    )
else()
    message(WARNING "Python could not be found or the python installation is too old ( < 2.5). Fortran03 bindings will not be build!")
endif()
