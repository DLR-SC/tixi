find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile @ONLY)
	configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/footer.html ${PROJECT_BINARY_DIR}/doc/doxygen/footer.html @ONLY)
	add_custom_command(
		OUTPUT ${PROJECT_BINARY_DIR}/doc/doxygen/html/index.html
		OUTPUT ${PROJECT_BINARY_DIR}/doc/doxygen/latex/refman.tex
		COMMAND ${DOXYGEN_EXECUTABLE}
		ARGS ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile
	)

	add_custom_target(api-html
		DEPENDS ${PROJECT_BINARY_DIR}/doc/doxygen/html/index.html
	)
	
	install(DIRECTORY ${PROJECT_BINARY_DIR}/doc/doxygen/html
			DESTINATION	share/doc/doxygen/tixi
			COMPONENT docu
			OPTIONAL)

	find_program(LATEX pdflatex)

	if(LATEX)
	    # run latex twice to fix links
		add_custom_command(
			OUTPUT ${PROJECT_BINARY_DIR}/doc/doxygen/tixi.pdf
			DEPENDS ${PROJECT_BINARY_DIR}/doc/doxygen/latex/refman.tex
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc/doxygen/latex/		
			COMMAND ${LATEX}
			ARGS ${PROJECT_BINARY_DIR}/doc/doxygen/latex/refman.tex
			COMMAND ${LATEX}
			ARGS ${PROJECT_BINARY_DIR}/doc/doxygen/latex/refman.tex
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy ${PROJECT_BINARY_DIR}/doc/doxygen/latex/refman.pdf ${PROJECT_BINARY_DIR}/doc/doxygen/tixi.pdf
		)

		add_custom_target(api-pdf
			COMMENT "Generating PDF documentation with latex" VERBATIM 
			DEPENDS ${PROJECT_BINARY_DIR}/doc/doxygen/tixi.pdf
		)

		add_custom_target(api-doc
			DEPENDS html pdf
			COMMENT "Generating API documentation with Doxygen" VERBATIM 
		)
		
		install(FILES ${PROJECT_BINARY_DIR}/doc/doxygen/tixi.pdf 
				DESTINATION	share/doc/doxygen/tixi
				COMPONENT docu
				OPTIONAL)
	else()
		add_custom_target(api-doc
			DEPENDS html
			COMMENT "Generating API documentation with Doxygen" VERBATIM 
		)
	endif()
endif(DOXYGEN_FOUND)