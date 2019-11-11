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

#ifndef r3dvis_ScalarLegend_H
#define r3dvis_ScalarLegend_H

#include "r3dvis_Export.h"
#include <vtkRenderWindowInteractor.h>
#include <vtkScalarBarWidget.h>
#include <vtkSmartPointer.h>
#include <vtkTextProperty.h>
#include <vtkLookupTable.h>
#include <vtkMapper.h>
#include <string>

namespace r3dvis {

class r3dvis_EXPORT ScalarLegend
{
public:
    explicit ScalarLegend( vtkRenderWindowInteractor*);
    virtual ~ScalarLegend(){}

    vtkTextProperty* titleProperty();
    vtkTextProperty* labelProperty();

    void setTitle( const std::string&);
    void setLookupTable( vtkLookupTable*);

    void setVisible( bool);
    bool isVisible() const;

private:
    vtkSmartPointer<vtkScalarBarWidget> _widget;
    bool _shown;
};  // end class

}   // end namespace

#endif
