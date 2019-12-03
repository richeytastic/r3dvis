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

#include <LookupTable.h>
using r3dvis::LookupTable;


vtkLookupTable* LookupTable::vtk( const vtkRenderer *ren)
{
    if ( _luts.count(ren) == 0)
    {
        _luts[ren] = vtkSmartPointer<vtkLookupTable>::New();
        _luts[ren]->DeepCopy( _blut);
    }   // end if
    return _luts.at(ren);
}   // end vtk


void LookupTable::setColours( const cv::Vec3b& c0, const cv::Vec3b& c1, size_t nc)
{
    const vtkColor3ub v0( c0[0], c0[1], c0[2]);
    const vtkColor3ub v1( c1[0], c1[1], c1[2]);
    setColours( v0, v1, nc);
}   // end setColours


void LookupTable::setColours( const cv::Vec3b& c0, const cv::Vec3b& c1, const cv::Vec3b &c2, size_t nc)
{
    const vtkColor3ub v0( c0[0], c0[1], c0[2]);
    const vtkColor3ub v1( c1[0], c1[1], c1[2]);
    const vtkColor3ub v2( c2[0], c2[1], c2[2]);
    setColours( v0, v1, v2, nc);
}   // end setColours


void LookupTable::setColours( const vtkColor3ub& scol, const vtkColor3ub& fcol, size_t ncols)
{
    ncols = std::max<size_t>(2, ncols);
    _setNumTableValues( ncols);

    const float stepProp = 1.0f/(ncols-1);
    float cstep[3];
    cstep[0] = stepProp * (float(fcol[0]) - float(scol[0]));
    cstep[1] = stepProp * (float(fcol[1]) - float(scol[1]));
    cstep[2] = stepProp * (float(fcol[2]) - float(scol[2]));

    float rgb[3];
    for ( size_t i = 0; i < ncols; ++i)
    {
        rgb[0] = (scol[0] + i*cstep[0])/255.0f;
        rgb[1] = (scol[1] + i*cstep[1])/255.0f;
        rgb[2] = (scol[2] + i*cstep[2])/255.0f;
        _updateValue( int(i), rgb);
    }   // end for

    _buildTables();
}   // end setColours


void LookupTable::setColours( const vtkColor3ub& scol, const vtkColor3ub& mcol, const vtkColor3ub& fcol, size_t ncols)
{
    ncols = std::max<size_t>(2, ncols);
    _setNumTableValues( ncols);

    float rgb[3];
    float cstep[3];
    const float stepProp = 2.0f/(ncols-1);
    const size_t hcols = size_t(float(ncols)/2);

    // First half
    cstep[0] = stepProp * (float(mcol[0]) - float(scol[0]));
    cstep[1] = stepProp * (float(mcol[1]) - float(scol[1]));
    cstep[2] = stepProp * (float(mcol[2]) - float(scol[2]));
    for ( size_t i = 0; i < hcols; ++i)
    {
        rgb[0] = (scol[0] + i*cstep[0])/255.0f;
        rgb[1] = (scol[1] + i*cstep[1])/255.0f;
        rgb[2] = (scol[2] + i*cstep[2])/255.0f;
        _updateValue( int(i), rgb);
    }   // end for

    // Second half
    cstep[0] = stepProp * (float(fcol[0]) - float(mcol[0]));
    cstep[1] = stepProp * (float(fcol[1]) - float(mcol[1]));
    cstep[2] = stepProp * (float(fcol[2]) - float(mcol[2]));
    int j = 0;
    for ( size_t i = hcols; i < ncols; ++i, ++j)
    {
        rgb[0] = (mcol[0] + j*cstep[0])/255.0f;
        rgb[1] = (mcol[1] + j*cstep[1])/255.0f;
        rgb[2] = (mcol[2] + j*cstep[2])/255.0f;
        _updateValue( int(i), rgb);
    }   // end for

    _buildTables();
}   // end setColours


void LookupTable::_buildTables()
{
    _blut->Build();
    for ( auto p : _luts)
        p.second->Build();
}   // end _buildTables


void LookupTable::_updateValue( int i, const float rgb[3])
{
    _blut->SetTableValue( i, rgb[0], rgb[1], rgb[2], 1);
    for ( auto p : _luts)
        p.second->SetTableValue( i, rgb[0], rgb[1], rgb[2], 1);
}   // end _updateValue


void LookupTable::_setNumTableValues( size_t ncols)
{
    _blut->SetNumberOfTableValues( (int)ncols);
    for ( auto p : _luts)
        p.second->SetNumberOfTableValues( (int)ncols);
}   // end _setNumTableValues
