/************************************************************************
 * Copyright (C) 2020 Richard Palmer
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

#include <VtkTools.h>
#include <vtkOctreePointLocator.h>
#include <vtkFeatureEdges.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataNormals.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageShiftScale.h>
#include <vtkImageExport.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkMatrixToLinearTransform.h>
#include <cassert>
using r3dvis::Vec3f;
using r3dvis::byte;


void r3dvis::setColoursLookupTable( vtkSmartPointer<vtkLookupTable> lut,
                            int numColours, const vtkColor3ub& scol, const vtkColor3ub& fcol)
{
    lut->SetNumberOfTableValues( numColours);
    lut->Build();

    float cstep[3];
    cstep[0] = (float(fcol[0]) - float(scol[0]))/(numColours-1);
    cstep[1] = (float(fcol[1]) - float(scol[1]))/(numColours-1);
    cstep[2] = (float(fcol[2]) - float(scol[2]))/(numColours-1);
    for ( int i = 0; i < numColours; ++i)
    {
        float rgb[3];
        rgb[0] = (scol[0] + i*cstep[0])/255.0;
        rgb[1] = (scol[1] + i*cstep[1])/255.0;
        rgb[2] = (scol[2] + i*cstep[2])/255.0;
        lut->SetTableValue( i, rgb[0], rgb[1], rgb[2], 1);
    }   // end for
}   // end createColoursLookupTable


r3d::Mesh::Ptr r3dvis::makeObject( const vtkActor* actor)
{
    r3d::Mesh::Ptr model = r3d::Mesh::create();
    vtkPolyData* pdata = getPolyData(actor);
    vtkPoints* points = pdata->GetPoints();
    vtkCellArray* faces = pdata->GetPolys();

    std::unordered_map<int,int> vmap; // VTK to Mesh vertex IDs
    double p[3];
    const int npoints = points->GetNumberOfPoints();
    for ( int i = 0; i < npoints; ++i)
    {
        points->GetPoint( i, p);
        vmap[i] = model->addVertex( p[0], p[1], p[2]);
    }   // end for

    faces->InitTraversal();
    vtkNew<vtkIdList> vidxs;
    vidxs->SetNumberOfIds(3);
    while ( faces->GetNextCell( vidxs) > 0) // TODO not a thread-safe call - upgrade to using NewIterator() instead (VTK 9.0.0)
        model->addFace( vmap[vidxs->GetId(0)], vmap[vidxs->GetId(1)], vmap[vidxs->GetId(2)]);
    return model;
}   // end makeObject


vtkPolyData* r3dvis::getPolyData( const vtkActor* actor)
{
    if ( !actor)
        return nullptr;
    vtkPolyDataMapper* mapper = (vtkPolyDataMapper*)const_cast<vtkActor*>(actor)->GetMapper();
    return mapper->GetInput();
}   // end getPolyData


void r3dvis::fixTransform( vtkActor* actor, const vtkMatrix4x4* m)
{
    if ( !m)
        m = actor->GetMatrix();
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(const_cast<vtkMatrix4x4*>(m));

    vtkSmartPointer<vtkTransformPolyDataFilter> tfilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    vtkPolyData* pdata = getPolyData(actor);
    tfilter->SetInputData( pdata);
    tfilter->SetTransform( transform);
    tfilter->Update();
    pdata->SetPoints( tfilter->GetOutput()->GetPoints());
    actor->GetMatrix()->Identity();
}   // end fixTransform


vtkSmartPointer<vtkImageImport> r3dvis::makeImageImporter( const cv::Mat img)
{
    const int rows = img.rows;
    const int cols = img.cols;
    const int nchannels = img.channels();

    vtkSmartPointer<vtkImageImport> imageImporter = vtkSmartPointer<vtkImageImport>::New();
    imageImporter->SetDataScalarTypeToUnsignedChar();
    imageImporter->CopyImportVoidPointer( img.data, rows * cols * nchannels * sizeof(unsigned char));
    imageImporter->SetNumberOfScalarComponents( nchannels);
    imageImporter->SetWholeExtent( 0, cols-1, 0, rows-1, 0, 0);
    imageImporter->SetDataExtentToWholeExtent();
    imageImporter->Update();
    return imageImporter;
}   // end makeImageImporter


// Converts the given image into a format suitable as a texture.
// Flips the image vertically and transposes the first and second
// bytes if image is triple channel.
vtkSmartPointer<vtkTexture> r3dvis::convertToTexture( const cv::Mat& image, bool XFLIP)
{
    vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();

    // Only accepts either cv::Mat_<cv::Vec3b> or cv::Mat_<byte> images for now
    const int numChannels = image.channels();
    if ( (image.depth() != CV_8U) || !image.isContinuous() || ( numChannels != 1 && numChannels != 3))
    {
        std::cerr << "[WARNING] r3dvis::convertToTexture(): Unable to create texture from image!" << std::endl;
        return vtkSmartPointer<vtkTexture>();
    }   // end if

    // Flip the input image vertically.
    cv::Mat img;
    if ( XFLIP)
        cv::flip( image, img, 0/*flip about x axis*/);
    else
        img = image;

    if ( numChannels == 3)  // Swap byte order from BGR to RGB
    {
        std::vector<cv::Mat> channels;
        cv::split( img, channels);
        cv::Mat swappedChannels[3] = {channels[2], channels[1], channels[0]};
        cv::Mat nimg;
        cv::merge( swappedChannels, 3, nimg);
        img = nimg;
    }   // end if

    // Make the texture from the image
    vtkSmartPointer<vtkImageImport> imageImporter = r3dvis::makeImageImporter( img);
    texture->SetInputConnection( imageImporter->GetOutputPort());
    texture->Update();
    return texture;
}   // end convertToTexture


