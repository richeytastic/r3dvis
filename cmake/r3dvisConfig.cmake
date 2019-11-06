# =============================================================================
# The r3dvis CMake configuration file.
#
#           ** File generated automatically, DO NOT MODIFY! ***

# To use from an external project, in your project's CMakeLists.txt add:
#   FIND_PACKAGE( r3dvis REQUIRED)
#   INCLUDE_DIRECTORIES( r3dvis ${r3dvis_INCLUDE_DIRS})
#   LINK_DIRECTORIES( ${r3dvis_LIBRARY_DIR})
#   TARGET_LINK_LIBRARIES( MY_TARGET_NAME ${r3dvis_LIBRARIES})
#
# This module defines the following variables:
#   - r3dvis_FOUND         : True if r3dvis is found.
#   - r3dvis_ROOT_DIR      : The root directory where r3dvis is installed.
#   - r3dvis_INCLUDE_DIRS  : The r3dvis include directories.
#   - r3dvis_LIBRARY_DIR   : The r3dvis library directory.
#   - r3dvis_LIBRARIES     : The r3dvis imported libraries to link to.
#
# =============================================================================

get_filename_component( r3dvis_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component( r3dvis_ROOT_DIR  "${r3dvis_CMAKE_DIR}"           PATH)

set( r3dvis_INCLUDE_DIRS "${r3dvis_ROOT_DIR}/../include" CACHE PATH "The r3dvis include directories.")
set( r3dvis_LIBRARY_DIR  "${r3dvis_ROOT_DIR}"            CACHE PATH "The r3dvis library directory.")

include( "${CMAKE_CURRENT_LIST_DIR}/Macros.cmake")
get_library_suffix( _lsuff)
set( _hints r3dvis${_lsuff} libr3dvis${_lsuff})
find_library( r3dvis_LIBRARIES NAMES ${_hints} PATHS "${r3dvis_LIBRARY_DIR}/static" "${r3dvis_LIBRARY_DIR}")
set( r3dvis_LIBRARIES     ${r3dvis_LIBRARIES}         CACHE FILEPATH "The r3dvis imported libraries to link to.")

# handle QUIETLY and REQUIRED args and set r3dvis_FOUND to TRUE if all listed variables are TRUE
include( "${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
find_package_handle_standard_args( r3dvis r3dvis_FOUND r3dvis_LIBRARIES r3dvis_INCLUDE_DIRS)
