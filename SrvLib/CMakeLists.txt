
aux_source_directory(./src SRC_FILES)
set(SrvLib_Src 
main.cpp
${SRC_FILES}
)

include_directories(${Base_Inc})
include_directories(${Net_Inc})
include_directories(${SrvLib_Inc})
include_directories(${zlib_Inc})
include_directories(${libevent_Inc})
include_directories(${protobuf_Inc})

add_library(srv
    ${SrvLib_Src}
)
#set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Lib") #ok

