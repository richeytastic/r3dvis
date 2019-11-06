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

#ifndef r3dvis_InteractorC1_H
#define r3dvis_InteractorC1_H

#include "Viewer.h"
#include "KeyPresser.h"
#include <vtkInteractorStyleTrackballCamera.h>

namespace r3dvis {

class r3dvis_EXPORT InteractorC1 : public vtkInteractorStyleTrackballCamera
{
public:
    static InteractorC1 *New();
    vtkTypeMacro( InteractorC1, vtkInteractorStyleTrackballCamera);

    virtual void OnKeyPress();
    void setKeyPresser( KeyPresser::Ptr);

private:
    KeyPresser::Ptr keyPresser;
    InteractorC1();
}; // end class

}   // end namespace

#endif
