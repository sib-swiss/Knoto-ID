cmake_minimum_required(VERSION 2.8)
project(KNOTOID)

SET(KNOTOID_VERSION_MAJOR 1)
SET(KNOTOID_VERSION_MINOR 1)
SET(KNOTOID_VERSION_PATCH 1)

configure_file (  "src/Version.h.in"  "${PROJECT_BINARY_DIR}/Version.h"  )

#set STATIC_LINKING=TRUE to link to static libraries
IF(STATIC_LINKING)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
ENDIF(STATIC_LINKING)

FIND_PACKAGE(Boost REQUIRED  COMPONENTS regex)

add_subdirectory (src) 


#######scripts########
add_subdirectory (scripts)

#######doc############
add_subdirectory (doc/) 
add_subdirectory (examples) 


INSTALL( FILES doc/README.txt DESTINATION ./ COMPONENT documentation)
INSTALL( FILES LICENSES DESTINATION ./ COMPONENT documentation)
INSTALL( FILES RELEASE_NOTES.txt DESTINATION ./ COMPONENT documentation)

########################################
# Tests
########################################

enable_testing()

add_subdirectory (tests) 


########################################
# Packages
########################################
SET(CPACK_PACKAGE_NAME "Knoto-ID")
SET(CPACK_PACKAGE_VERSION ${KNOTOID_VERSION_MAJOR}.${KNOTOID_VERSION_MINOR}.${KNOTOID_VERSION_PATCH})
SET(CPACK_GENERATOR "TGZ")


set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

set(CPACK_COMPONENTS_GROUPING "ALL_COMPONENTS_IN_ONE") 
set(CPACK_COMPONENTS_ALL applications documentation) #other groups defined in "install()": applications, readme and devel 
set(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY ON)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)


INCLUDE(CPack)

