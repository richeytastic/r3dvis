/************************************************************************
 * Copyright (C) 2017 Richard Palmer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#ifndef r3dvis_RendererPicker_H
#define r3dvis_RendererPicker_H

#include "r3dvis_Export.h"
#include <r3d/r3dTypes.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vector>

namespace r3dvis {

// An actor and a subset of its cells.
struct r3dvis_EXPORT ActorSubset
{
    vtkActor* actor;
    std::vector<int> cellIds;
};  // end struct


class r3dvis_EXPORT RendererPicker
{
public:
    enum PointOrigin
    {
        BOTTOM_LEFT,    // VTK render window - default
        TOP_LEFT        // OpenCV, Qt, and others
    };  // end enum

    // Use the PointOrigin parameter to let the picking algorithms know if the provided
    // points are given with a bottom left origin (VTK default) or a top left origin.
    // Calls ResetCameraClippingRange() on the provided vtkRenderer to ensure picking accuracy.
    RendererPicker( vtkRenderer*, PointOrigin po=BOTTOM_LEFT, double tolerance=0.0005);

    // Given array of 2D pixel coordinates, find the actors and their cell IDs
    // pointed to by these locations. Returns number of actors found at the given 2D
    // coordinates (the number of entries appended to // output parameter picked).
    int pickActorCells( const std::vector<cv::Point>& points2d,
                        std::vector<ActorSubset>& picked) const;

    // Given array of 2D pixel coords and a specified actor, find the cell IDs
    // being addressed, returning the number appended to cellIds.
    int pickActorCells( const std::vector<cv::Point>& points2d,
                        vtkActor* actor,
                        std::vector<int>& cellIds) const;

    // Given a 2D point, find the actor being pointed to. Returns null if no actor found.
    vtkActor* pickActor( const cv::Point&) const;
    vtkActor* pickActor( const cv::Point2f&) const;

    // As above, but only selects the actor from the given vector of actors.
    vtkActor* pickActor( const cv::Point& point, const std::vector<vtkActor*>&) const;
    vtkActor* pickActor( const cv::Point2f&, const std::vector<vtkActor*>&) const;

    // Pick an actor's cell returning its ID. Returns -1 if no cell found.
    int pickCell( const cv::Point&) const;
    int pickCell( const cv::Point2f&) const;

    // Given a 2D point, find the intersecting world position. Note that a position
    // in space is always returned even if the point does not intersect with a prop/actor!
    r3d::Vec3f pickWorldPosition( const cv::Point&) const;
    r3d::Vec3f pickWorldPosition( const cv::Point2f&) const;

    // Pick the normal vector to the surface at the given point.
    // If no 3D point intersects, return the zero vector.
    r3d::Vec3f pickNormal( const cv::Point&) const;
    r3d::Vec3f pickNormal( const cv::Point2f&) const;

    // Project v to the rendering image plane. Point is returned using
    // the coordinates origin set in the constructor.
    cv::Point projectToImagePlane( const r3d::Vec3f& v) const;

private:
    vtkRenderer* _ren;
    const PointOrigin _pointOrigin;
    const double _tolerance;
};  // end class

}   // end namespace

#endif
