
aux_source_directory(./src SRC_FILES)
aux_source_directory(./proto PROTO_CXX_FILES)
set(NetLib_Src 
main.cpp
${SRC_FILES}
${PROTO_CXX_FILES}
)

include_directories(${Base_Inc})
include_directories(${Net_Inc})
include_directories(${zlib_Inc})
include_directories(${libevent_Inc})
include_directories(${protobuf_Inc})

add_library(net
    ${NetLib_Src}
)
#set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Lib") #ok

