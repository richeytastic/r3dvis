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

/***
 * Query 3D position on surface of a poly data object from a 2D render window pixel position.
 *
 * Richard Palmer
 * March 2015
 ***/

#ifndef r3dvis_PointPlacer_H
#define r3dvis_PointPlacer_H

#include <memory>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include "r3dvis_Export.h"

namespace r3dvis {

enum r3dvis_EXPORT DisplayOrigin
{
    TOP_LEFT_DISPLAY_ORIGIN,    // OpenCV 2D
    BOTTOM_LEFT_DISPLAY_ORIGIN  // VTK 2D
};  // end enum


class r3dvis_EXPORT PointPlacer
{
public:
    using Ptr = std::shared_ptr<PointPlacer>;
    static Ptr create( const vtkRenderer*);
    static Ptr create( const vtkRenderer*, vtkPolygonalSurfacePointPlacer*);

    void set( const vtkProp*);    // Set prop as the only one
    void add( const vtkProp*);    // Add a prop
    void clear(); // Remove all props

    bool hasProps() const;

    // Calculate the surface position on the model from x,y display coords and set worldPos with the result.
    // Returns true if x,y is a valid display coordinate on the model.
    // Default display origin uses VTK 2D display coords (bottom left).
    // OpenCV coords uses TOP_LEFT_DISPLAY_ORIGIN.
    bool calcSurfacePosition( int x, int y, float* worldPos, DisplayOrigin displayOrigin=BOTTOM_LEFT_DISPLAY_ORIGIN) const;

    const vtkRenderer* getRenderer() const { return _renderer;}
    const vtkPolygonalSurfacePointPlacer* getVtkPointPlacer() const { return _pplacer;}

protected:
    vtkRenderer* _renderer;
    vtkPolygonalSurfacePointPlacer* _pplacer;
    bool _delplacer;

private:
    explicit PointPlacer( const vtkRenderer*);
    PointPlacer( const vtkRenderer*, vtkPolygonalSurfacePointPlacer*);
    ~PointPlacer();
    PointPlacer( const PointPlacer&) = delete;
    void operator=( const PointPlacer&) = delete;
};  // end class

}   // end namespace

#endif
