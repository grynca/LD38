cmake_minimum_required(VERSION 3.2)
project(LD38)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLM_FORCE_RADIANS -std=c++14 -ffast-math" )

IF (CMAKE_BUILD_TYPE STREQUAL "Release")
    #disable profiling
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNPROFILE -Wall -Werror -Wno-strict-aliasing" )
ENDIF()

IF (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    IF (WIN32)
        # for profiling
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -gdwarf-2" )
    ENDIF()
ENDIF()

option(USE_SDL2 "Use SDL2" ON)
IF (USE_SDL2)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_SDL2=1" )
    IF (WIN32)
        # TODO:
    ELSE()
        set(LIBS "-lGLEW -lGL -lGLU -lSDL2")
    ENDIF()
ENDIF()

IF (WIN32)
    # TODO
ELSE()
    include_directories(/home/grynca/DEV/gamedev/base/include)
    include_directories(/home/grynca/DEV/gamedev/sysent/include/)
    include_directories(/home/grynca/DEV/gamedev/maths/include/)
    include_directories(/home/grynca/DEV/gamedev/SAP/include/)
    include_directories(/home/grynca/DEV/gamedev/assets/include/)
    include_directories(/home/grynca/DEV/gamedev/ggnet/include/)
    include_directories(/home/grynca/DEV/gamedev/geng/include/)
    include_directories(/usr/include/freetype2)

    set(LIBS "${LIBS} -lfreetype -lSDL2_image")
ENDIF()
include_directories(include/)

set(SOURCES
        src/main.cpp
        src/incl.h
        src/MyGame.h
        src/MyGame.inl
        src/entities/World.h
        src/entities/World.inl
        src/entities/Player.h
        src/entities/Player.inl
        src/entities/WorldSystem.h src/entities/CWorldBodyData.h src/entities/Projectile.h)

#add_library(geng ${GENG_SOURCES})
add_executable(LD38 ${SOURCES})
target_link_libraries(LD38 ${LIBS})