vtkSmartPointer<vtkTexture> r3dvis::loadTexture( const std::string& fname, bool XFLIP)
{
    cv::Mat m = cv::imread( fname, true);
    if ( m.empty())
        return vtkSmartPointer<vtkTexture>();
    return convertToTexture( m, XFLIP);
}   // end loadTexture


void r3dvis::extractBoundaryVertices( const vtkSmartPointer<vtkPolyData>& pdata, std::vector<int>& bvids)
{
    vtkSmartPointer<vtkFeatureEdges> vfe = vtkSmartPointer<vtkFeatureEdges>::New();
    vfe->SetInputData( pdata);
    vfe->BoundaryEdgesOn(); // Extract edges used by exactly one polygon
    vfe->FeatureEdgesOff();
    vfe->ManifoldEdgesOff();
    vfe->NonManifoldEdgesOff();
    vfe->Update();
    vtkPolyData* odata = vfe->GetOutput();

    vtkOctreePointLocator* plocator = vtkOctreePointLocator::New();
    plocator->SetDataSet( pdata);

    double x[3];
    const int npts = odata->GetNumberOfPoints();
    for ( int i = 0; i < npts; ++i)
    {
        odata->GetPoint( i, x);
        bvids.push_back( plocator->FindClosestPoint(x));
    }   // end for

    plocator->Delete();
}   // end extractBoundaryVertices


vtkSmartPointer<vtkPolyData> r3dvis::generateNormals( vtkSmartPointer<vtkPolyData> pdata)
{
    vtkSmartPointer<vtkPolyDataNormals> normalsGenerator = vtkPolyDataNormals::New();
    normalsGenerator->SetInputData( pdata);
    normalsGenerator->ComputePointNormalsOn();
    normalsGenerator->ComputeCellNormalsOff();
    //normalsGenerator->NonManifoldTraversalOn();
    //normalsGenerator->ConsistencyOff();
    normalsGenerator->Update();
    return normalsGenerator->GetOutput();
}   // end generateNormals


