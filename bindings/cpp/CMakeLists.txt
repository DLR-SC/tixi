# TIXI C++ interface
# @author: Jan Kleinert

# check for existence of std::unique_ptr and set flag in tixicpp.h correspondingly
include(CheckCXXSourceCompiles)
set(src_unique_ptr "#include <memory>\nint main(){\n std::unique_ptr<int> a(new int)\;\n return 0\;\n}\n")
CHECK_CXX_SOURCE_COMPILES(${src_unique_ptr} HAVE_STDUNIQUE_PTR)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tixicpp.h.in ${TIXI_LIB_NAME}cpp.h)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${TIXI_LIB_NAME}cpp.h
    RENAME ${TIXI_LIB_NAME}cpp.h
    DESTINATION share/${TIXI_LIB_NAME}/cpp
    COMPONENT headers
)
