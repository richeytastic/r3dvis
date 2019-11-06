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

#ifndef r3dvis_LOOKUP_TABLE_H
#define r3dvis_LOOKUP_TABLE_H

#include "r3dvis_Export.h"
#include <opencv2/opencv.hpp>
#include <vtkLookupTable.h>
#include <vtkColor.h>

namespace r3dvis {

class r3dvis_EXPORT LookupTable
{
public:
    // Set simple colour range from minCol to maxCol over ncols.
    void setColours( const vtkColor3ub& minCol, const vtkColor3ub& maxCol, size_t ncols);

    // As above but use cv::Vec3b for the colours.
    void setColours( const cv::Vec3b& minCol, const cv::Vec3b& maxCol, size_t ncols);

    // Specify a colour midway between the start and end colours.
    // If two values for the number of colours are used, ncols0 and ncols1,
    // ncols0 specifies the number of colour values below the middle value
    // and ncols1 specifies the number of colour values above the middle value.
    void setColours( const vtkColor3ub& minCol,  // Colour at min value
                     const vtkColor3ub& midCol,  // Colour at midway value
                     const vtkColor3ub& maxCol,  // Colour at max value
                     size_t ncols0, size_t ncols1=0);

    // As above but use cv::Vec3b for the colours.
    void setColours( const cv::Vec3b& minCol,
                     const cv::Vec3b& midCol,
                     const cv::Vec3b& maxCol,
                     size_t ncols0, size_t ncols1=0);

    vtkLookupTable* vtk() { return _lut;}
    const vtkLookupTable* vtk() const { return _lut;}

private:
    vtkNew<vtkLookupTable> _lut;
};  // end class

}   // end namespace

#endif
