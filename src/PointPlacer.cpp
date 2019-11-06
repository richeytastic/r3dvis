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

#include <PointPlacer.h>
using r3dvis::PointPlacer;


PointPlacer::Ptr PointPlacer::create( const vtkRenderer* r)
{
    return Ptr( new PointPlacer(r), [](PointPlacer* d){ delete d;});
}   // end create


PointPlacer::Ptr PointPlacer::create( const vtkRenderer* r, vtkPolygonalSurfacePointPlacer* p)
{
    return Ptr( new PointPlacer(r,p), [](PointPlacer* d){ delete d;});
}   // end create


// private
PointPlacer::PointPlacer( const vtkRenderer* r)
    : _renderer(const_cast<vtkRenderer*>(r)), _pplacer( vtkPolygonalSurfacePointPlacer::New()), _delplacer(true)
{}   // end ctor


// private
PointPlacer::PointPlacer( const vtkRenderer* r, vtkPolygonalSurfacePointPlacer* p)
    : _renderer(const_cast<vtkRenderer*>(r)), _pplacer( p), _delplacer(false)
{}   // end ctor


// private
PointPlacer::~PointPlacer()
{
    if ( _delplacer)
        _pplacer->Delete();
}   // end dtor


void PointPlacer::set( const vtkProp* p)
{
    _pplacer->RemoveAllProps();
    _pplacer->AddProp( const_cast<vtkProp*>(p));
}   // end set


void PointPlacer::add( const vtkProp* p)
{
    _pplacer->AddProp( const_cast<vtkProp*>(p));
}   // end add


void PointPlacer::clear()
{
    _pplacer->RemoveAllProps();
}   // end clear


bool PointPlacer::hasProps() const
{
    return _pplacer->GetNumberOfProps() > 0;
}   // end hasProps


bool PointPlacer::calcSurfacePosition( int x, int y, float* worldPos, r3dvis::DisplayOrigin dispOrig) const
{
    if ( _pplacer->GetNumberOfProps() == 0)
        return false;

    if ( dispOrig == r3dvis::TOP_LEFT_DISPLAY_ORIGIN)
        y = _renderer->GetSize()[1] - y - 1;    // Convert to VTK display origin (BOTTOM_LEFT_DISPLAY_ORIGIN)

    double dpos[2] = {(double)x, (double)y};
    if ( !_pplacer->ValidateDisplayPosition( _renderer, dpos))
        return false;

    double wpos[3];
    double worient[9];  // not used
    if (!_pplacer->ComputeWorldPosition( _renderer, dpos, wpos, worient))
        return false;

    worldPos[0] = static_cast<float>(wpos[0]);
    worldPos[1] = static_cast<float>(wpos[1]);
    worldPos[2] = static_cast<float>(wpos[2]);

    return true;
}   // end calcSurfacePosition

