#file(GLOB ALL_HEADER_FILES *.h)
#file(GLOB ALL_SOURCE_FILES *.cpp)
aux_source_directory(./net NET_DIR)

set(SRV_DIR
main.cpp 
${NET_DIR}
)

include_directories(${LIB_DIR})
include_directories(${THIRDPARTY_DIR})

add_library(srv
    ${SRV_DIR}
)
#set_target_properties(libhello PROPERTIES OUTPUT_NAME "hello")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") #ok

