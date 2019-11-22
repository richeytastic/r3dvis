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

#include <VtkVectorMap.h>
#include <vtkProperty.h>
#include <VtkTools.h>
using r3dvis::VtkVectorMap;


VtkVectorMap::Ptr VtkVectorMap::create( vtkPolyData* inputData, bool useNormal)
{
    return Ptr( new VtkVectorMap( inputData, useNormal));
}   // end create


VtkVectorMap::VtkVectorMap( vtkPolyData* inputData, bool useNormal)
{
    _arrow->SetShaftRadius( 0.01);
    _arrow->SetTipLength( 0.2);
    _arrow->SetTipRadius( 0.04);

    _glyph->SetSourceConnection( _arrow->GetOutputPort());
    _glyph->SetInputData( inputData);
    _glyph->SetScaleFactor( 1.0);

    if ( useNormal)
        _glyph->SetVectorModeToUseNormal();
    else
        _glyph->SetVectorModeToUseVector();

    _glyph->Update();

    // Create the actor
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection( _glyph->GetOutputPort());
    mapper->SetScalarVisibility(false);
    _actor->SetMapper(mapper);

    // Ambient lighting only
    vtkProperty* property = _actor->GetProperty();
    property->SetAmbient(1.0);
    property->SetDiffuse(1.0);
    property->SetSpecular(0.0);
}   // end ctor


void VtkVectorMap::copyProperties( const VtkVectorMap& sa)
{
    setPickable(sa.pickable());
    setScaleFactor(sa.scaleFactor());
    setVisible(sa.visible());
    setColour(sa.colour());
    setOpacity(sa.opacity());
    pokeTransform(sa._actor->GetMatrix());
}   // end copyProperties


void VtkVectorMap::setScaleFactor( double f) { _glyph->SetScaleFactor(f);}
double VtkVectorMap::scaleFactor() const { return _glyph->GetScaleFactor();}

void VtkVectorMap::setPickable( bool v) { _actor->SetPickable(v);}
bool VtkVectorMap::pickable() const { return _actor->GetPickable() != 0;}

void VtkVectorMap::setVisible( bool v) { _actor->SetVisibility(v);}
bool VtkVectorMap::visible() const { return _actor->GetVisibility() != 0;}

void VtkVectorMap::setColour( double r, double g, double b) { _actor->GetProperty()->SetColor( r, g, b);}
void VtkVectorMap::setColour( const double c[3]) { _actor->GetProperty()->SetColor( const_cast<double*>(c));}
const double* VtkVectorMap::colour() const { return _actor->GetProperty()->GetColor();}

void VtkVectorMap::setOpacity( double a) { _actor->GetProperty()->SetOpacity(a);}
double VtkVectorMap::opacity() const { return _actor->GetProperty()->GetOpacity();}


void VtkVectorMap::pokeTransform( const vtkMatrix4x4* vm) { _actor->PokeMatrix( const_cast<vtkMatrix4x4*>(vm));}

const vtkMatrix4x4* VtkVectorMap::transform() const { return _actor->GetMatrix();}

void VtkVectorMap::fixTransform() { r3dvis::fixTransform( _actor);}
