# TIXI C++ interface
# @author: Jan Kleinert

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tixicpp.h
    RENAME ${TIXI_LIB_NAME}cpp.h
    DESTINATION share/${TIXI_LIB_NAME}/cpp
    COMPONENT headers
)
