find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/Doxyfile @ONLY)
	configure_file(${PROJECT_SOURCE_DIR}/doc/footer.html ${PROJECT_BINARY_DIR}/doc/footer.html @ONLY)
	add_custom_command(
		OUTPUT ${PROJECT_BINARY_DIR}/doc/html/index.html
		OUTPUT ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
		COMMAND ${DOXYGEN_EXECUTABLE}
		ARGS ${PROJECT_BINARY_DIR}/doc/Doxyfile
	)

	add_custom_target(html EXCLUDE_FROM_ALL
		DEPENDS ${PROJECT_BINARY_DIR}/doc/html/index.html
	)

	find_program(LATEX pdflatex)

	if(LATEX)
		add_custom_command(
			OUTPUT ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf
			DEPENDS ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc/latex/		
			COMMAND ${LATEX}
			ARGS ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
		)

		add_custom_target(pdf EXCLUDE_FROM_ALL
			COMMENT "Generating PDF documentation with latex" VERBATIM 
			DEPENDS ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf
		)

		add_custom_target(doc EXCLUDE_FROM_ALL
			DEPENDS html pdf
			COMMENT "Generating API documentation with Doxygen" VERBATIM 
		)
	else()
		add_custom_target(doc EXCLUDE_FROM_ALL
			DEPENDS html
			COMMENT "Generating API documentation with Doxygen" VERBATIM 
		)
	endif()
endif(DOXYGEN_FOUND)