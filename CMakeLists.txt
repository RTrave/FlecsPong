cmake_minimum_required(VERSION 2.6)

#Configuration du projet

project(FlecsPong)

if (CMAKE_BUILD_TYPE STREQUAL "")
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release." FORCE)
endif ()

#set(CMAKE_DEBUG_POSTFIX "_d")
#set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/${CMAKE_BUILD_TYPE})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17") #c++11

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2GFX REQUIRED SDL2_gfx>=1.0.2)
#PKG_SEARCH_MODULE(SDL2NET REQUIRED SDL2_net>=2.0.0)

include(FetchContent)

FetchContent_Declare(
        flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
        GIT_TAG v3.2.11
)
FetchContent_MakeAvailable(flecs)

include_directories(
	./
	./sojotz/
	${SDL2_INCLUDE_DIRS}
	./build/_deps/flecs-src/include/
	/usr/include/
	)

file(
        GLOB_RECURSE
        src_files
        src/*
	)

add_executable(
        flecspong
        ${src_files}
	)

target_link_libraries(
        flecspong
        ${SDL2_LIBRARIES}
        ${SDL2GFX_LIBRARIES}
        flecs::flecs_static 
#        ${SDL2NET_LIBRARIES}
	)


#add the install targets
install(
	TARGETS flecspong
	DESTINATION bin
	)

# uninstall
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/uninstall.cmake")
