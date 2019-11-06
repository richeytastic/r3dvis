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

#ifndef r3dvis_SNAPSHOT_KEY_PRESSER_H
#define r3dvis_SNAPSHOT_KEY_PRESSER_H

#include "Viewer.h"
#include "KeyPresser.h"

namespace r3dvis {

using Vec3f = Eigen::Vector3f;

class r3dvis_EXPORT SnapshotKeyPresser : public r3dvis::KeyPresser
{
public:
    typedef boost::shared_ptr<SnapshotKeyPresser> Ptr;

    static r3dvis::SnapshotKeyPresser::Ptr create( r3dvis::Viewer::Ptr,
            const Vec3f& camPos, const Vec3f& focalDir, const Vec3f& vUp, float fov);

    virtual bool handleKeyPress( const string& keySym);
    virtual ~SnapshotKeyPresser() {}

    virtual void printUsage( std::ostream&) const;

    void setImage( const cv::Mat&);

protected:
    SnapshotKeyPresser( r3dvis::Viewer::Ptr v,
            const Vec3f& camPos, const Vec3f& focalDir, const Vec3f& vUp, float fov);

private:
    const Vec3f _camPos;    // Initial camera position
    const Vec3f _focalDir;  // Direction of camera focus (added to _camPos to give focal point)
    const Vec3f _viewUp;    // Initial camera view up
    const float _fov;   // Field of view
    mutable int _snapCount; // Snapshot counter for images saved from the rendering buffer
    cv::Mat _image; // Image to display
    bool _showingImage; // True if showing image

    void resetCamera(); // Reset to front
    void addCameraYaw( float yaw);
    void addCameraPitch( float pitch);
    void addCameraRoll( float roll);
    void saveSnapshot();
};  // end class

}   // end namespace

#endif
