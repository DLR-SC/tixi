find_package(PythonInterp)

if(PYTHONINTERP_FOUND AND NOT PYTHON_VERSION_STRING VERSION_LESS "2.5" )
	
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/make-wrapper.py.in ${CMAKE_CURRENT_BINARY_DIR}/make-wrapper.py)
	
	#run python to create the wrapper
		add_custom_command(
			OUTPUT ${LIBRARY_OUTPUT_PATH}/tixiwrapper.py
			DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/make-wrapper.py
			WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
			COMMAND ${PYTHON_EXECUTABLE}
			ARGS ${CMAKE_CURRENT_BINARY_DIR}/make-wrapper.py
		)

		add_custom_target(tixiPython ALL
			COMMENT "Create python interface for tixi" VERBATIM 
			DEPENDS ${LIBRARY_OUTPUT_PATH}/tixiwrapper.py
		)
		
		install(FILES ${LIBRARY_OUTPUT_PATH}/tixiwrapper.py 
				DESTINATION share/tixi/python
				COMPONENT interfaces)
else()
    message(WARNING "Python could not be found or the python installation is too old ( < 2.5). Python bindings will not be build!")
endif()