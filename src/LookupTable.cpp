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
using r3dvis::LookupTable;


LookupTable::LookupTable( const rimg::Colour &c0, const rimg::Colour &c1, size_t nc)
{
    const vtkColor3ub v0( c0.ired(), c0.igreen(), c0.iblue());
    const vtkColor3ub v1( c1.ired(), c1.igreen(), c1.iblue());
    setColours( v0, v1, nc);
}   // end ctor


LookupTable::LookupTable( const rimg::Colour &c0, const rimg::Colour &c1, const rimg::Colour &c2, size_t nc)
{
    const vtkColor3ub v0( c0.ired(), c0.igreen(), c0.iblue());
    const vtkColor3ub v1( c1.ired(), c1.igreen(), c1.iblue());
    const vtkColor3ub v2( c2.ired(), c2.igreen(), c2.iblue());
    setColours( v0, v1, v2, nc);
}   // end ctor


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
    _cols.resize(N);

    const rimg::Colour scol = rimg::Colour( (int)sc[0], (int)sc[1], (int)sc[2]);
    const rimg::Colour fcol = rimg::Colour( (int)fc[0], (int)fc[1], (int)fc[2]);

    const double stepProp = 1.0/N;
    double cstep[3];
    cstep[0] = stepProp * (fcol[0] - scol[0]);
    cstep[1] = stepProp * (fcol[1] - scol[1]);
    cstep[2] = stepProp * (fcol[2] - scol[2]);

    for ( int i = 0; i < N; ++i)
        _cols[i] = rimg::Colour( scol[0] + i*cstep[0], scol[1] + i*cstep[1], scol[2] + i*cstep[2]);
}   // end setColours


void LookupTable::setColours( const vtkColor3ub& sc, const vtkColor3ub& mc, const vtkColor3ub& fc, size_t ncols)
{
    const int N = std::max<int>(2, int(ncols));
    _cols.resize(N);

    const rimg::Colour scol = rimg::Colour( (int)sc[0], (int)sc[1], (int)sc[2]);
    const rimg::Colour mcol = rimg::Colour( (int)mc[0], (int)mc[1], (int)mc[2]);
    const rimg::Colour fcol = rimg::Colour( (int)fc[0], (int)fc[1], (int)fc[2]);

    int hcols = N / 2;    // Integer division
    const double stepProp = (N % 2 == 0) ?  2.0 / (N-1) : 2.0 / (N-2);

    double cstep[3];

    // First half
    cstep[0] = stepProp * (mcol[0] - scol[0]);
    cstep[1] = stepProp * (mcol[1] - scol[1]);
    cstep[2] = stepProp * (mcol[2] - scol[2]);
    for ( int i = 0; i < hcols; ++i)
        _cols[i] = rimg::Colour( scol[0] + i*cstep[0], scol[1] + i*cstep[1], scol[2] + i*cstep[2]);

    if ( N % 2 == 1)
        _cols[hcols] = mcol;
    else
        hcols--;

    // Second half
    cstep[0] = stepProp * (fcol[0] - mcol[0]);
    cstep[1] = stepProp * (fcol[1] - mcol[1]);
    cstep[2] = stepProp * (fcol[2] - mcol[2]);
    int j = 0;
    for ( int i = N-1; i > hcols; --i, ++j)
        _cols[i] = rimg::Colour( fcol[0] - j*cstep[0], fcol[1] - j*cstep[1], fcol[2] - j*cstep[2]);
}   // end setColours


vtkSmartPointer<vtkLookupTable> LookupTable::toVTK() const
{
    const int ncols = int(_cols.size());
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues( ncols);
    for ( int i = 0; i < ncols; ++i)
    {
        const rimg::Colour &rgb = _cols.at(i);
        lut->SetTableValue( i, rgb[0], rgb[1], rgb[2], 1);
    }   // end for
    lut->Build();
    return lut;
}   // end toVTK
