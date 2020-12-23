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

#include <SurfaceMapper.h>
#include <VtkTools.h>
#include <climits>
#include <cstdlib>
#include <cassert>
using r3dvis::SurfaceMapper;
using r3dvis::FaceSurfaceMapper;
using r3dvis::VertexSurfaceMapper;
using r3dvis::MetricFn;
using r3d::Mesh;

SurfaceMapper::SurfaceMapper( const MetricFn& fn, size_t d) : _mfn(fn), _ndims( std::max<size_t>(d, 1)) {}
VertexSurfaceMapper::VertexSurfaceMapper( const MetricFn& fn, size_t d) : SurfaceMapper( fn, d) {}
FaceSurfaceMapper::FaceSurfaceMapper( const MetricFn& fn, size_t d) : SurfaceMapper( fn, d) {}


vtkSmartPointer<vtkFloatArray> SurfaceMapper::_initArray( const Mesh& mesh, const char *aname) const
{
    assert( mesh.hasSequentialIds());
    vtkSmartPointer<vtkFloatArray> vals = vtkSmartPointer<vtkFloatArray>::New();
    vals->SetNumberOfComponents( static_cast<int>(dims()));
    vals->SetName( aname);
    return vals;
}   // end _initArray


vtkSmartPointer<vtkFloatArray> SurfaceMapper::makeArray( const Mesh& mesh, const char *aname) const
{
    vtkSmartPointer<vtkFloatArray> vals = _initArray(mesh, aname);
    _makeArray( mesh, vals);
    return vals;
}   // end makeArray


vtkSmartPointer<vtkFloatArray> SurfaceMapper::makeArrayNoTx( const Mesh& mesh, const char *aname) const
{
    vtkSmartPointer<vtkFloatArray> vals = _initArray(mesh, aname);
    _makeArrayNoTx( mesh, vals);
    return vals;
}   // end makeArrayNoTx


void FaceSurfaceMapper::_makeArray( const r3d::Mesh &mesh, vtkFloatArray *cvals) const
{
    const size_t nd = dims();
    const int nf = int(mesh.numFaces());
    cvals->SetNumberOfTuples( nf);
    for ( size_t k = 0; k < nd; ++k)
        for ( int fid = 0; fid < nf; ++fid)
            cvals->SetComponent( fid, k, _mfn( fid, k));
}   // end _makeArray


void FaceSurfaceMapper::_makeArrayNoTx( const r3d::Mesh &mesh, vtkFloatArray *cvals) const
{ _makeArray(mesh, cvals);}


// For vertex mapping, depending on how the actor's polydata have been created, there could be the
// same number of points as there are vertices in the mesh (if texture mapping was not done) or
// there could be three times the number of triangles (if texture mapping was done). In the first
// case, setting the value is a straight forward one-to-one mapping. However, in the second case,
// vertices that share faces will be duplicated and so each unique vertex metric value needs to
// be mapped to all of these duplicate corresponding points in the array.
void VertexSurfaceMapper::_makeArrayNoTx( const r3d::Mesh &mesh, vtkFloatArray *cvals) const
{
    const size_t nd = dims();
    const int nv = int(mesh.numVtxs());
    cvals->SetNumberOfTuples( nv);
    for ( size_t k = 0; k < nd; ++k)
        for ( int vid = 0; vid < nv; ++vid)
            cvals->SetComponent( vid, k, _mfn( vid, k));    // One-to-one mapping
}   // end _makeArrayNoTx


void VertexSurfaceMapper::_makeArray( const r3d::Mesh &mesh, vtkFloatArray *cvals) const
{
    if ( !mesh.hasMaterials())
        _makeArrayNoTx( mesh, cvals);
    else
    {
        const size_t nd = dims();
        const int nv = int(mesh.numVtxs());
        float *vmap = (float*)malloc( nv*nd*sizeof(float));

        // For each dimension store the per vertex value from the delegate fn
        for ( size_t k = 0; k < nd; ++k)
            for ( int vid = 0; vid < nv; ++vid)
                vmap[nd*vid+k] = _mfn( vid, k);

        const int nf = int(mesh.numFaces());
        cvals->SetNumberOfTuples( 3*nf);
        int i = 0;
        for ( int fid = 0; fid < nf; ++fid)
        {
            const int* fvidxs = mesh.fvidxs(fid);
            cvals->SetTuple( i++, &vmap[nd*fvidxs[0]]);
            cvals->SetTuple( i++, &vmap[nd*fvidxs[1]]);
            cvals->SetTuple( i++, &vmap[nd*fvidxs[2]]);
        }   // end for

        free(vmap);
    }   // end else
}   // end _makeArray
