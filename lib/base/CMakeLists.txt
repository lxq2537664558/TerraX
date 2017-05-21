file(GLOB ALL_HEADER_FILES *.h)
file(GLOB ALL_SOURCE_FILES *.cpp)


add_library(base
    ${ALL_SOURCE_FILES}
)
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin") #ok
#set_property(TARGET ComLib PROPERTY
#    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/Lib"
#    )
#message("project source dir is: ${PROJECT_SOURCE_DIR}")

install (
    TARGETS base 
    DESTINATION ${PROJECT_SOURCE_DIR}/bin
    )