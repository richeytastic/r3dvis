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

#include "ViewerProjector.h"
#include <algorithm>
#include <iostream>
using r3dvis::ViewerProjector;


ViewerProjector::ViewerProjector( const r3dvis::Viewer::Ptr viewer) : _viewer(viewer) {}


cv::Mat ViewerProjector::makeRangeMap( float depthProp, int colourMap)
{
    cv::Mat_<float> rngMap = _viewer->extractZ();

    double mn, mx;
    cv::minMaxLoc( rngMap, &mn, &mx);
    rngMap -= float(mn);   // Make nearest point 0
    
    cv::Mat img;
    rngMap.convertTo( img, CV_8U, -255./((mx-mn)*depthProp), 255);

    if ( colourMap < 0)
        return img;

    cv::Mat outMap;
    cv::applyColorMap( img, outMap, colourMap);
    return outMap;
}   // end makeRangeMap
