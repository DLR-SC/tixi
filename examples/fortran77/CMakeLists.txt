# only build, if fortran compiler was found before
if(NOT CMAKE_Fortran_COMPILER STREQUAL CMAKE_Fortran_COMPILER-NOTFOUND AND CMAKE_Fortran_COMPILER)
  enable_language(Fortran)

  # copy xml files
  file(GLOB_RECURSE inputdata ${PROJECT_SOURCE_DIR}/examples/Demo/*.xml)
  file(COPY ${inputdata} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

  add_executable(testarrayf77 testarrayf77.f)
  target_link_libraries(testarrayf77 ${TIXI_LIB_NAME}-f90 ${TIXI_LIB_NAME})

  add_executable(testvectorf77 testvectorf77.f)
  target_link_libraries(testvectorf77 ${TIXI_LIB_NAME}-f90 ${TIXI_LIB_NAME})

  add_executable(tixi_fortran_examples tixi_fortran_examples.f)
  target_link_libraries(tixi_fortran_examples ${TIXI_LIB_NAME}-f90 ${TIXI_LIB_NAME})


endif()
