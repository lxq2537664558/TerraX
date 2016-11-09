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
 -Wall
 -Wextra
 -Werror
 -Wno-error=conversion
 #-Wconversion #http://stackoverflow.com/questions/21050620/suppress-wconversion-for-specific-lines-of-code
 -Wno-unused-parameter
 -Wno-error=old-style-cast
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

set (ComLib_Inc ${PROJECT_SOURCE_DIR}/ComLib/include)
set (SrvLib_Inc ${PROJECT_SOURCE_DIR}/SrvLib/include ${PROJECT_SOURCE_DIR}/SrvLib)
set (libevent_Inc ${PROJECT_SOURCE_DIR}/ThirdParty/libevent-2.0.23-stable-rc/include)
set (protobuf_Inc ${PROJECT_SOURCE_DIR}/ThirdParty/protobuf/include)
set (zlib_Inc ${PROJECT_SOURCE_DIR}/ThirdParty/zlib-1.2.8)


#find_library(MyLib_Lib 
#    NAMES com srv
#    PATHS ${PROJECT_SOURCE_DIR}/Lib
#    )
#if(MyLib_Lib)
#    message("MyLib_Lib Found!")
#endif()

#find_library(ThirdParty_Lib
#    NAMES z event event_core event_extra protobuf protobuf-lite protoc
#    PATHS ${PROJECT_SOURCE_DIR}/ThirdParty/Lib
#    )
#if(ThirdParty_Lib)
#    message("ThirdParty_Lib Found!")
#endif()

add_subdirectory(ComLib)
add_subdirectory(SrvLib)
add_subdirectory(TestClient)
add_subdirectory(TestServer)