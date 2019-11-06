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

#include <VtkActorCreator.h>
#include <VtkTools.h>
#include <cassert>
#include <vtkPoints.h>
#include <vtkTexture.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
using r3dvis::VtkActorCreator;
using r3d::Mesh;
using r3d::Vec3f;
using r3d::Vec2f;


namespace {

void init()
{
    static bool initCalled = false;
    if (!initCalled)
    {
        // Add static initialisation here...
        vtkMapper::SetResolveCoincidentTopologyToPolygonOffset();
        initCalled = true;
    }   // end if
}   // end ctor


vtkSmartPointer<vtkActor> makeActor( vtkSmartPointer<vtkPolyData> pd)
{
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData( pd);
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper);
    return actor;
}   // end makeActor


vtkSmartPointer<vtkPoints> createVerts( const std::vector<Vec3f>& vtxs, vtkCellArray* vertices)
{
    const int n = (int)vtxs.size();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints( n);
    for ( int i = 0; i < n; ++i)
    {
        points->SetPoint( i, &vtxs[i][0]);
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
    }   // end for
    return points;
}   // end createVerts


vtkSmartPointer<vtkPoints> createSequenceVerts( const Mesh& model, vtkCellArray* vertices)
{
    assert( model.hasSequentialVertexIds());
    const int n = model.numVtxs();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints( n);
    for ( int i = 0; i < n; ++i)
    {
        points->SetPoint( i, &model.uvtx( i)[0]);
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
    }   // end for
    return points;
}   // end createSequenceVerts


vtkSmartPointer<vtkPoints> createRandomVerts( const Mesh& model, vtkCellArray* vertices)
{
    const IntSet& vidxs = model.vtxIds();
    const int n = model.numVtxs();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints( n);
    int i = 0;
    for ( int vid : vidxs)
    {
        points->SetPoint( i, &model.uvtx( vid)[0]);
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
        i++;
    }   // end for
    return points;
}   // end createRandomVerts


vtkSmartPointer<vtkPoints> createRandomVerts( const Mesh& model, const IntSet& vidxs, vtkCellArray* vertices)
{
    const int n = static_cast<int>(vidxs.size());
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints( n);
    int i = 0;
    for ( int vid : vidxs)
    {
        points->SetPoint( i, &model.uvtx( vid)[0]);
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
        i++;
    }   // end for
    return points;
}   // end createRandomVerts


vtkSmartPointer<vtkPoints> createLines( const std::vector<Vec3f>& vtxs,
                                        vtkCellArray* vertices, vtkCellArray* lines,
                                        bool joinLoop)
{
    const int n = static_cast<int>(vtxs.size());
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints( n);
    for ( int i = 0; i < n; ++i)
    {
        points->SetPoint( i, &vtxs[i][0]);
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
    }   // end for

    if ( joinLoop)
    {
        lines->InsertNextCell(2);
        lines->InsertCellPoint(n-1);
        lines->InsertCellPoint(0);
    }   // end if

    for ( int i = 1; i < n; ++i)
    {
        lines->InsertNextCell(2);
        lines->InsertCellPoint(i-1);
        lines->InsertCellPoint(i);
    }   // end for

    return points;
}   // end createLines


vtkSmartPointer<vtkPoints> createLines( const Mesh& model, const std::list<int>& vidxs,
                                        vtkCellArray* vertices, vtkCellArray* lines, bool joinLoop)
{
    const int n = static_cast<int>(vidxs.size());
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints( n);
    int i = 0;
    for ( int vid : vidxs)
    {
        points->SetPoint( i, &model.uvtx(vid)[0]);
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
        i++;
    }   // end for

    if ( joinLoop)
    {
        lines->InsertNextCell(2);
        lines->InsertCellPoint(n-1);
        lines->InsertCellPoint(0);
    }   // end if

    for ( int j = 1; j < n; ++j)
    {
        lines->InsertNextCell(2);
        lines->InsertCellPoint(j-1);
        lines->InsertCellPoint(j);
    }   // end for

    return points;
}   // end createLines


vtkSmartPointer<vtkPoints> createLinePairs( const std::vector<Vec3f>& lps,
                                        vtkCellArray* vertices, vtkCellArray* lines)
{
    const int n = (int)lps.size();
    assert( n % 2 == 0);
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints( n);
    for ( int i = 0; i < n; ++i)
    {
        points->SetPoint( i, &lps[i][0]);
        vertices->InsertNextCell(1);
        vertices->InsertCellPoint(i);
    }   // end for

    const int m = n/2;
    for ( int i = 0; i < m; ++i)
    {
        lines->InsertNextCell(2);
        lines->InsertCellPoint(2*i+0);
        lines->InsertCellPoint(2*i+1);
    }   // end for

    return points;
}   // end createLinePairs

}   // end namespace


