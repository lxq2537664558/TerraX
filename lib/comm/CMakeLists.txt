
aux_source_directory(./net NET_DIR)
aux_source_directory(./proto PROTO_DIR)
aux_source_directory(./config CONFIG_DIR)
set(COMM_DIR 
main.cpp
${NET_DIR}
${PROTO_DIR}
${CONFIG_DIR}
)

include_directories(${LIB_DIR})
include_directories(${THIRDPARTY_DIR})

add_library(comm
    ${COMM_DIR}
)
#set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") #ok

