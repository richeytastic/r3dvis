/************************************************************************
 * Copyright (C) 2021 Richard Palmer
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
#include <r3d/Colour.h>
#include <opencv2/opencv.hpp>
#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>
#include <vtkColor.h>

namespace r3dvis {

class r3dvis_EXPORT LookupTable
{
public:
    LookupTable() {}

    // Range of ncols between and min and max colour.
    LookupTable( const r3d::Colour &minCol, const r3d::Colour &maxCol, size_t ncols);

    // Range of ncols from minCol through midCol to maxCol.
    LookupTable( const r3d::Colour &minCol,
                 const r3d::Colour &midCol,
                 const r3d::Colour &maxCol, size_t ncols);

    // Return the colour at the given index.
    const r3d::Colour &colour( int idx) const;

    // Set simple colour range from minCol to maxCol over ncols.
    void setColours( const vtkColor3ub& minCol, const vtkColor3ub& maxCol, size_t ncols);

    // As above but use cv::Vec3b for the colours.
    void setColours( const cv::Vec3b& minCol, const cv::Vec3b& maxCol, size_t ncols);

    // Specify a colour midway between the start and end colours.
    void setColours( const vtkColor3ub& minCol,  // Colour at min value
                     const vtkColor3ub& midCol,  // Colour at midway value
                     const vtkColor3ub& maxCol,  // Colour at max value
                     size_t ncols);

    // As above but use cv::Vec3b for the colours.
    void setColours( const cv::Vec3b& minCol,
                     const cv::Vec3b& midCol,
                     const cv::Vec3b& maxCol,
                     size_t ncols);

    vtkSmartPointer<vtkLookupTable> toVTK() const;

private:
    std::vector<r3d::Colour> _cols;
};  // end class

}   // end namespace

#endif