cv::Mat_<cv::Vec3b> r3dvis::extractBGR( vtkRenderWindow *rwin)
{
    vtkNew<vtkWindowToImageFilter> filter;
    filter->SetInput(rwin);
    filter->SetInputBufferTypeToRGB();
    filter->Modified();
    filter->Update();
    vtkImageData *vtkimg = filter->GetOutput();
    int dims[3];    // Width, height, depth
    vtkimg->GetDimensions( dims);
    const int cols = dims[0];
    const int rows = dims[1];
    assert( vtkimg->GetNumberOfScalarComponents() == 3);    // RGB

    cv::Mat_<cv::Vec3b> img( rows, cols);
    for ( int i = 0; i < rows; ++i)
    {
        cv::Vec3b *imgRow = img.ptr<cv::Vec3b>(rows-i-1); // Flip vertically for OpenCV
        for ( int j = 0; j < cols; ++j)
        {
            const unsigned char *ipxl = static_cast<unsigned char*>(vtkimg->GetScalarPointer(j,i,0));
            cv::Vec3b &pxl = imgRow[j];
            // Reverse the red/blue channel order
            pxl[0] = ipxl[2];
            pxl[1] = ipxl[1];
            pxl[2] = ipxl[0];
        }   // end for
    }   // end for

    return img;
}   // end extractBGR


cv::Mat_<float> r3dvis::extractZ( vtkRenderWindow *rwin)
{
    vtkNew<vtkWindowToImageFilter> filter;
    filter->SetInput(rwin);
    filter->SetInputBufferTypeToZBuffer();
    filter->Modified();
    filter->Update();
    vtkImageData *vtkimg = filter->GetOutput();
    int dims[3];    // Width, height, depth
    vtkimg->GetDimensions( dims);
    const int cols = dims[0];
    const int rows = dims[1];
    assert( vtkimg->GetNumberOfScalarComponents() == 1);

    cv::Mat_<float> img( rows, cols);
    for ( int i = 0; i < rows; ++i)
    {
        float *imgRow = img.ptr<float>(rows-i-1); // Flip vertically for OpenCV
        for ( int j = 0; j < cols; ++j)
            imgRow[j] = *static_cast<float*>(vtkimg->GetScalarPointer(j,i,0));
    }   // end for

    return img;
}   // end extractZ


void r3dvis::printCameraDetails( vtkCamera* cam, std::ostream &os)
{
    using std::endl;
    os << "*********************************************" << endl;
	double *camPos = cam->GetPosition();
	os << "Camera Position (x,y,z):          " << camPos[0] << ", " << camPos[1] << ", " << camPos[2] << endl;
	double *camFoc = cam->GetFocalPoint();
	os << "Camera Focus (x,y,z):             " << camFoc[0] << ", " << camFoc[1] << ", " << camFoc[2] << endl;
    double focDist = cam->GetDistance();  // Distance of focal point to camera position
    os << "Focal point distance (from cam):  " << focDist << endl;
    double *normal = cam->GetViewPlaneNormal();
    os << "Camera View Plane Normal (x,y,z): " << normal[0] << ", " << normal[1] << ", " << normal[2] << endl;
    double *projDir = cam->GetDirectionOfProjection();
    os << "Direction of projection:          " << projDir[0] << ", " << projDir[1] << ", " << projDir[2] << endl;
    double *camUp = cam->GetViewUp();
    os << "Camera View Up Vector (x,y,z):    " << camUp[0] << ", " << camUp[1] << ", " << camUp[2] << endl;
    double *camOr = cam->GetOrientationWXYZ();
    os << "Camera Orientation (w,x,y,z):     " << camOr[0] << ", " << camOr[1] << ", " << camOr[2] << ", " << camOr[3] << endl;

	if ( cam->GetParallelProjection())
	{
		os << "Cam Projection:                   ORTHOGONAL" << endl;
		double ps = cam->GetParallelScale();
		os << "Cam Parallel Scale:           " << ps << " metres (viewport height)" << endl;
	}	// end if
	else
	{
		os << "Cam Projection:                   PERSPECTIVE" << endl;
		double va = cam->GetViewAngle();
		os << "Cam View Angle:                   " << va << " degrees" << endl;
	}	// end else
    os << "*********************************************" << endl;
}	// end printCameraDetails



