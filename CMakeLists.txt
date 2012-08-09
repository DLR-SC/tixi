cmake_minimum_required (VERSION 2.6)

# Set a default build type if none was specified
if(NOT DEFINED CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

project (TIXI)

set(TIXI_VERSION "2.0")

set(ADD_INCLUDE_PATH "" CACHE PATH "Additional include path for package search")
set(ADD_LIB_PATH "" CACHE PATH     "Additional library path for package search")

set(CMAKE_INCLUDE_PATH "${CMAKE_INCLUDE_PATH}" ${ADD_INCLUDE_PATH})
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH}" ${ADD_LIB_PATH})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/contrib)

find_package(CURL REQUIRED)
find_package(XSLT REQUIRED)
find_package(XML REQUIRED)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#create tixi library
add_subdirectory(src)

#create bindings to different languages (fortran, java ...)
add_subdirectory(bindings)

#create tests
add_subdirectory(tests)

# create the doc
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

	add_custom_target(html
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

		add_custom_target(pdf
			COMMENT "Generating PDF documentation with latex" VERBATIM 
			DEPENDS ${PROJECT_BINARY_DIR}/doc/latex/refman.pdf
		)

		add_custom_target(doc
			DEPENDS html pdf
			COMMENT "Generating API documentation with Doxygen" VERBATIM 
		)
	else()
		add_custom_target(doc
			DEPENDS html
			COMMENT "Generating API documentation with Doxygen" VERBATIM 
		)
	endif()
endif(DOXYGEN_FOUND)

