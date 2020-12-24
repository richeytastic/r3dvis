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

#ifndef r3dvis_ImageGrabber_H
#define r3dvis_ImageGrabber_H

#include "Viewer.h"
using byte = unsigned char;

namespace r3dvis {

class r3dvis_EXPORT ImageGrabber
{
public:
    // Grabs the current set of images from the passed in render window and scales to requested height.
    // If height is left as non-positive, produced images are the same size as the render window.
    ImageGrabber( vtkRenderWindow*, int reqPixelHeight=0);

    // Refresh images (only needed if render window has been updated since construction).
    void refresh( int reqPixelHeight = 0);

    inline cv::Size size() const { return _colmap.size();}
    inline cv::Mat_<cv::Vec3b> colour() const { return _colmap;}
    inline cv::Mat_<byte> light() const { return _dcmap;}
    inline cv::Mat_<float> depth() const { return _dzmap;}
    inline cv::Mat_<byte> depthb() const { return _ddmap;}

private:
    vtkRenderWindow* _renWin;
    cv::Mat_<cv::Vec3b> _colmap;  // Original colour map
    cv::Mat_<byte> _dcmap;   // CIE-L light map
    cv::Mat_<float> _dzmap;  // Depth map (raw z-buffer floats)
    cv::Mat_<byte> _ddmap;   // Converted depth map

    ImageGrabber( const ImageGrabber&) = delete;
    void operator=( const ImageGrabber&) = delete;
};  // end class

}   // end namespace

#endif
