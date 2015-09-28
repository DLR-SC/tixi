# only build, if fortran compiler was found before
if(CMAKE_Fortran_COMPILER)
  enable_language(Fortran)

  # copy xml files
  file(GLOB_RECURSE inputdata ${PROJECT_SOURCE_DIR}/examples/Demo/*.xml)
  file(COPY ${inputdata} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

  add_executable(testarrayf77 testarrayf77.f)
  target_link_libraries(testarrayf77 TIXIf TIXI)

  add_executable(testvectorf77 testvectorf77.f)
  target_link_libraries(testvectorf77 TIXIf TIXI)

  add_executable(tixi_fortran_examples tixi_fortran_examples.f)
  target_link_libraries(tixi_fortran_examples TIXIf TIXI)


endif(CMAKE_Fortran_COMPILER)
