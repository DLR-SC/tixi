find_package(PythonInterp)

if(PYTHONINTERP_FOUND)
	
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/make-wrapper.py.in ${CMAKE_CURRENT_BINARY_DIR}/make-wrapper.py)
	
	#run python to create the wrapper
		add_custom_command(
			OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tixiwrapper.py
			DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/make-wrapper.py
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMAND ${PYTHON_EXECUTABLE}
			ARGS ${CMAKE_CURRENT_BINARY_DIR}/make-wrapper.py
		)

		add_custom_target(tixiPython
			COMMENT "Create python interface for tixi" VERBATIM 
			DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tixiwrapper.py
		)
endif()