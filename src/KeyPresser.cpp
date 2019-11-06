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

#include <KeyPresser.h>
#include <iostream>
using r3dvis::KeyPresser;


KeyPresser::Ptr KeyPresser::create( Viewer::Ptr pv)
{
    KeyPresser *kp = new KeyPresser(pv);
    return KeyPresser::Ptr( kp);
}   // end create


KeyPresser::KeyPresser( Viewer::Ptr pv) : m_pv(pv), m_orth( false)
{}  // end ctor



bool KeyPresser::handleKeyPress( const string& ckey)
{
    bool handled = false;

    if ( ckey == "z")
    {
        m_orth = !m_orth;   // Toggle
        if ( !m_orth)
            m_pv->setPerspective( true);	// Persepective setting
        else
            m_pv->setPerspective( false);	// Orthogonnal projection
        m_pv->updateRender();
        handled = true;
    }   // end else if
    else if ( ckey == "c")
        handled = true;

    return handled;
}  // end handleKeyPress



r3dvis::Viewer::Ptr KeyPresser::getViewer() const
{
    return m_pv;
}   // end getViewer



void KeyPresser::printUsage( std::ostream& os) const
{
    os << "Toggle perspective/orthogonal projection: 'z'" << endl;
    os << "Print camera details: 'c'" << endl;
}   // end printUsage
