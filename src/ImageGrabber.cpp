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

#include <ImageGrabber.h>
#include <VtkTools.h>
using r3dvis::ImageGrabber;
using r3dvis::Viewer;

// public
ImageGrabber::ImageGrabber( vtkRenderWindow* rw, int h) : _renWin(rw)
{
    refresh(h);
}   // end ctor

// public
ImageGrabber::ImageGrabber( Viewer& v, int h) : _renWin(v.renderWindow())
{
    refresh(h);
}   // end ctor


// public
void ImageGrabber::refresh( int reqHeight)
{
    if ( reqHeight <= 0)
        reqHeight = _renWin->GetSize()[1];
    // Get the raw input images at their view size
    const cv::Mat_<cv::Vec3b> cimg = r3dvis::extractImage( _renWin);
    const cv::Mat_<float> dimg = r3dvis::extractZBuffer( _renWin);

    const cv::Size REQSZ( cvRound( reqHeight * double(cimg.cols)/cimg.rows), reqHeight);
    cv::resize( dimg, _dzmap, REQSZ); // Resize depth image to required dims
    cv::resize( cimg, _colmap, REQSZ); // Resize colour image to required dims
    // Get the contrast stretched CIE-L component
    _dcmap = r3dvis::contrastStretch( r3dvis::getLightness( _colmap, 255, CV_8U));

    // Make a depth byte map from the reduced size z-buffer map
    double mn, mx;
    cv::minMaxLoc( _dzmap, &mn, &mx);
    cv::Mat_<float> rngMap = _dzmap.clone();
    rngMap -= float(mn);   // Make nearest point 0
    const float depthProp = 1.0;    // Proportion of depth buffer to use (also try 1/2)
    rngMap.convertTo( _ddmap, CV_8U, -255./((mx-mn)*depthProp), 255);
}   // end refresh
