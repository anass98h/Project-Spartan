# - Find curlpp
# Find the native CURLpp headers and libraries.
#
#  CURLPP_INCLUDE_DIRS - where to find curlpp/cURLpp.hpp, etc.
#  CURLPP_LIBRARIES    - List of libraries when using curlpp.
#  CURLPP_FOUND        - True if curlpp found.

# Look for the header file.
FIND_PATH(CURLPP_INCLUDE_DIR NAMES curlpp/cURLpp.hpp)
MARK_AS_ADVANCED(CURLPP_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(CURLPP_LIBRARY NAMES curlpp)
MARK_AS_ADVANCED(CURLPP_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set CURL_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CURLPP DEFAULT_MSG CURLPP_LIBRARY CURLPP_INCLUDE_DIR)

IF(CURLPP_FOUND)
    SET(CURLPP_LIBRARIES ${CURLPP_LIBRARY})
    SET(CURLPP_INCLUDE_DIRS ${CURLPP_INCLUDE_DIR})
ELSE(CURLPP_FOUND)
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/libcurlpp/utilspp/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/utilspp")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/libcurlpp/curlpp/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/curlpp")
ENDIF (CURLPP_FOUND)
