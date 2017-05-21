cmake_minimum_required (VERSION 2.6)

project (TerraX C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 #-D_FILE_OFFSET_BITS=64
 # -Wall
 -Wextra
 -Werror
 -Wno-error=conversion
 #-Wconversion #http://stackoverflow.com/questions/21050620/suppress-wconversion-for-specific-lines-of-code
 -Wno-unused-parameter
 -Wno-error=old-style-cast
 -Wno-reorder
 #-Woverloaded-virtual #http://stackoverflow.com/questions/16505092/confused-about-virtual-overloaded-functions
 #-Wpointer-arith
 #-Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 # -std=c++0x
 -std=c++11
 -pthread
 -rdynamic #http://stackoverflow.com/questions/20491130/impact-disadvantages-of-rdynamic-gcc-option
           #http://stackoverflow.com/questions/12634114/will-adding-the-rdynamic-linker-option-to-gcc-g-impact-performance
 )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++" )
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")

set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty/include)

add_subdirectory(lib/base)
#add_subdirectory(lib/clt)
add_subdirectory(lib/comm)
add_subdirectory(lib/srv)
add_subdirectory(server/world_server)
add_subdirectory(server/gate_server)
#add_subdirectory(client)