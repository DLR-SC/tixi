# TIXI C++ interface
# @author: Jan Kleinert

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tixicpp.h
    RENAME ${TIXI_LIB_NAME}cpp.h
    DESTINATION include/tixicpp.h
    COMPONENT headers
)
