# TIXI Library
# @author Martin Siggel
# 
# The demo serves for 2 purposes. First,k it demonstrated the usage of tixi.
# Second, it allows us to scan for 3rd party library dependencies and install
# them with tixi (cmake doesn't allow scanning dlls only).

cmake_minimum_required(VERSION 3.1)

project(Tixi-Demos)


if (NOT TARGET tixi3)
    find_package(tixi3 REQUIRED)
endif()

if (APPLE)
  SET(CMAKE_INSTALL_RPATH "@executable_path/../${CMAKE_INSTALL_LIBDIR}")
endif(APPLE)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "-Wall -fmessage-length=0")
endif()

add_executable(tixiDemo tixiDemo.c)
target_link_libraries (tixiDemo tixi3)

if (UNIX)
  # link math library
  target_link_libraries (tixiDemo m)
endif(UNIX)

add_executable(tixi_c_examples tixi_c_examples.c)
target_link_libraries (tixi_c_examples tixi3)

if(WIN32)
    target_compile_definitions(tixi_c_examples PRIVATE _CRT_SECURE_NO_WARNINGS)
endif(WIN32)

add_executable(createCpacsFile createcpacsfile.c)
target_link_libraries (createCpacsFile tixi3)

add_custom_command(
    TARGET tixiDemo
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/howtoin.xml ${CMAKE_CURRENT_BINARY_DIR}/howtoin.xml
)
#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------

SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/tixiDemo")
IF(APPLE)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/tixiDemo")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/tixiDemo.exe")
ENDIF(WIN32)

#--------------------------------------------------------------------------------

# directories to look for dependencies
SET(DIRS ${ADD_LIB_PATH})
SET(DIRS ${DIRS} ${LIBRARY_OUTPUT_PATH})

# Install the tixiDemo application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
INSTALL(TARGETS tixiDemo 
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

if (WIN32)

install(FILES $<TARGET_FILE:tixi3> DESTINATION bin COMPONENT Runtime)

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
INSTALL(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\" \"${DIRS}\")
    " COMPONENT Runtime)

endif()
