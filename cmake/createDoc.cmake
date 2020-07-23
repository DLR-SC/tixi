find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/Doxyfile @ONLY)
	configure_file(${PROJECT_SOURCE_DIR}/doc/footer.html ${PROJECT_BINARY_DIR}/doc/footer.html @ONLY)
	add_custom_command(
		DEPENDS ${PROJECT_SOURCE_DIR}/src/tixi.h
		DEPENDS ${PROJECT_SOURCE_DIR}/Changelog.md
		OUTPUT ${PROJECT_BINARY_DIR}/doc/html/index.html
		OUTPUT ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
		COMMAND ${DOXYGEN_EXECUTABLE}
		ARGS ${PROJECT_BINARY_DIR}/doc/Doxyfile
	)

	add_custom_target(html
		DEPENDS ${PROJECT_BINARY_DIR}/doc/html/index.html
	)
	
	install(DIRECTORY ${PROJECT_BINARY_DIR}/doc/html
                        DESTINATION	share/${TIXI_LIB_NAME}/doc
			COMPONENT docu
			OPTIONAL)

	find_program(LATEX pdflatex)

	if(LATEX)
	    # run latex twice to fix links
		add_custom_command(
                        OUTPUT ${PROJECT_BINARY_DIR}/doc/tixi.pdf
			DEPENDS ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc/latex/		
			COMMAND ${LATEX}
			ARGS ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
			COMMAND ${LATEX}
			ARGS ${PROJECT_BINARY_DIR}/doc/latex/refman.tex
			COMMAND ${CMAKE_COMMAND}
                        ARGS -E copy ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf ${PROJECT_BINARY_DIR}/doc/tixi.pdf
		)

		add_custom_target(pdf
			COMMENT "Generating PDF documentation with latex" VERBATIM 
                        DEPENDS ${PROJECT_BINARY_DIR}/doc/tixi.pdf
		)

		add_custom_target(doc
			DEPENDS html pdf
			COMMENT "Generating API documentation with Doxygen" VERBATIM 
		)
		
                install(FILES ${PROJECT_BINARY_DIR}/doc/tixi.pdf
                                DESTINATION	share/${TIXI_LIB_NAME}/doc
				COMPONENT docu
				OPTIONAL)
	else()
		add_custom_target(doc
			DEPENDS html
			COMMENT "Generating API documentation with Doxygen" VERBATIM 
		)
	endif()
endif(DOXYGEN_FOUND)
