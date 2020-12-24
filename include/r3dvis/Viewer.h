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

/**
 * Display generic actors in a VTK viewer.
 *
 * Richard Palmer
 * June/December 2011
 */

#ifndef r3dvis_VIEWER_H
#define r3dvis_VIEWER_H

#include "VTKTypes.h"
#include <r3d/CameraParams.h>
#include <memory>

namespace r3dvis {

class r3dvis_EXPORT Viewer
{
public:
    using Ptr = std::shared_ptr<Viewer>;
    static Ptr create( bool offscreenRendering=false);
    Viewer( bool offscreenRendering=false);

    // Add the provided actor. If this is the first actor,
    // subsequent actors will be placed relative to it.
    void addActor( vtkActor* actor);

    // Remove the provided actor from the viewer.
    void removeActor( vtkActor* actor);

    void clear();	// Remove all actors

    // Get/set the near and far clipping range values
    void setClippingRange( double near, double far);    // Default is 0.1, 1000
    void resetClippingRange();
    double clipNear() const;
    double clipFar() const;

    void setCamera( const r3d::CameraParams&);
    r3d::CameraParams camera() const;

    // Affects direction camera is looking in (i.e. modifies focus and up vector).
    void setCameraOrientation( double pitch, double yaw, double roll);
    void addCameraYaw( double yaw);
    void addCameraPitch( double pitch);
    void addCameraRoll( double roll);

    // Set perspective to be enabled if true or parallel projection if false.
    void setPerspective( bool enabled);

    // Set view scale for orthogonal view (only matters in parallel projection mode).
    void setParallelScale( double scale);

    // Set the interactor to be used on this viewer. Set the passed in interactor
    // with a custom style before or after calling this function. Alternatively,
    // use function setInteractorStyle below to attach a style to a new Interactor
    // which is set for the render window.
    void setInteractor( vtkRenderWindowInteractor*);
    void setInteractorStyle( vtkInteractorStyle*);

	// Change background colour to something between 0 and 255.
	void changeBackground( double c);

	// Set stereo rendering on or off and get the current stereo rendering value.
	void setStereoRendering( bool opt);
	bool stereoRendering() const;

    void setSize( size_t width, size_t height);
    cv::Size size() const;
    int width() const;
    int height() const;

	void updateRender();

    vtkRenderer* renderer() const { return _ren;}
	vtkRenderWindow* renderWindow() const { return _renWin;}

    // Create and return an image of the current window
    cv::Mat_<cv::Vec3b> extractBGR();

    // Extract Z buffer - ensure camera clipping range is set properly prior to using!
    cv::Mat_<float> extractZ();

private:
    vtkNew<vtkRenderer> _ren;
    vtkNew<vtkRenderWindow> _renWin;

    Viewer( const Viewer&) = delete;
    void operator=( const Viewer&) = delete;
};  // end class

}   // end namespace

#endif
