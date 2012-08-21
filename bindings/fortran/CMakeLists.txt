# TIXI Fortran interface
# @author: Martin Siggel

set(TIXI_F_SRC tixi77.c)
set(TIXI_F_HDR tixi77.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)

#find out the fortran compiler
include(CMakeDetermineFortranCompiler)

if(CMAKE_Fortran_COMPILER STREQUAL CMAKE_Fortran_COMPILER-NOTFOUND)
	message(STATUS "No Fortran Compiler found, please specify for which compiler the interface should be built")
else()
	if(${CMAKE_Fortran_COMPILER} MATCHES "gfortran")
		message(STATUS "Generating fortran interface for gfortran naming scheme.")
		add_definitions(-DGFORTRAN)
	elseif(${CMAKE_Fortran_COMPILER} MATCHES "g77")
		message(STATUS "Generating fortran interface for g77 naming scheme.")
		add_definitions(-DG77)
	elseif(${CMAKE_Fortran_COMPILER} MATCHES "f90_irix")
		message(STATUS "Generating fortran interface for f90/irix naming scheme.")
		add_definitions(-DF90_IRIX)
	elseif(${CMAKE_Fortran_COMPILER} MATCHES "f90")
		message(STATUS "Generating fortran interface for f90 naming scheme.")
		add_definitions(-DGFORTRAN)
	elseif(${CMAKE_Fortran_COMPILER} MATCHES "ifort")
		message(STATUS "Generating fortran interface for intel-fortran (ifort) naming scheme.")
		add_definitions(-DIFORT_WIN32)
	endif()

	add_library(TIXIf STATIC ${TIXI_F_SRC} ${TIXI_F_HDR})
	set_target_properties(TIXIf PROPERTIES VERSION ${TIXI_VERSION})
	install (TARGETS TIXIf 
			DESTINATION lib
			COMPONENT interfaces)
	install (FILES tixi77.h 
			DESTINATION include
			COMPONENT interfaces)
	
endif()

