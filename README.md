# r3dvis
Visualise r3d::Mesh objects using VTK. Also provides some other high-level VTK specific wrapper functions.

## Prerequisites
- [rFeatures](../../../rFeatures)

- [VTK](http://www.vtk.org)

  Requires at least VTK version 8.1.1
  For integration with [QTools](../../../QTools), it's necessary to ensure VTK references
  the installed Qt5 distribution. When configuring VTK with CMake, ensure that `WITH_QT` is
  checked and that the correct Qt5 import libraries are being referenced.

  Some applications place themselves on the PATH (looking at you Anaconda) and this can cause
  older versions of the Qt shared libraries to be found. This is pretty much a Windows only problem,
  but if the required QT5 environment variable isn't set on Linux, this could be a problem there too.
  Also note that when upgrading Qt and later rebuilding VTK that VTK maintains its CMake variable
  cache correctly i.e. that it points to the correct Qt libraries and not some other
  Qt libraries found in the environment.
  
  In addition, ensure that separate debug and release builds are compiled. When integrating with
  Qt, QApplication will whinge if mixing build types (app will fail to start).
