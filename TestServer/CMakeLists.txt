
file(GLOB ALL_HEADER_FILES *.h)
file(GLOB ALL_SOURCE_FILES *.cpp)


include_directories(${ComLib_Inc})
include_directories(${SrvLib_Inc})
include_directories(${zlib_Inc})
include_directories(${libevent_Inc})
include_directories(${protobuf_Inc})

link_directories(
    ${PROJECT_SOURCE_DIR}/Lib
    ${PROJECT_SOURCE_DIR}/ThirdParty/Lib
)

set (ThirdParty_Lib
    libcom.a
    libsrv.a
)

set (MyLib_Lib
    libevent.a
    libevent_core.a
    libevent_extra.a
    libprotobuf.a
    libprotobuf-lite.a
    libprotoc.a
    libz.a
)
add_executable(TestServer 
    ${ALL_SOURCE_FILES}
    ${ALL_HEADER_FILES}
    )
target_link_libraries( TestServer 
    ${ThirdParty_Lib} 
    ${MyLib_Lib}
    )

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/${CMAKE_BUILD_TYPE}")