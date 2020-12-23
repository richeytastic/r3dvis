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

#ifndef r3dvis_VTK_VECTOR_FIELD_H
#define r3dvis_VTK_VECTOR_FIELD_H

#include "r3dvis_Export.h"
#include <vtkLookupTable.h>
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <memory>

namespace r3dvis {

class r3dvis_EXPORT VtkVectorField
{
public:
    using Ptr = std::shared_ptr<VtkVectorField>;

    // If useNormal is false, the vector array set on the polydata will be used.
    static Ptr create( const vtkPolyData*, bool useNormal=false);

    explicit VtkVectorField( const vtkPolyData*, bool useNormal=false);

    // Caller must ensure that the renderer the prop is added to
    // corresponds with the vtkLookupTable set (if any).
    const vtkActor* prop() const { return _actor;}
    vtkActor* prop() { return _actor;}

    void setPickable( bool);
    bool pickable() const;

    void setScaleFactor( double);
    double scaleFactor() const;

    void setVisible( bool);
    bool visible() const;

    void setColour( double r, double g, double b);
    void setColour( const double[3]);
    const double* colour() const;

    void setColourMap( const vtkLookupTable*, double minVal, double maxVal);

    void setOpacity( double);
    double opacity() const;

    void pokeTransform( const vtkMatrix4x4*);   // Directly adjust the actor's transform.
    const vtkMatrix4x4* transform() const;      // Return the actor's current transform.
    void fixTransform();

    // Copy properties from the provided actor to this one.
    void copyProperties( const VtkVectorField&);

private:
    vtkNew<vtkArrowSource> _arrow;
    vtkNew<vtkGlyph3D> _glyph;
    vtkNew<vtkActor> _actor;

    VtkVectorField( const VtkVectorField&) = delete;
    void operator=( const VtkVectorField&) = delete;
};  // end class

}   // end namespace

#endif
