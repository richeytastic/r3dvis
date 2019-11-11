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

#include <ScalarLegend.h>
#include <vtkTextProperty.h>
#include <vtkScalarBarActor.h>
#include <iostream>
#include <sstream>
using r3dvis::ScalarLegend;


ScalarLegend::ScalarLegend( vtkRenderWindowInteractor* rwint)
{
    _widget = vtkSmartPointer<vtkScalarBarWidget>::New();
    vtkScalarBarActor* legend = _widget->GetScalarBarActor();

    legend->GetLabelTextProperty()->SetFontFamilyToCourier();
    legend->GetLabelTextProperty()->SetItalic(false);
    legend->GetLabelTextProperty()->SetFontSize(14);
    legend->SetNumberOfLabels( 11);
    legend->SetMaximumWidthInPixels( 80);
    legend->SetUnconstrainedFontSize(true);
    legend->DrawTickLabelsOn();
    legend->SetPosition(0.61, 0.3);
    legend->SetHeight(0.65);

    legend->GetTitleTextProperty()->SetFontFamilyToCourier();
    legend->GetTitleTextProperty()->SetFontSize(16);
    legend->GetTitleTextProperty()->SetBold(false);
    legend->GetTitleTextProperty()->SetItalic(false);

    _widget->SetInteractor(rwint);
    _widget->SetRepositionable(false);
}   // end dtor


vtkTextProperty* ScalarLegend::titleProperty() { return _widget->GetScalarBarActor()->GetTitleTextProperty();}
vtkTextProperty* ScalarLegend::labelProperty() { return _widget->GetScalarBarActor()->GetLabelTextProperty();}


void ScalarLegend::setTitle( const std::string& title)
{
    vtkScalarBarActor* legend = _widget->GetScalarBarActor();
    legend->SetTitle( title.c_str());
}   // end setTitle


void ScalarLegend::setLookupTable( vtkLookupTable* lut)
{
    const double minv = lut->GetTableRange()[0];
    const double maxv = lut->GetTableRange()[1];

    const int maxWidth = 4;
    int ndecimals = std::max<int>(0, maxWidth - (int)(log(maxv - minv) + 1));
    ndecimals = std::min<int>(ndecimals, maxWidth-1);

    std::ostringstream oss;
    oss << "% " << maxWidth << "." << ndecimals << "f";
    vtkScalarBarActor* legend = _widget->GetScalarBarActor();
    legend->SetLabelFormat( oss.str().c_str());
    legend->SetLookupTable( lut);
}   // end setLookupTable


bool ScalarLegend::isVisible() const { return _widget->GetEnabled() > 0;}


void ScalarLegend::setVisible( bool visible)
{
    if ( _widget->GetScalarBarActor()->GetLookupTable() != nullptr)
        _widget->SetEnabled(visible);
}   // end setVisible