vtkSmartPointer<vtkActor> VtkActorCreator::generatePointsActor( const Mesh& model)
{
    init();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> points = model.hasSequentialVertexIds() ? createSequenceVerts( model, vertices) : createRandomVerts( model, vertices);
    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->SetVerts( vertices);
    pd->SetPoints( points);

    vtkSmartPointer<vtkActor> actor = makeActor(pd);
    actor->PokeMatrix( r3dvis::toVTK( model.transformMatrix()));
    return actor;
}   // end generatePointsActor


vtkSmartPointer<vtkActor> VtkActorCreator::generatePointsActor( const Mesh& model, const IntSet& vidxs)
{
    init();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> points = createRandomVerts( model, vidxs, vertices);
    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->SetVerts( vertices);
    pd->SetPoints( points);

    vtkSmartPointer<vtkActor> actor = makeActor(pd);
    actor->PokeMatrix( r3dvis::toVTK( model.transformMatrix()));
    return actor;
}   // end generatePointsActor


vtkSmartPointer<vtkActor> VtkActorCreator::generatePointsActor( const std::vector<Vec3f>& vtxs)
{
    init();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> points = createVerts( vtxs, vertices);
    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->SetVerts( vertices);
    pd->SetPoints( points);
    return makeActor( pd);
}   // end generatePointsActor


vtkSmartPointer<vtkActor> VtkActorCreator::generateLineActor( const std::vector<Vec3f>& vtxs, bool joinLoop)
{
    init();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> points = createLines( vtxs, vertices, lines, joinLoop);
    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->SetVerts( vertices);
    pd->SetPoints( points);
    pd->SetPolys( lines);
    return makeActor( pd);
}   // end generateLineActor


vtkSmartPointer<vtkActor> VtkActorCreator::generateLineActor( const std::list<Vec3f>& vtxs, bool joinLoop)
{
    std::vector<Vec3f> vpts(vtxs.begin(), vtxs.end());
    return generateLineActor( vpts, joinLoop);
}   // end generateLineActor


vtkSmartPointer<vtkActor> VtkActorCreator::generateLineActor( const Mesh& model, const std::list<int>& vidxs, bool joinLoop)
{
    init();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> points = createLines( model, vidxs, vertices, lines, joinLoop);
    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->SetVerts( vertices);
    pd->SetPoints( points);
    pd->SetPolys( lines);

    vtkSmartPointer<vtkActor> actor = makeActor( pd);
    actor->PokeMatrix( r3dvis::toVTK( model.transformMatrix()));
    return actor;
}   // end generateLineActor


vtkSmartPointer<vtkActor> VtkActorCreator::generateLinePairsActor( const std::vector<Vec3f>& lps)
{
    init();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> points = createLinePairs( lps, vertices, lines);
    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->SetVerts( vertices);
    pd->SetPoints( points);
    pd->SetPolys( lines);
    return makeActor( pd);
}   // end generateLinePairsActor


namespace {
vtkSmartPointer<vtkPoints> createSequencePoints( const Mesh& model)
{
    assert( model.hasSequentialVertexIds());
    const int n = model.numVtxs();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints( n);
    for ( int i = 0; i < n; ++i)
        points->SetPoint( i, &model.uvtx( i)[0]);
    return points;
}   // end createSequencePoints


vtkSmartPointer<vtkCellArray> createSequencePolys( const Mesh& model)
{
    assert( model.hasSequentialFaceIds());
    const int n = int(model.numFaces());
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    for ( int f = 0; f < n; ++f)
    {
        const int* fvidxs = model.fvidxs(f);
        polys->InsertNextCell( 3);
        polys->InsertCellPoint( fvidxs[0]);
        polys->InsertCellPoint( fvidxs[1]);
        polys->InsertCellPoint( fvidxs[2]);
    }   // end for
    return polys;
}   // end createSequencePolys


vtkSmartPointer<vtkPolyData> createSequencePolyData( const Mesh& model)
{
    vtkSmartPointer<vtkPoints> points = createSequencePoints( model);
    vtkSmartPointer<vtkCellArray> faces = createSequencePolys( model);
    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->SetPoints( points);
    pd->SetPolys( faces);
    pd = r3dvis::generateNormals( pd);    // Required for interpolated shading
    return pd;
}   // end createSequencePolyData

}   // end namespace


