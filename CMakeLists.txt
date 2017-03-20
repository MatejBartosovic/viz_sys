cmake_minimum_required(VERSION 3.6)
project(viz_sys)

set (home $ENV{HOME})
set (ThirdPartyDIR ${home}/3rdParty_REPO_gcc4.9.4)

unset(OpenCV_CONFIG_PATH CACHE)
if(MSVC)
    set(OpenCV_DIR "${ThirdPartyDIR}/opencv-3.1.0-without_cuda")
elseif(UNIX)
    if(APPLE)
        set(OpenCV_DIR "${ThirdPartyDIR}/opencv-3.1.0/share/OpenCV")
    else()
        set(OpenCV_DIR "${ThirdPartyDIR}/opencv-3.1.0-without_cuda/share/OpenCV")
    endif()
endif()
set(OpenCV_STATIC OFF)
FIND_PACKAGE(OpenCV REQUIRED)
if(MSVC12)
    set(OpenCV_DLL_DIR ${OpenCV_DIR}/x64/vc12/bin)
elseif(MSVC14)
    set(OpenCV_DLL_DIR ${OpenCV_DIR}/x64/vc14/bin)
elseif(UNIX)
    if(NOT APPLE)
        set(OpenCV_DLL_DIR ${OpenCV_CONFIG_PATH}/../../lib)
    endif()
endif()
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_CLIENTCONFIG "RELEASE")

include_directories(
        ${OpenCV_INCLUDE_DIRS}
)

message(STATUS "${OpenCV_INCLUDE_DIRS}")
set(CMAKE_CXX_STANDARD 11)

add_executable(uloha1 src/main.cpp)

target_link_libraries(uloha1
        ${OpenCV_LIBS}
        )

add_executable(uloha2   src/HoughDetection.cpp
                        src/HughLineDetection.cpp
                        src/HughLineDetection.h
                        )
target_compile_definitions(uloha2 PRIVATE DEBUG=true)

target_link_libraries(uloha2
        ${OpenCV_LIBS}
        )
