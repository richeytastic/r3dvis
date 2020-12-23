/************************************************************************
 * Copyright (C) 2020 Richard Palmer
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
    // Set dims to 1 for mapping scalars (default), higher values for vectors.
    SurfaceMapper( const MetricFn&, size_t dims);

    inline size_t dims() const { return _ndims;}

    // Make and return the array of metrics - returned array's name is 'name'.
    // (Array name must be set before adding to an actor's cell or point data).
    // Use function makeArrayNoTx to assume the same number of vertices as the mesh
    // for the actor (i.e. if the array mapping is for a non-textured surface actor).
    vtkSmartPointer<vtkFloatArray> makeArray( const r3d::Mesh&, const char *name) const;
    vtkSmartPointer<vtkFloatArray> makeArrayNoTx( const r3d::Mesh&, const char *name) const;

protected:
    const MetricFn _mfn;
    virtual void _makeArray( const r3d::Mesh&, vtkFloatArray*) const = 0;
    virtual void _makeArrayNoTx( const r3d::Mesh&, vtkFloatArray*) const = 0;

private:
    const size_t _ndims;
    vtkSmartPointer<vtkFloatArray> _initArray( const r3d::Mesh&, const char*) const;
    SurfaceMapper( const SurfaceMapper&) = delete;
    void operator=( const SurfaceMapper&) = delete;
};  // end class


class r3dvis_EXPORT VertexSurfaceMapper : public SurfaceMapper
{
public:
    VertexSurfaceMapper( const MetricFn&, size_t);
protected:
    void _makeArray( const r3d::Mesh&, vtkFloatArray*) const override;
    void _makeArrayNoTx( const r3d::Mesh&, vtkFloatArray*) const override;
};  // end class


class r3dvis_EXPORT FaceSurfaceMapper : public SurfaceMapper
{
public:
    FaceSurfaceMapper( const MetricFn&, size_t);
protected:
    void _makeArray( const r3d::Mesh&, vtkFloatArray*) const override;
    void _makeArrayNoTx( const r3d::Mesh&, vtkFloatArray*) const override;
};  // end class

}   // end namespace

#endif
