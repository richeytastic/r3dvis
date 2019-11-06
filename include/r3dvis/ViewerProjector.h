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

#ifndef r3dvis_VIEWER_PROJECTOR_H
#define r3dvis_VIEWER_PROJECTOR_H

#include "Viewer.h"

/*
COLOUR MAPS AVAILABLE IN OPENCV:
cv::COLORMAP_AUTUMN = 0
cv::COLORMAP_BONE = 1
cv::COLORMAP_JET = 2
cv::COLORMAP_WINTER = 3
cv::COLORMAP_RAINBOW = 4
cv::COLORMAP_OCEAN = 5
cv::COLORMAP_SUMMER = 6
cv::COLORMAP_SPRING = 7
cv::COLORMAP_COOL = 8
cv::COLORMAP_HSV = 9
cv::COLORMAP_PINK = 10
cv::COLORMAP_HOT = 11
*/


namespace r3dvis {

class r3dvis_EXPORT ViewerProjector
{
public:
    explicit ViewerProjector( const r3dvis::Viewer::Ptr viewer);

    // Create and return an image of the current window (wraps viewer->extractImage()).
    cv::Mat_<cv::Vec3b> extractImage() const;

    // Makes a scaled and coloured range map.
    // depthProp: Proportion of the front of the Z-buffer to use.
    // colourMap: If left at -1, a CV_8UC1 is returned with a grey scale mapping of
    // the depth values. Otherwise, if one of the OpenCV colour maps are used (see above)
    // a CV_8UC3 map is returned.
    cv::Mat makeRangeMap( float depthProp, int colourMap=-1) const;

private:
    const r3dvis::Viewer::Ptr _viewer;

};  // end class

}   // end namespace

#endif
