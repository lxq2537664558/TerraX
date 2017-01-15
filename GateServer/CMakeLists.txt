
file(GLOB ALL_HEADER_FILES *.h)
file(GLOB ALL_SOURCE_FILES *.cpp)

include_directories(${zlib_Inc})
include_directories(${libevent_Inc})
include_directories(${protobuf_Inc})
include_directories(${Base_Inc})
include_directories(${Net_Inc})
include_directories(${SrvLib_Inc})

link_directories(
    ${PROJECT_SOURCE_DIR}/Lib
    ${PROJECT_SOURCE_DIR}/ThirdParty/Lib
)

set (MyLib_Lib
    libbase.a
    libnet.a
    libsrv.a
)

set (ThirdParty_Lib
    libevent.a
    libevent_core.a
    libevent_extra.a
    libprotobuf.a
    libprotobuf-lite.a
    libprotoc.a
    libz.a
)
add_executable( GateServer 
    ${ALL_SOURCE_FILES}
    ${ALL_HEADER_FILES}
    )
target_link_libraries( GateServer 
    ${MyLib_Lib}
    ${ThirdParty_Lib} 
    )

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}")