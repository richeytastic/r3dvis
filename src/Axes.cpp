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

#include <Axes.h>
using r3dvis::Axes;


Axes::Axes( vtkRenderWindowInteractor* rwint)
{
    _actor = vtkSmartPointer<vtkAxesActor>::New();
    _widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    _widget->SetOutlineColor( 0.9, 0.5, 0.1);
    _widget->SetOrientationMarker(_actor);
    if ( rwint)
        _widget->SetInteractor( rwint);
    _widget->SetViewport( 0.0, 0.0, 0.4, 0.4);
    setVisible(true);
    setEnabled(true);
}   // end ctor


void Axes::setVisible( bool v) { _widget->SetEnabled((int)v);}
bool Axes::isVisible() const { return _widget->GetEnabled() > 0;}
void Axes::setEnabled( bool v) { _widget->SetInteractive( v);}
bool Axes::isEnabled() const { return _widget->GetInteractive() > 0;}