vtkSmartPointer<vtkActor> VtkActorCreator::generateSurfaceActor( const Mesh& model)
{
    assert( model.hasSequentialIds());
    if ( !model.hasSequentialIds())
    {
        std::cerr << "[ERROR] r3dvis::VtkActorCreator::generateSurfaceActor: Vertex/Face IDs must be in sequential order!" << std::endl;
        return nullptr;
    }   // end if

    init();
    vtkSmartPointer<vtkPolyData> pd = createSequencePolyData( model);
    vtkSmartPointer<vtkActor> actor = makeActor( pd);
    actor->PokeMatrix( r3dvis::toVTK( model.transformMatrix()));
    return actor;
}   // end generateSurfaceActor


vtkSmartPointer<vtkActor> VtkActorCreator::generateActor( const Mesh& model)
{
    if ( model.numMats() > 1)  // Can't create if more than one material!
    {
        std::cerr << "[ERROR] r3dvis::VtkActorCreator::generateActor: Model has more than one material! Merge first." << std::endl;
        return nullptr;
    }   // end if

    if ( !model.hasSequentialIds())
    {
        std::cerr << "[ERROR] r3dvis::VtkActorCreator::generateActor: Model IDs must be in sequential order!" << std::endl;
        return nullptr;
    }   // end if

    if ( model.numMats() == 0)
    {
        std::cerr << "[INFO] r3dvis::VtkActorCreator::generateActor: Model has no materials; generating surface actor." << std::endl;
        return generateSurfaceActor( model);
    }   // end if

    init();

    const int MID = *model.materialIds().begin();   // The one and only material ID
    vtkSmartPointer<vtkTexture> texture = r3dvis::convertToTexture( model.texture(MID));

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> faces = vtkSmartPointer<vtkCellArray>::New();

    // Need THREE TIMES the number of triangles in points because VTK only does per vertex texture
    // mapping so need more points than necessary to correspond with the texture vertices.
    const int NP = 3*int(model.numFaces());
    points->SetNumberOfPoints( NP);

    // The array to hold the 2D texture coordinates.
    vtkSmartPointer<vtkFloatArray> uvs = vtkSmartPointer<vtkFloatArray>::New();
    uvs->SetNumberOfComponents(2);
    uvs->SetNumberOfTuples( NP);
    uvs->SetName( "TCoords_0");

    // We need to map vertex normals to the texture mapped points. We can't use VTKs built in normal generator algorithm
    // directly on the cell array being populated here because this expects it to be connected with common vertex IDs
    // which it is not because VTK doesn't do per polygon texture mapping (which is really annoying). To hack around this,
    // we create a second temporary poly data object as though we're just making a surface model, and map the point normals
    // calculated on it to a float array which we use to manually set the normals.
    vtkSmartPointer<vtkPolyData> tmpPolyData = createSequencePolyData( model);
    vtkDataArray* cnrms = tmpPolyData->GetPointData()->GetNormals();
    double nv[3];
    vtkSmartPointer<vtkFloatArray> nrm = vtkSmartPointer<vtkFloatArray>::New();
    nrm->SetNumberOfComponents(3);
    nrm->SetNumberOfTuples( NP);
    nrm->SetName( "Normals_0");

    int vtkPointId = 0;
    const int nfaces = int(model.numFaces());
    for ( int fid = 0; fid < nfaces; ++fid)    // Over all polygons
    {
        faces->InsertNextCell(3);
        const int* fvidxs = model.fvidxs(fid);
        const int* uvids = model.faceUVs(fid);

        for ( int i = 0; i < 3; ++i)
        {
            const int vidx = fvidxs[i];
            faces->InsertCellPoint( vtkPointId);
            points->SetPoint( vtkPointId, &(model.uvtx( vidx)[0]));

            if ( uvids)
            {
                const Vec2f& uv = model.uv( MID, uvids[i]);
                uvs->SetTuple2( vtkPointId, uv[0], uv[1]);
            }   // end if
            else
                uvs->SetTuple2( vtkPointId, 0.0, 0.0);

            cnrms->GetTuple( vidx, nv);
            nrm->SetTuple3( vtkPointId, static_cast<float>(nv[0]), static_cast<float>(nv[1]), static_cast<float>(nv[2]));

            vtkPointId++;
        }   // end for
    }   // end for

    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();

    pd->SetPoints( points);
    pd->SetPolys( faces);
    pd->GetPointData()->SetTCoords( uvs);
    pd->GetPointData()->SetNormals( nrm);  // Required for interpolated shading

    vtkSmartPointer<vtkActor> actor = makeActor(pd);
    actor->SetTexture( texture);

    // Set ambient lighting for proper texture lighting
    actor->GetProperty()->SetAmbient(1.0);
    actor->GetProperty()->SetDiffuse(0.0);
    actor->GetProperty()->SetSpecular(0.0);

    actor->PokeMatrix( r3dvis::toVTK( model.transformMatrix()));
    return actor;
}   // end generateActor
