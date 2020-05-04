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

#include <RendererPicker.h>
#include <vtkCellPicker.h>
#include <vtkPropPicker.h>
#include <vtkWorldPointPicker.h>
#include <vtkCoordinate.h>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cassert>
using r3dvis::RendererPicker;
using r3dvis::ActorSubset;
using r3d::Vec3f;


namespace {
cv::Point _toPxls( vtkRenderer *ren, const cv::Point2f& p)
{
    const int w = ren->GetSize()[0];
    const int h = ren->GetSize()[1];
    return cv::Point( std::min( int(p.x * w), w-1), std::min( int(p.y * h), h-1));
}   // end _toPxls


bool _isValidPoint( vtkRenderer *ren, const cv::Point& p)
{
    return (p.x >= 0) && (p.y >= 0) && (p.x < ren->GetSize()[0]) && (p.y < ren->GetSize()[1]);
}   // end _isValidPoint
}   // end namespace


RendererPicker::RendererPicker( vtkRenderer* ren, PointOrigin po, double t)
    : _ren(ren), _pointOrigin(po), _tolerance(t)
{
    assert(ren);
    _ren->ResetCameraClippingRange();
}   // end ctor


namespace {
cv::Point changeOriginOfPoint( vtkRenderer* ren, const cv::Point& p, RendererPicker::PointOrigin po)
{
    if ( po == RendererPicker::BOTTOM_LEFT)
        return p;
    // p is given using a top left origin, so need to create and return a new point
    // that treats the bottom left as the origin so the algorithms work as per VTK's expectations.
    return cv::Point( p.x, ren->GetSize()[1] - p.y - 1);
}   // end changeOriginOfPoint
}   // end namespace


int RendererPicker::pickActorCells( const std::vector<cv::Point>& points2d, std::vector<ActorSubset>& picked) const
{
    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
    cellPicker->SetTolerance( _tolerance);

    std::unordered_map<vtkActor*, std::unordered_set<int> > actorCells;
    for ( const cv::Point& p : points2d)
    {
        if ( !_isValidPoint( _ren, p))
            continue;

        const cv::Point np = changeOriginOfPoint( _ren, p, _pointOrigin);
        propPicker->Pick( np.x, np.y, 0, _ren);
        vtkActor* actor = propPicker->GetActor();
        if ( !actor)
            continue;

        cellPicker->Pick( np.x, np.y, 0, _ren);
        const int cellId = cellPicker->GetCellId();
        actorCells[actor].insert(cellId);
    }   // end for

    int numActorsPicked = 0;
    // Copy the picked actors and their cells to the output parameter
    for ( const auto& pickedActor : actorCells)
    {
        numActorsPicked++;
        picked.resize( picked.size() + 1);
        ActorSubset& actorSubset = *picked.rbegin();
        actorSubset.actor = pickedActor.first;
        const std::unordered_set<int>& actorCellIds = pickedActor.second;
        actorSubset.cellIds.insert( actorSubset.cellIds.end(), actorCellIds.begin(), actorCellIds.end());
    }   // end for

    return numActorsPicked;
}   // end pickActorCells


int RendererPicker::pickActorCells( const std::vector<cv::Point>& points2d,
                                    vtkActor* actor, std::vector<int>& cellIds) const
{
    if ( !actor)
        return 0;

    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
    vtkSmartPointer<vtkPropCollection> pickFrom = vtkSmartPointer<vtkPropCollection>::New();
    pickFrom->AddItem(actor);
    cellPicker->SetTolerance( _tolerance);

    std::unordered_set<int> setCellIds;   // Avoid duplicate cell IDs being picked
    for ( const cv::Point& p : points2d)
    {
        if ( !_isValidPoint( _ren, p))
            continue;

        const cv::Point np = changeOriginOfPoint( _ren, p, _pointOrigin);
        propPicker->PickProp( np.x, np.y, _ren, pickFrom);
        if ( propPicker->GetActor() == actor)
        {
            cellPicker->Pick( np.x, np.y, 0, _ren);
            setCellIds.insert( cellPicker->GetCellId());
        }   // end if
    }   // end for

    cellIds.insert( cellIds.end(), setCellIds.begin(), setCellIds.end());
    return (int)setCellIds.size();
}   // end pickActorCells


vtkActor* RendererPicker::pickActor( const cv::Point& p) const
{
    if ( !_isValidPoint( _ren, p))
        return nullptr;

    const cv::Point np = changeOriginOfPoint( _ren, p, _pointOrigin);
    vtkNew<vtkPropPicker> ppicker;
    vtkActor* act = nullptr;
    if ( ppicker->PickProp( np.x, np.y, _ren) > 0)
        act = ppicker->GetActor();
    return act;
}   // end pickActor


