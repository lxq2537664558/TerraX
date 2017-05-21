aux_source_directory(./world_server WORLD_SERVER_DIR)

include_directories(${LIB_DIR})
include_directories(${THIRDPARTY_DIR})

link_directories(
    ${PROJECT_SOURCE_DIR}/bin
)

set (LINK_LIB
    libsrv.a
    libcomm.a
    libbase.a
    
    libevent.a
    libevent_core.a
    libevent_extra.a
    libprotobuf.a
    libprotobuf-lite.a
    libprotoc.a
)

add_executable(world_server 
    main.cpp
    ${WORLD_SERVER_DIR}
    )
target_link_libraries(world_server 
    ${LINK_LIB}
    )

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/world_server")