# TIXI C++ interface
# @author: Jan Kleinert

add_library(tixicpp INTERFACE)
target_include_directories(tixicpp INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tixicpp.h
    DESTINATION include/${TIXI_LIB_NAME}
    COMPONENT headers
)