r3dvis::Light::Light()
    : light( vtkSmartPointer<vtkLight>::New())
{
    light->SetLightTypeToHeadlight();
    light->SetColor(1,1,1);
    light->SetAmbientColor(1,1,1);
    light->SetIntensity(1);
}   // end ctor


r3dvis::Light::Light( const Vec3f& pos, const Vec3f& foc)
    : light( vtkSmartPointer<vtkLight>::New())
{
    light->SetLightTypeToSceneLight();
    light->SetColor(1,1,1);
    light->SetAmbientColor(1,1,1);
    light->SetIntensity(1);
    light->SetPosition( pos[0], pos[1], pos[2]);
    light->SetFocalPoint( foc[0], foc[1], foc[2]);
}   // end ctor


void r3dvis::Light::toCamera( bool enable)
{
    if ( enable)
        light->SetLightTypeToCameraLight();
    else
        light->SetLightTypeToSceneLight();
}   // end toCamera


void r3dvis::resetLights( vtkRenderer* ren, const std::vector<Light>& lights)
{
    ren->RemoveAllLights();
    const int n = (int)lights.size();
    for ( int i = 0; i < n; ++i)
        ren->AddLight( lights[i].light);
}   // end resetLights


void r3dvis::createBoxLights( float d, std::vector<Light>& lights, bool toCamera)
{
    const Vec3f focus(0,0,0);
    lights.resize(6);
    lights[0] = r3dvis::Light( Vec3f( 0, 0, d), focus);  // From +Z
    lights[1] = r3dvis::Light( Vec3f( 0, 0,-d), focus);  // From -Z
    lights[2] = r3dvis::Light( Vec3f( 0, d, 0), focus);  // From +Y
    lights[3] = r3dvis::Light( Vec3f( 0,-d, 0), focus);  // From -Y
    lights[4] = r3dvis::Light( Vec3f( d, 0, 0), focus);  // From +X
    lights[5] = r3dvis::Light( Vec3f(-d, 0, 0), focus);  // From -X
    lights[0].toCamera(toCamera);
    lights[1].toCamera(toCamera);
    lights[2].toCamera(toCamera);
    lights[3].toCamera(toCamera);
    lights[4].toCamera(toCamera);
    lights[5].toCamera(toCamera);
}   // end createBoxLights


vtkSmartPointer<vtkMatrix4x4> r3dvis::toVTK( const r3d::Mat4f& m)
{
    vtkSmartPointer<vtkMatrix4x4> vm = vtkSmartPointer<vtkMatrix4x4>::New();

    vm->SetElement(0,0,m(0,0));
    vm->SetElement(0,1,m(0,1));
    vm->SetElement(0,2,m(0,2));
    vm->SetElement(0,3,m(0,3));

    vm->SetElement(1,0,m(1,0));
    vm->SetElement(1,1,m(1,1));
    vm->SetElement(1,2,m(1,2));
    vm->SetElement(1,3,m(1,3));

    vm->SetElement(2,0,m(2,0));
    vm->SetElement(2,1,m(2,1));
    vm->SetElement(2,2,m(2,2));
    vm->SetElement(2,3,m(2,3));

    vm->SetElement(3,0,m(3,0));
    vm->SetElement(3,1,m(3,1));
    vm->SetElement(3,2,m(3,2));
    vm->SetElement(3,3,m(3,3));

    return vm;
}   // end toVTK


r3d::Mat4f r3dvis::toEigen( const vtkMatrix4x4 *m)
{
    if ( !m)
        return r3d::Mat4f::Identity();

    r3d::Mat4f cm;

    cm(0,0) = m->GetElement(0,0);
    cm(0,1) = m->GetElement(0,1);
    cm(0,2) = m->GetElement(0,2);
    cm(0,3) = m->GetElement(0,3);

    cm(1,0) = m->GetElement(1,0);
    cm(1,1) = m->GetElement(1,1);
    cm(1,2) = m->GetElement(1,2);
    cm(1,3) = m->GetElement(1,3);

    cm(2,0) = m->GetElement(2,0);
    cm(2,1) = m->GetElement(2,1);
    cm(2,2) = m->GetElement(2,2);
    cm(2,3) = m->GetElement(2,3);

    cm(3,0) = m->GetElement(3,0);
    cm(3,1) = m->GetElement(3,1);
    cm(3,2) = m->GetElement(3,2);
    cm(3,3) = m->GetElement(3,3);

    return cm;
}   // end toEigen


