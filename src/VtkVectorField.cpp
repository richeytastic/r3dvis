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

#include <VtkVectorField.h>
#include <vtkProperty.h>
#include <VtkTools.h>
using r3dvis::VtkVectorField;


VtkVectorField::Ptr VtkVectorField::create( const vtkPolyData* inputData, bool useNormal)
{
    return Ptr( new VtkVectorField( inputData, useNormal));
}   // end create


VtkVectorField::VtkVectorField( const vtkPolyData* inputData, bool useNormal)
{
    _arrow->SetShaftRadius( 0.06);
    _arrow->SetTipLength( 0.30);
    _arrow->SetTipRadius( 0.18);

    _glyph->SetSourceConnection( _arrow->GetOutputPort());
    _glyph->SetInputData( const_cast<vtkPolyData*>(inputData));
    _glyph->SetScaleFactor( 1.0);

    if ( useNormal)
        _glyph->SetVectorModeToUseNormal();
    else
        _glyph->SetVectorModeToUseVector();

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


void VtkVectorField::copyProperties( const VtkVectorField& sa)
{
    setPickable(sa.pickable());
    setScaleFactor(sa.scaleFactor());
    setVisible(sa.visible());
    setColour(sa.colour());
    setOpacity(sa.opacity());
    pokeTransform(sa._actor->GetMatrix());
}   // end copyProperties


void VtkVectorField::setColourMap( const vtkLookupTable *ltab, double minVal, double maxVal)
{
    _glyph->SetColorModeToColorByScalar();
    _actor->GetMapper()->SetLookupTable( const_cast<vtkLookupTable*>(ltab));
    _actor->GetMapper()->SetScalarRange( minVal, maxVal);
    _actor->GetMapper()->SetScalarVisibility( ltab != nullptr);
    _glyph->Update();
}   // end setColourMap


void VtkVectorField::setScaleFactor( double f) { _glyph->SetScaleFactor(f);}
double VtkVectorField::scaleFactor() const { return _glyph->GetScaleFactor();}

void VtkVectorField::setPickable( bool v) { _actor->SetPickable(v);}
bool VtkVectorField::pickable() const { return _actor->GetPickable() != 0;}

void VtkVectorField::setVisible( bool v) { _actor->SetVisibility(v);}
bool VtkVectorField::visible() const { return _actor->GetVisibility() != 0;}

void VtkVectorField::setColour( double r, double g, double b)
{
    _actor->GetMapper()->SetScalarVisibility(false);
    _actor->GetProperty()->SetColor( r, g, b);
}   // end setColour

void VtkVectorField::setColour( const double c[3]) { setColour( c[0], c[1], c[2]);}

const double* VtkVectorField::colour() const { return _actor->GetProperty()->GetColor();}

void VtkVectorField::setOpacity( double a) { _actor->GetProperty()->SetOpacity(a);}
double VtkVectorField::opacity() const { return _actor->GetProperty()->GetOpacity();}


void VtkVectorField::pokeTransform( const vtkMatrix4x4* vm) { _actor->PokeMatrix( const_cast<vtkMatrix4x4*>(vm));}

const vtkMatrix4x4* VtkVectorField::transform() const { return _actor->GetMatrix();}

void VtkVectorField::fixTransform() { r3dvis::fixTransform( _actor);}
