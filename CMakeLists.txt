cmake_minimum_required(VERSION 2.8)
project(Project-Spartan)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.2)
        message(FATAL_ERROR "Your GCC seems outdated. Update your compiler, run 'git clean -fdx' and try again.")
    endif()
else()
    message(FATAL_ERROR "Install GCC version 5.2 or higher to compile Project Spartan.")
endif()

FUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST _InFileList _excludeDirName _verbose)
    foreach (ITR ${_InFileList})
        if ("${_verbose}")
            message(STATUS "ITR=${ITR}")
        endif ("${_verbose}")

        if ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)") # Check if the item matches the directory name in _excludeDirName
            message(STATUS "Remove Item from List: ${ITR}")
            list (REMOVE_ITEM _InFileList ${ITR}) # Remove the item from the list
        endif ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")

    endforeach(ITR)
    set(SOURCE_FILES ${_InFileList} PARENT_SCOPE) # Return the SOURCE_FILES variable to the calling parent
ENDFUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3 -g -std=c++14 -Wall -Wno-maybe-uninitialized -Wno-unused-result")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++14 -Wall -Wno-maybe-uninitialized -Wno-unused-result")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -DNDEBUG -std=c++14 -Wall -Wno-maybe-uninitialized -Wno-unused-result")
#set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -std=c++14 -Wall -Wno-maybe-uninitialized -Wno-unused-result")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE RELEASE)
endif(NOT CMAKE_BUILD_TYPE)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/ImGUI/examples/" FALSE)
EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/ImGUI/extra_fonts" FALSE)

####################################################
# DEPENDENCIES                                     #
####################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

FIND_PACKAGE(CURL REQUIRED)
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${CURL_INCLUDE_DIRS}")

# I'd be happy if somebody could teach me how to CMake module cuz every example only checked for availability of Curl++ and not installing it
#FIND_PACKAGE(curlpp REQUIRED)
#set(INCLUDE_DIRS ${INCLUDE_DIRS} "${curlpp_INCLUDE_DIRS}")

FIND_PACKAGE(Lua 5.3 REQUIRED)
set(INCLUDE_DIRS ${INCLUDE_DIRS} "${LUA_INCLUDE_DIR}")
add_definitions(-DLUA)

include_directories(${INCLUDE_DIRS})

####################################################

add_library(Spartan SHARED ${SOURCE_FILES} src/Utils/protectionutil.cpp src/Utils/protectionutil.h)
target_link_libraries(Spartan SDL2 GL dl xdo ${CURL_LIBRARIES} ${LUA_LIBRARY})
