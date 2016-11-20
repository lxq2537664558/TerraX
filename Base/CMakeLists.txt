#project(ComLib C CXX)
#SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Lib )
 
file(GLOB SRC_FILES src/*.cpp)

set(Base_Src 
main.cpp
${SRC_FILES}
)

add_library(base
    ${Base_Inc}
    ${Base_Src}
)
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Lib") #ok
#set_property(TARGET ComLib PROPERTY
#    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/Lib"
#    )
#message("project source dir is: ${PROJECT_SOURCE_DIR}")

install (
    TARGETS base 
    DESTINATION ${PROJECT_SOURCE_DIR}/Lib
    )