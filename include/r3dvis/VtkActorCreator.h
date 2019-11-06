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

#ifndef r3dvis_VTK_ActorCreator_H
#define r3dvis_VTK_ActorCreator_H

#include "r3dvis_Export.h"
#include <r3d/Mesh.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vector>
#include <list>

namespace r3dvis {

class r3dvis_EXPORT VtkActorCreator
{
public:
    // Return a texture mapped actor for the given mesh as long as only a single material is defined.
    // If no materials are defined, this function is equivalent to calling generateSurfaceActor.
    // The provided mesh must have all its vertex/face IDs in sequential order so they can
    // be treated as indices. On return, lighting is set to 100% ambient, 0% diffuse and 0% specular
    // so that the texture is lit properly. Returns null if more than one material defined on the object.
    // On return, the internal matrix of the actor will match Mesh::transformMatrix.
    static vtkSmartPointer<vtkActor> generateActor( const r3d::Mesh&);

    // Returns a non-textured actor for the given mesh. Mesh must have all its vertex/face IDs
    // stored in sequential order so they can be treated as indices.
    // On return, the internal matrix of the actor will match Mesh::transformMatrix.
    static vtkSmartPointer<vtkActor> generateSurfaceActor( const r3d::Mesh&);

    // Generate a simple points actor.
    // On return, the actor's internal matrix will match Mesh::transformMatrix.
    static vtkSmartPointer<vtkActor> generatePointsActor( const r3d::Mesh&);

    // Generate a simple points actor from the given subset of vertex IDs.
    // On return, the actor's internal matrix will match Mesh::transformMatrix.
    static vtkSmartPointer<vtkActor> generatePointsActor( const r3d::Mesh&, const IntSet& vidxsSubset);

    // Generate a points actor from raw vertices.
    static vtkSmartPointer<vtkActor> generatePointsActor( const std::vector<r3d::Vec3f>&);

    // Generate a single line where the given points are joined in sequence.
    // Set joinLoop to true if the first point should be joined to the last.
    static vtkSmartPointer<vtkActor> generateLineActor( const std::vector<r3d::Vec3f>&, bool joinLoop=false);
    static vtkSmartPointer<vtkActor> generateLineActor( const std::list<r3d::Vec3f>&, bool joinLoop=false);

    // Generate a single line where the given points are joined in sequence.
    // Set joinLoop to true if the first point should be joined to the last.
    // On return, the actor's internal matrix will match Mesh::transformMatrix.
    static vtkSmartPointer<vtkActor> generateLineActor( const r3d::Mesh&, const std::list<int>&, bool joinLoop=false);

    // Generate an actor that is a set of line segments where lps is a sequence of line segment
    // endpoints. (lps.size() must be even).
    static vtkSmartPointer<vtkActor> generateLinePairsActor( const std::vector<r3d::Vec3f>& lps);
};  // end class

}   // end namespace

#endif
