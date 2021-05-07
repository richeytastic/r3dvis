/************************************************************************
 * Copyright (C) 2021 Richard Palmer
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
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vector>

namespace r3dvis {

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

    // Given array of 2D pixel coords and a specified actor, find the cell IDs
    // being addressed, returning the number appended to cellIds.
    int pickActorCells( const std::vector<cv::Point>& points2d, const vtkProp* actor,
                        std::vector<int>& cellIds) const;

    // Given a 2D point, find the actor being pointed to. Returns null if no actor found.
    const vtkActor* pickActor( const cv::Point&) const;
    const vtkActor* pickActor( const cv::Point2f&) const;

    // As above, but only selects the actor from the given vector of actors.
    const vtkActor* pickActor( const cv::Point& point, const std::vector<const vtkProp*>&) const;
    const vtkActor* pickActor( const cv::Point2f&, const std::vector<const vtkProp*>&) const;

    // Pick an actor's cell returning its ID. Returns -1 if no cell found.
    int pickCell( const cv::Point&) const;
    int pickCell( const cv::Point2f&) const;

    // Find the intersecting position on the given actor and 2D point.
    // Returns true iff a valid intersection of the actor is found.
    // On (true) return, the provided Vec3f is set to the intersection point.
    bool pickPosition( const vtkProp*, const cv::Point&, r3d::Vec3f&) const;
    bool pickPosition( const vtkProp*, const cv::Point2f&, r3d::Vec3f&) const;

    // Given a 2D point, find the intersecting world position. Note that a position
    // in space is always returned even if the point does not intersect with a prop/actor!
    r3d::Vec3f pickPosition( const cv::Point&) const;
    r3d::Vec3f pickPosition( const cv::Point2f&) const;

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
