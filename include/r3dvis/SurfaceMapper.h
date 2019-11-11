/************************************************************************
 * Copyright (C) 2019 Richard Palmer
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

#ifndef r3dvis_SURFACE_MAPPER_H
#define r3dvis_SURFACE_MAPPER_H

#include "r3dvis_Export.h"
#include <r3d/Mesh.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>
#include <functional>

namespace r3dvis {

// Get metric for mapping either polygon or vertex ID from Mesh for metric component
// k (always 0 for scalars but up to 1 less than dimensionality for vector metrics).
using MetricFn = std::function<float(int id, size_t k)>;

class r3dvis_EXPORT SurfaceMapper
{
public:
    // Set mapPolys to true to map polygons (default), false to map vertices.
    // Set dims to 1 for mapping scalars (default), higher values for mapping vectors.
    using CPtr = std::shared_ptr<const SurfaceMapper>;
    static CPtr create( const MetricFn&, bool mapPolys=true, size_t dims=1);

    SurfaceMapper( const MetricFn&, bool mapPolys, size_t dims);

    inline bool mapsPolys() const { return _mapsPolys;}
    inline size_t ndimensions() const { return _ndims;}

    // Make and return the array of metrics.
    // If isTextureMapped == true then the actor is assumed to have 3*nfaces vertices.
    // Note that the name of the returned array must be set before adding to an actor's
    // cell or point data.
    vtkSmartPointer<vtkFloatArray> makeArray( const r3d::Mesh&, bool isTextureMapped) const;

    // Get min/max for component c from last call to mapActor.
    float getMin( int c=0) const { return _min[c];}
    float getMax( int c=0) const { return _max[c];}

private:
    MetricFn _metricfn;
    const bool _mapsPolys;
    const size_t _ndims;
    mutable std::vector<float> _min;
    mutable std::vector<float> _max;
    float val( int, size_t) const;

    SurfaceMapper( const SurfaceMapper&) = delete;
    void operator=( const SurfaceMapper&) = delete;
};  // end class

}   // end namespace

#endif
