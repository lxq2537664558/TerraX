#project(ComLib C CXX)
#SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Lib )
 
file(GLOB SRC_FILES src/*.cpp)

set(Com_Src 
main.cpp
${SRC_FILES}
)

add_library(com
    ${Com_Inc}
    ${Com_Src}
)
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Lib") #ok
#set_property(TARGET ComLib PROPERTY
#    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/Lib"
#    )
#message("project source dir is: ${PROJECT_SOURCE_DIR}")

install (
    TARGETS com 
    DESTINATION ${PROJECT_SOURCE_DIR}/Lib
    )