void r3dvis::print( std::ostream& os, const vtkMatrix4x4* m)
{
    os << toEigen(m) << std::endl;
}   // end print


cv::Mat r3dvis::getLightness( const cv::Mat_<cv::Vec3b>& img, double scale, int imgType)
{
    cv::Mat_<cv::Vec3f> f3img;
    img.convertTo( f3img, CV_32F, 1.0/255);  // Make correct range of values across colour channels
    cv::Mat labImg;
    //cv::cvtColor( f3img, labImg, CV_BGR2Lab);
    cv::cvtColor( f3img, labImg, cv::COLOR_BGR2Lab);
    std::vector<cv::Mat_<float> > labChannels;
    cv::split( labImg, labChannels);
    const cv::Mat_<float> lightMap = labChannels[0]; // 0 to 100
    cv::Mat outMap;
    lightMap.convertTo( outMap, imgType, scale/100);
    return outMap;
}   // end getLightness


cv::Mat_<byte> r3dvis::contrastStretch( const cv::Mat& m, const cv::Mat_<byte> mask)
{
    double mn, mx;
    cv::minMaxLoc( m, &mn, &mx, 0, 0, mask);
    cv::Mat_<byte> outm( m.size());
    const cv::Mat mtemp = m - mn;
    mtemp.convertTo( outm, CV_8U, 255./(mx-mn));
    return outm;
}   // end contrastStretch


namespace {

vtkSmartPointer<vtkFloatArray> makeTexturedNormals( const r3d::Curvature &cv)
{
    vtkSmartPointer<vtkFloatArray> narr = vtkSmartPointer<vtkFloatArray>::New();
    narr->SetNumberOfComponents( 3);

    const r3d::Mesh& mesh = cv.mesh();
    const int nfaces = int(mesh.numFaces());
    const r3d::MatX3f& nrms = cv.vertexNormals();
    narr->SetNumberOfTuples( 3*nfaces);

    int i = 0;
    for ( int fid = 0; fid < nfaces; ++fid)
    {
        const int* fvidxs = mesh.fvidxs(fid);
        narr->SetTuple3( i++, nrms(fvidxs[0],0), nrms(fvidxs[0],1), nrms(fvidxs[0],2));
        narr->SetTuple3( i++, nrms(fvidxs[1],0), nrms(fvidxs[1],1), nrms(fvidxs[1],2));
        narr->SetTuple3( i++, nrms(fvidxs[2],0), nrms(fvidxs[2],1), nrms(fvidxs[2],2));
    }   // end for

    return narr;
}   // end makeTexturedNormals


vtkSmartPointer<vtkFloatArray> makeNonTexturedNormals( const r3d::Curvature &cv)
{
    vtkSmartPointer<vtkFloatArray> narr = vtkSmartPointer<vtkFloatArray>::New();
    narr->SetNumberOfComponents( 3);

    const r3d::MatX3f& nrms = cv.vertexNormals();
    const int nvtxs = nrms.rows();
    narr->SetNumberOfTuples( nvtxs);

    for ( int vidx = 0; vidx < nvtxs; ++vidx)
        narr->SetTuple3( vidx, nrms(vidx,0), nrms(vidx,1), nrms(vidx,2));

    return narr;
}   // end makeNonTexturedNormals

}   // end namespace


vtkSmartPointer<vtkFloatArray> r3dvis::makeNormals( const r3d::Curvature &cv)
{
    vtkSmartPointer<vtkFloatArray> nrms;
    if ( cv.mesh().numMats() > 0)
        nrms = makeTexturedNormals( cv);
    else
        nrms = makeNonTexturedNormals( cv);
    return nrms;
}   // end makeNormals
