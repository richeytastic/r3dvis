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

    // Convert to HSV colour space
    const r3d::Colour shsv = r3d::Colour::rgb2hsv( r3d::Colour( (int)scol[0], (int)scol[1], (int)scol[2]));
    const r3d::Colour fhsv = r3d::Colour::rgb2hsv( r3d::Colour( (int)fcol[0], (int)fcol[1], (int)fcol[3]));

    const double stepProp = 1.0/ncols;
    double cstep[3];
    cstep[0] = 0;//stepProp * (fhsv[0] - shsv[0]);
    cstep[1] = stepProp * (fhsv[1] - shsv[1]);
    cstep[2] = stepProp * (fhsv[2] - shsv[2]);

    for ( size_t i = 0; i < ncols; ++i)
    {
        r3d::Colour hsv = shsv;
        hsv[0] += i*cstep[0];
        hsv[1] += i*cstep[1];
        hsv[2] += i*cstep[2];
        const r3d::Colour rgb = r3d::Colour::hsv2rgb( hsv);
        _updateValue( int(i), rgb);
    }   // end for

    _buildTables();
}   // end setColours


void LookupTable::setColours( const vtkColor3ub& scol, const vtkColor3ub& mcol, const vtkColor3ub& fcol, size_t ncols)
{
    ncols = std::max<size_t>(2, ncols);
    _setNumTableValues( ncols);

    // Convert to HSV colour space
    const r3d::Colour shsv = r3d::Colour::rgb2hsv( r3d::Colour( (int)scol[0], (int)scol[1], (int)scol[2]));
    const r3d::Colour mhsv = r3d::Colour::rgb2hsv( r3d::Colour( (int)mcol[0], (int)mcol[1], (int)mcol[2]));
    const r3d::Colour fhsv = r3d::Colour::rgb2hsv( r3d::Colour( (int)fcol[0], (int)fcol[1], (int)fcol[2]));

    double cstep[3];
    const size_t hcols = double(ncols)/2;
    double stepProp = 1.0/hcols;

    // First half
    cstep[0] = 0;//stepProp * (mhsv[0] - shsv[0]);
    cstep[1] = stepProp * (mhsv[1] - shsv[1]);
    cstep[2] = stepProp * (mhsv[2] - shsv[2]);
    for ( size_t i = 0; i < hcols; ++i)
    {
        r3d::Colour hsv = shsv;
        hsv[0] += i*cstep[0];
        hsv[1] += i*cstep[1];
        hsv[2] += i*cstep[2];
        const r3d::Colour rgb = r3d::Colour::hsv2rgb( hsv);
        _updateValue( int(i), rgb);
    }   // end for

    stepProp = 1.0/(ncols - hcols);

    // Second half
    cstep[0] = 0;//stepProp * (fhsv[0] - mhsv[0]);
    cstep[1] = stepProp * (fhsv[1] - mhsv[1]);
    cstep[2] = stepProp * (fhsv[2] - mhsv[2]);
    int j = 0;
    for ( size_t i = hcols; i < ncols; ++i, ++j)
    {
        r3d::Colour hsv = mhsv;
        hsv[0] += j*cstep[0];
        hsv[1] += j*cstep[1];
        hsv[2] += j*cstep[2];
        const r3d::Colour rgb = r3d::Colour::hsv2rgb( hsv);
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


void LookupTable::_updateValue( int i, const r3d::Colour &rgb)
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
