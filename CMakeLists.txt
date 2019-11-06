cmake_minimum_required(VERSION 3.12.2 FATAL_ERROR)

project( r3dvis)

set( WITH_R3D TRUE)
set( WITH_VTK TRUE)
include( "cmake/FindLibs.cmake")

set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set( INCLUDE_F "${INCLUDE_DIR}/${PROJECT_NAME}")
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src")
include_directories( ${INCLUDE_DIR})
include_directories( ${INCLUDE_F})

set( INCLUDE_FILES
    "${INCLUDE_F}.h"
    "${INCLUDE_F}/Axes.h"
    "${INCLUDE_F}/ImageGrabber.h"
    "${INCLUDE_F}/InteractorC1.h"
    "${INCLUDE_F}/KeyPresser.h"
    "${INCLUDE_F}/LookupTable.h"
    "${INCLUDE_F}/OffscreenMeshViewer.h"
    "${INCLUDE_F}/PointPlacer.h"
    "${INCLUDE_F}/RendererPicker.h"
    "${INCLUDE_F}/ScalarLegend.h"
    "${INCLUDE_F}/SnapshotKeyPresser.h"
    "${INCLUDE_F}/SurfaceMapper.h"
    "${INCLUDE_F}/Viewer.h"
    "${INCLUDE_F}/ViewerProjector.h"
    "${INCLUDE_F}/VtkActorCreator.h"
    "${INCLUDE_F}/VtkScalingActor.h"
    "${INCLUDE_F}/VtkTools.h"
    "${INCLUDE_F}/VTKTypes.h"
    )

set( SRC_FILES
    ${SRC_DIR}/Axes
    ${SRC_DIR}/ImageGrabber
    ${SRC_DIR}/InteractorC1
    ${SRC_DIR}/KeyPresser
    ${SRC_DIR}/LookupTable
    ${SRC_DIR}/OffscreenMeshViewer
    ${SRC_DIR}/PointPlacer
    ${SRC_DIR}/RendererPicker
    ${SRC_DIR}/ScalarLegend
    ${SRC_DIR}/SnapshotKeyPresser
    ${SRC_DIR}/SurfaceMapper
    ${SRC_DIR}/Viewer
    ${SRC_DIR}/ViewerProjector
    ${SRC_DIR}/VtkActorCreator
    ${SRC_DIR}/VtkScalingActor
    ${SRC_DIR}/VtkTools
    )

add_library( ${PROJECT_NAME} ${SRC_FILES} ${INCLUDE_FILES})
include( "cmake/LinkLibs.cmake")
