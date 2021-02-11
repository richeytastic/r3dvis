/************************************************************************
 * Copyright (C) 2021 Richard Palmer
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

#ifndef r3dvis_SCALARLEGEND_H
#define r3dvis_SCALARLEGEND_H

#include "r3dvis_Export.h"
#include <vtkScalarBarActor.h>
#include <vtkTextProperty.h>
#include <vtkLookupTable.h>
#include <string>

namespace r3dvis {

class r3dvis_EXPORT ScalarLegend
{
public:
    ScalarLegend();

    vtkTextProperty* titleProperty();
    vtkTextProperty* labelProperty();

    void setTitle( const std::string&);

    void setPosition( double x, double y);
    void setHeight( double);
    void setWidth( double);

    void setNumLabels( int);
    void setLookupTable( const vtkLookupTable*);

    inline const vtkScalarBarActor *prop() const { return _legend.Get();}
    inline vtkScalarBarActor *prop() { return _legend.Get();}

private:
    vtkNew<vtkScalarBarActor> _legend;
};  // end class

}   // end namespace

#endif
