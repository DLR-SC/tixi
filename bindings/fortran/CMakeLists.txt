# TIXI Fortran interface
# @author: Martin Siggel

set(TIXI_F_SRC tixi77.c)
set(TIXI_F_HDR tixi77.h)

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
        if (WIN32)
            add_definitions(-DIFORT_WIN32)
        else()
            add_definitions(-DIFORT_UNIX)
        endif()
    endif()

    if (C_COMPILER_HAS_NO_DEPRECATED_DECL_OPTION)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
    endif()

    add_library(${TIXI_LIB_NAME}-f90 STATIC ${TIXI_F_SRC} ${TIXI_F_HDR})
    target_link_libraries(${TIXI_LIB_NAME}-f90 ${TIXI_LIB_NAME})
    set_target_properties(${TIXI_LIB_NAME}-f90 PROPERTIES VERSION ${TIXI_VERSION})

    install (TARGETS ${TIXI_LIB_NAME}-f90
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT interfaces)
    
    install (FILES tixi77.h 
        DESTINATION ${CMAKE_INSTALL_INCLUDE_DIR}
        COMPONENT interfaces)
    
    install(TARGETS ${TIXI_LIB_NAME}-f90 EXPORT ${TIXI_LIB_NAME}-f90-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(EXPORT ${TIXI_LIB_NAME}-f90-targets
        DESTINATION ${CONFIG_INSTALL_DIR}
    )

endif()

