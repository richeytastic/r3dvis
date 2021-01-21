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

#include <ScalarLegend.h>
#include <vtkTexturedActor2D.h>
#include <vtkTexture.h>
#include <iostream>
#include <sstream>
using r3dvis::ScalarLegend;

ScalarLegend::ScalarLegend()
{
    _legend->GetLabelTextProperty()->SetFontFamilyToCourier();
    _legend->GetLabelTextProperty()->SetItalic(false);
    _legend->GetLabelTextProperty()->SetFontSize(13);
    _legend->SetNumberOfLabels( 11);
    _legend->SetUnconstrainedFontSize(true);
    _legend->DrawTickLabelsOn();

    _legend->GetTitleTextProperty()->SetFontFamilyToCourier();
    _legend->GetTitleTextProperty()->SetFontSize(16);
    _legend->GetTitleTextProperty()->SetBold(false);
    _legend->GetTitleTextProperty()->SetItalic(false);
}   // end ctor


vtkTextProperty* ScalarLegend::titleProperty() { return _legend->GetTitleTextProperty();}
vtkTextProperty* ScalarLegend::labelProperty() { return _legend->GetLabelTextProperty();}

void ScalarLegend::setTitle( const std::string& title) { _legend->SetTitle( title.c_str());}
void ScalarLegend::setNumLabels( int n) { _legend->SetNumberOfLabels( n);}
void ScalarLegend::setPosition( double x, double y) { _legend->SetPosition(x,y);}
void ScalarLegend::setHeight( double v) { _legend->SetHeight(v);}

void ScalarLegend::setLookupTable( const vtkLookupTable* lut)
{
    //const double minv = lut->GetTableRange()[0];
    //const double maxv = lut->GetTableRange()[1];

    const int maxWidth = 4;
    int ndecimals = 2;//std::max<int>(0, maxWidth - (int)(log(maxv - minv) + 1));
    //ndecimals = std::min<int>(ndecimals, maxWidth-1);

    std::ostringstream oss;
    oss << "% " << maxWidth << "." << ndecimals << "f";
    _legend->SetLabelFormat( oss.str().c_str());
    _legend->SetLookupTable( const_cast<vtkLookupTable*>(lut));
}   // end setLookupTable


/*
void ScalarLegend::printDebug() const
{
    vtkTexturedActor2D *ta = _legend->GetTextureActor();
    vtkTexture *tx = ta->GetTexture();
}   // end printDebug
*/
