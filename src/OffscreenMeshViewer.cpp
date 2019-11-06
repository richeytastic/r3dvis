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

#include <OffscreenMeshViewer.h>
#include <VtkActorCreator.h>
#include <ImageGrabber.h>
using r3dvis::OffscreenMeshViewer;
using r3dvis::byte;
using r3d::CameraParams;


OffscreenMeshViewer::OffscreenMeshViewer( const cv::Size& dims, float rng)
    : _actor(nullptr), _picker(nullptr)
{
    _viewer = Viewer::create(true/*offscreen*/);
    _viewer->renderer()->UseFXAAOn();
    setSize(dims);
    setCamera( CameraParams( r3d::Vec3f( 0, 0, rng)));
}   // end ctor


OffscreenMeshViewer::~OffscreenMeshViewer()
{
    clear();
    if ( _picker)
        delete _picker;
}   // end dtor


void OffscreenMeshViewer::setBackgroundColour( double r, double g, double b)
{
    _viewer->renderer()->SetBackground(r,g,b);
}   // end setBackgroundColour


void OffscreenMeshViewer::setSize( const cv::Size& dims)
{
    _viewer->setSize( static_cast<size_t>(dims.width), static_cast<size_t>(dims.height));
    setCamera( _viewer->camera());  // Refresh
}   // end setSize


void OffscreenMeshViewer::clear()
{
    if ( _actor)
    {
        _viewer->removeActor(_actor);
        _actor = nullptr;
        setCamera( _viewer->camera());  // Refresh
    }   // end if
}   // end clear


void OffscreenMeshViewer::setModel( const r3d::Mesh& model)
{
    clear();
    // Create the actor
    _actor = VtkActorCreator::generateActor( model);
    _viewer->addActor( _actor);
    setCamera( _viewer->camera());  // Refresh
}   // end setModel


void OffscreenMeshViewer::setCamera( const CameraParams& cp)
{
    _viewer->setCamera( cp);
    _viewer->resetClippingRange();
    _viewer->updateRender();
}   // end setCamera


cv::Mat_<cv::Vec3b> OffscreenMeshViewer::snapshot() const
{
    ImageGrabber ig(*_viewer);
    return ig.colour();
}   // end snapshot


cv::Mat_<byte> OffscreenMeshViewer::lightnessSnapshot() const
{
    ImageGrabber ig(*_viewer);
    return ig.light();
}   // end lightnessSnapshot


bool OffscreenMeshViewer::pick( const cv::Point2f& p) const
{
    return picker()->pickActor(p) != nullptr;
}   // end pick


r3d::Vec3f OffscreenMeshViewer::worldPosition( const cv::Point2f& p) const
{
    return picker()->pickWorldPosition(p);
}   // end worldPosition


cv::Point2f OffscreenMeshViewer::imagePlane( const r3d::Vec3f& v) const
{
    const cv::Size sz = _viewer->size();
    cv::Point p = picker()->projectToImagePlane(v);
    return cv::Point2f( float(p.x)/sz.width, float(p.y)/sz.height);
}   // end imagePlane


r3dvis::RendererPicker* OffscreenMeshViewer::picker() const
{
    if ( !_picker)
        _picker = new RendererPicker( _viewer->renderer(), RendererPicker::TOP_LEFT);
    return _picker;
}   // end picker