vtkActor* RendererPicker::pickActor( const cv::Point2f& p) const { return pickActor( _toPxls(_ren, p));}


namespace
{

vtkSmartPointer<vtkPropCollection> createPropCollection( const std::vector<vtkActor*>& possActors)
{
    vtkSmartPointer<vtkPropCollection> pickFrom = vtkSmartPointer<vtkPropCollection>::New();
    std::for_each( std::begin(possActors), std::end(possActors), [&](vtkActor* a){pickFrom->AddItem(a);});
    return pickFrom;
}   // end createPropCollection

/*
vtkSmartPointer<vtkPropCollection> createPropCollection( const std::vector<vtkSmartPointer<vtkActor> >& possActors)
{
    vtkSmartPointer<vtkPropCollection> pickFrom = vtkSmartPointer<vtkPropCollection>::New();
    std::for_each( std::begin(possActors), std::end(possActors), [&](vtkActor a){pickFrom->AddItem(a.GetPointer());});
    return pickFrom;
}   // end createPropCollection
*/

vtkActor* pick( const cv::Point& p, vtkSmartPointer<vtkPropCollection> pickFrom, vtkRenderer* ren, RendererPicker::PointOrigin po)
{
    if ( !_isValidPoint( ren, p))
        return nullptr;

    const cv::Point np = changeOriginOfPoint( ren, p, po);
    vtkSmartPointer<vtkPropPicker> propPicker = vtkSmartPointer<vtkPropPicker>::New();
    propPicker->PickProp( np.x, np.y, ren, pickFrom);
    vtkActor* act = propPicker->GetActor();
    return act;
}   // end pick
}   // end namespace


vtkActor* RendererPicker::pickActor( const cv::Point& p, const std::vector<vtkActor*>& possActors) const
{
    return pick( p, createPropCollection( possActors), _ren, _pointOrigin);
}   // end pickActor


vtkActor* RendererPicker::pickActor( const cv::Point2f& p, const std::vector<vtkActor*>& possActors) const { return pickActor( _toPxls(_ren, p), possActors);} 


int RendererPicker::pickCell( const cv::Point& p) const
{
    if ( !_isValidPoint( _ren, p))
        return -1;
    const cv::Point np = changeOriginOfPoint( _ren, p, _pointOrigin);
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance( _tolerance);
    picker->Pick( np.x, np.y, 0, _ren);
    const int cid = picker->GetCellId();
    return cid;
}   // end pickCell


int RendererPicker::pickCell( const cv::Point2f& p) const { return pickCell( _toPxls(_ren, p));}


Vec3f RendererPicker::pickWorldPosition( const cv::Point& p) const
{
    if ( !_isValidPoint( _ren, p))
        return Vec3f::Zero();
    const cv::Point np = changeOriginOfPoint( _ren, p, _pointOrigin);
    vtkSmartPointer<vtkWorldPointPicker> picker = vtkSmartPointer<vtkWorldPointPicker>::New();   // Hardware accelerated
    picker->Pick( np.x, np.y, 0, _ren);
    const double* wpos = picker->GetPickPosition();
    const Vec3f v( (float)wpos[0], (float)wpos[1], (float)wpos[2]);
    return v;
}   // end pickWorldPosition


Vec3f RendererPicker::pickWorldPosition( const cv::Point2f& p) const { return pickWorldPosition( _toPxls(_ren, p));}



Vec3f RendererPicker::pickNormal( const cv::Point& p) const
{
    if ( !_isValidPoint( _ren, p))
        return Vec3f::Zero();
    const cv::Point np = changeOriginOfPoint( _ren, p, _pointOrigin);
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    Vec3f v(0,0,0);
    if ( picker->Pick( np.x, np.y, 0, _ren))
    {
        const double* normal = picker->GetPickNormal();
        v = Vec3f( (float)normal[0], (float)normal[1], (float)normal[2]);
    }   // end if
    return v;
}   // end pickNormal


Vec3f RendererPicker::pickNormal( const cv::Point2f& p) const { return pickNormal( _toPxls(_ren, p));}


cv::Point RendererPicker::projectToImagePlane( const Vec3f& v) const
{
    vtkSmartPointer<vtkCoordinate> coordConverter = vtkSmartPointer<vtkCoordinate>::New();
    coordConverter->SetCoordinateSystemToWorld();
    coordConverter->SetValue( v[0], v[1], v[2]);
    const int* dpos = coordConverter->GetComputedDisplayValue( _ren);
    const cv::Point p( dpos[0], dpos[1]); // Bottom left origin
    return changeOriginOfPoint( _ren, p, _pointOrigin);
}   // end projectToImagePlane

