#include "ICPTransform.h"

#include <vtkVersion.h>  
#include <vtkSmartPointer.h>  
#include <vtkTransform.h>  
#include <vtkVertexGlyphFilter.h>  
#include <vtkPoints.h>  
#include <vtkPolyData.h>  
#include <vtkCellArray.h>  
#include <vtkIterativeClosestPointTransform.h>  
#include <vtkTransformPolyDataFilter.h>  
#include <vtkLandmarkTransform.h>  
#include <vtkMath.h>  
#include <vtkMatrix4x4.h> 

#include <vtkXMLPolyDataWriter.h>  
#include <vtkPolyDataMapper.h>  
#include <vtkActor.h>  
#include <vtkRenderWindow.h>  
#include <vtkRenderer.h>  
#include <vtkRenderWindowInteractor.h>  
#include <vtkXMLPolyDataReader.h>  
#include <vtkProperty.h>  
#include <vtkPLYReader.h>  
#include <sstream>  
#include <iostream>  

namespace igstk 
{

ICPTransform::
  ICPTransform()
{
  m_ITKImage = NULL;
  m_Transform.SetToIdentity( 1e300 );
}


void ICPTransform::
  PrintSelf(std::ostream& os, itk::Indent indent)
{
  Superclass::PrintSelf(os, indent);
}
  
bool ICPTransform::Execute()  
{  
      vtkSmartPointer<vtkPolyData> sourceTmp =  
            vtkSmartPointer<vtkPolyData>::New();  
      vtkSmartPointer<vtkPolyData> targetTmp =  
            vtkSmartPointer<vtkPolyData>::New();  
              
      vtkSmartPointer<vtkPolyData> source =  
            vtkSmartPointer<vtkPolyData>::New();  
      vtkSmartPointer<vtkPolyData> target =  
            vtkSmartPointer<vtkPolyData>::New();  
   
       const char*  fileName1 = "D:/DATA/outputFileA.txt";
	   const char*  fileName2 = "D:/DATA/1.txt";
        // Get all data from the file 

        std::string strSource = fileName1;  
        std::string strTarget = fileName2;  
  
        std::ifstream fSource(strSource.c_str());  
        std::ifstream fTarget(strTarget.c_str());  
  
        std::string line;  
        vtkSmartPointer<vtkPoints> sourcePoints =  
                vtkSmartPointer<vtkPoints>::New();  
        vtkSmartPointer<vtkPoints> targetPoints =  
                vtkSmartPointer<vtkPoints>::New();  
  
        while(std::getline(fSource, line))  
        {  
            double x,y,z;  
            std::stringstream linestream;  
            linestream << line;  
            linestream >> x >> y >> z;  
            sourcePoints->InsertNextPoint(x, y, z);  
        }  
        sourceTmp->SetPoints(sourcePoints);  
        vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter1 =  
        vtkSmartPointer<vtkVertexGlyphFilter>::New();  
#if VTK_MAJOR_VERSION <= 5  
        vertexFilter1->SetInputConnection(sourceTmp->GetProducerPort());  
#else  
        vertexFilter1->SetInputData(sourceTmp);  
#endif  
        vertexFilter1->Update();  
        source->ShallowCopy(vertexFilter1->GetOutput());  
  
        while(std::getline(fTarget, line))  
        {  
            double x,y,z;  
            std::stringstream linestream;  
            linestream << line;  
            linestream >> x >> y >> z;  
            targetPoints->InsertNextPoint(x, y, z);  
        }  
        targetTmp->SetPoints(targetPoints);  
        vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter2 =  
        vtkSmartPointer<vtkVertexGlyphFilter>::New();  
#if VTK_MAJOR_VERSION <= 5  
        vertexFilter2->SetInputConnection(targetTmp->GetProducerPort());  
#else  
        vertexFilter2->SetInputData(targetTmp);  
#endif  
        vertexFilter2->Update();  
        target->ShallowCopy(vertexFilter2->GetOutput());  
           

  
    // Setup ICP transform  
    vtkSmartPointer<vtkIterativeClosestPointTransform> icp =  
            vtkSmartPointer<vtkIterativeClosestPointTransform>::New();  
    icp->SetSource(source);  
    icp->SetTarget(target);  
    icp->SetMeanDistanceModeToRMS();  
    icp->GetLandmarkTransform()->SetModeToRigidBody();  
	icp->CheckMeanDistanceOn();
    icp->StartByMatchingCentroidsOn();
    icp->SetMaximumNumberOfIterations( 50 );  
    icp->SetMaximumMeanDistance( 0.0000001 );  
    icp->SetMaximumNumberOfLandmarks( target->GetNumberOfPoints() );
    icp->GetLandmarkTransform()->SetModeToRigidBody();
    icp->Modified();
    icp->Update();  

	vtkIndent indent;
    icp->PrintSelf( std::cout, indent);

    igstk::Transform::VersorType  rotation;
    igstk::Transform::VectorType  translation;

	itk::Matrix< double > itkM;
    vtkMatrix4x4 * vtkM = icp->GetMatrix();

    for(unsigned int i=0; i<3; i++ )
      {
       for(unsigned int j=0; j<3; j++ )
         {
          itkM(i,j) = vtkM->GetElement(i,j);   
         }
       translation[i] = vtkM->GetElement(i,3);
      }
     rotation.Set( itkM );
     m_Transform.SetTranslationAndRotation( translation, rotation, 0.1, 1e300 );

  return true;
  }
} 