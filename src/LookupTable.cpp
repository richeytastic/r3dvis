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

#include <LookupTable.h>
#include <vtkColorSeries.h>
using r3dvis::LookupTable;


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


void LookupTable::setColours( const vtkColor3ub& sc, const vtkColor3ub& fc, size_t ncols)
{
    const int N = std::max<int>(2, int(ncols));
    _setNumTableValues( N);

    // Convert to HSV colour space
    //const r3d::Colour scol = r3d::Colour::rgb2hsv( r3d::Colour( (int)sc[0], (int)sc[1], (int)sc[2]));
    //const r3d::Colour fcol = r3d::Colour::rgb2hsv( r3d::Colour( (int)fc[0], (int)fc[1], (int)fc[2]));
    const r3d::Colour scol = r3d::Colour( (int)sc[0], (int)sc[1], (int)sc[2]);
    const r3d::Colour fcol = r3d::Colour( (int)fc[0], (int)fc[1], (int)fc[2]);

    const double stepProp = 1.0/N;
    double cstep[3];
    cstep[0] = stepProp * (fcol[0] - scol[0]);
    cstep[1] = stepProp * (fcol[1] - scol[1]);
    cstep[2] = stepProp * (fcol[2] - scol[2]);

    for ( int i = 0; i < N; ++i)
    {
        r3d::Colour col = scol;
        col[0] += i*cstep[0];
        col[1] += i*cstep[1];
        col[2] += i*cstep[2];
        //_updateValue( i, r3d::Colour::hsv2rgb( col));
        _updateValue( i, col);
    }   // end for

    _buildTable();
}   // end setColours


void LookupTable::setColours( const vtkColor3ub& sc, const vtkColor3ub& mc, const vtkColor3ub& fc, size_t ncols)
{
    const int N = std::max<int>(2, int(ncols));
    _setNumTableValues( N);

    // Convert to HSV colour space
    //const r3d::Colour scol = r3d::Colour::rgb2hsv( r3d::Colour( (int)sc[0], (int)sc[1], (int)sc[2]));
    //const r3d::Colour mcol = r3d::Colour::rgb2hsv( r3d::Colour( (int)mc[0], (int)mc[1], (int)mc[2]));
    //const r3d::Colour fcol = r3d::Colour::rgb2hsv( r3d::Colour( (int)fc[0], (int)fc[1], (int)fc[2]));
    const r3d::Colour scol = r3d::Colour( (int)sc[0], (int)sc[1], (int)sc[2]);
    const r3d::Colour mcol = r3d::Colour( (int)mc[0], (int)mc[1], (int)mc[2]);
    const r3d::Colour fcol = r3d::Colour( (int)fc[0], (int)fc[1], (int)fc[2]);

    int hcols = N / 2;    // Integer division
    const double stepProp = (N % 2 == 0) ?  2.0 / (N-1) : 2.0 / (N-2);

    double cstep[3];

    // First half
    cstep[0] = stepProp * (mcol[0] - scol[0]);
    cstep[1] = stepProp * (mcol[1] - scol[1]);
    cstep[2] = stepProp * (mcol[2] - scol[2]);
    for ( int i = 0; i < hcols; ++i)
    {
        r3d::Colour col = scol;
        col[0] += i*cstep[0];
        col[1] += i*cstep[1];
        col[2] += i*cstep[2];
        //_updateValue( i, r3d::Colour::hsv2rgb( col));
        _updateValue( i, col);
    }   // end for

    if ( N % 2 == 1)
    {
        //_updateValue( hcols, r3d::Colour::hsv2rgb(mcol));
        _updateValue( hcols, mcol);
    }   // end if
    else
        hcols--;

    // Second half
    cstep[0] = stepProp * (fcol[0] - mcol[0]);
    cstep[1] = stepProp * (fcol[1] - mcol[1]);
    cstep[2] = stepProp * (fcol[2] - mcol[2]);
    int j = 0;
    for ( int i = N-1; i > hcols; --i, ++j)
    {
        r3d::Colour col = fcol;
        col[0] -= j*cstep[0];
        col[1] -= j*cstep[1];
        col[2] -= j*cstep[2];
        //_updateValue( i, r3d::Colour::hsv2rgb( col));
        _updateValue( i, col);
    }   // end for

    _buildTable();
}   // end setColours


void LookupTable::_buildTable() { _lut->Build();}

void LookupTable::_updateValue( int i, const r3d::Colour &rgb) { _lut->SetTableValue( i, rgb[0], rgb[1], rgb[2], 1);}
void LookupTable::_setNumTableValues( int ncols) { _lut->SetNumberOfTableValues( ncols);}
