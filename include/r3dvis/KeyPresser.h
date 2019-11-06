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

/**
 * Add key commands to a viewer
 * Richard Palmer
 * 2012
 */

#ifndef r3dvis_KEY_PRESSER_H
#define r3dvis_KEY_PRESSER_H

#include "Viewer.h"
using r3dvis::Viewer;
#include <string>
using std::string;
#include <iostream>
#include <boost/shared_ptr.hpp>


namespace r3dvis {

class r3dvis_EXPORT KeyPresser
{
public:
    typedef boost::shared_ptr<KeyPresser> Ptr;
    static Ptr create( Viewer::Ptr);

    // If true is returned, key press will not be passed to enclosing interactor for parsing.
    // Default implementation does the following. Entries marked (HANDLED) mean that the
    // key press is handled here and is not passed up to the interactor for parsing.
    // * z - change between perspective and orthogonal projection (HANDLED)
    // * c - print the current camera details to stdout
    virtual bool handleKeyPress( const string& keySymbol);

    virtual ~KeyPresser(){}

    virtual void printUsage( std::ostream& os) const;

protected:
    KeyPresser( Viewer::Ptr);
    r3dvis::Viewer::Ptr getViewer() const;

private:
    r3dvis::Viewer::Ptr m_pv;// point viewer to interact with
    bool m_orth;		// True if set to orthogonal view
}; // end class

}   // end namespace

#endif
