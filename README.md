# r3dvis

Visualise r3d::Mesh objects using VTK. Also provides some other high-level VTK specific wrapper functions.

Download [libbuild](https://github.com/richeytastic/libbuild) for easy build and install of this library.

## Prerequisites
- [r3d](../../../r3d)

- [VTK](http://www.vtk.org)

  Requires VTK version 9.1.0+
  For integration with [QTools](../../../QTools), it's necessary to ensure VTK references
  the installed Qt5 distribution. When configuring VTK with CMake, ensure that `WITH_QT` is
  checked and that the correct Qt5 import libraries are being referenced. Importantly, if
  upgrading to a newer version of Qt, ensure that the Qt components of VTK are also rebuilt.
