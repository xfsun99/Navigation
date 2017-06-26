/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    $RCSfile: igstkImageSliceRepresentationPlus.txx,v $
  Language:  C++
  Date:      $Date: 2011-01-03 23:37:28 $
  Version:   $Revision: 1.7 $

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __igstkImageSliceRepresentationPlus_txx
#define __igstkImageSliceRepresentationPlus_txx


#include "igstkImageSliceRepresentationPlus.h"

#include "igstkEvents.h"

#include <vtkImageData.h>
#include <vtkTransform.h>
#include <vtkPlaneSource.h>
#include <vtkPlane.h>
#include <vtkCamera.h>
#include <vtkMath.h>
#include <vtkImageMapToColors.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageProperty.h>
#include <vtkImageSlice.h>

namespace igstk
{

/** Constructor */
template < class TBGImageSO,  class TFGImageSO  >
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::ImageSliceRepresentationPlus():m_StateMachine(this)
{
  // We create the image spatial object
  m_ImageSpatialObject = NULL;
  m_FGImageSO = NULL;
  m_BGImageData  = NULL;
  m_FGImageData  = NULL;
  this->RequestSetSpatialObject( m_ImageSpatialObject );

  m_BGPlaneSource = NULL;
  m_BGPlane = NULL;

  m_FGPlaneSource = NULL;
  m_FGPlane = NULL;

  // Create classes for displaying images
  m_BGImageSlice = vtkImageSlice::New();
  m_FGImageSlice = vtkImageSlice::New();

  // Setting of vtkImageProperty
	m_BGImageProperty = vtkImageProperty::New();
	m_BGImageProperty->SetColorWindow(m_Window);
	m_BGImageProperty->SetColorLevel(m_Level);
	m_BGImageProperty->SetAmbient(1.0);
    m_BGImageProperty->SetBacking(1.0);
    m_BGImageProperty->SetBackingColor(1.0,0.0,0.0);
	m_BGImageProperty->SetOpacity(0.5);
	m_BGImageProperty->SetInterpolationTypeToLinear();

	m_FGImageProperty = vtkImageProperty::New();
	m_FGImageProperty->SetColorWindow(m_Window);
	m_FGImageProperty->SetColorLevel(m_Level);
	m_FGImageProperty->SetAmbient(1.0);
    m_FGImageProperty->SetBacking(1.0);
    m_FGImageProperty->SetBackingColor(1.0,0.0,0.0);
	m_FGImageProperty->SetOpacity(0.5);
	m_FGImageProperty->SetInterpolationTypeToLinear();

  // Create classes for displaying images
  this->AddActor( m_BGImageSlice );
  this->AddActor( m_FGImageSlice );

  m_ResliceInterpolate       = VTK_NEAREST_RESLICE;

  m_BGPlaneSource = vtkPlaneSource::New();
  m_BGPlaneSource->SetXResolution(1);
  m_BGPlaneSource->SetYResolution(1);

  m_FGPlaneSource = vtkPlaneSource::New();
  m_FGPlaneSource->SetXResolution(1);
  m_FGPlaneSource->SetYResolution(1);

  // Define some default point coordinates
  double bounds[6];
  bounds[0] = -0.5;
  bounds[1] =  0.5;
  bounds[2] = -0.5;
  bounds[3] =  0.5;
  bounds[4] = -0.5;
  bounds[5] =  0.5;

  double center[3];
  center[0] = (bounds[0] + bounds[1])/2.0;
  center[1] = (bounds[2] + bounds[3])/2.0;
  center[2] = (bounds[4] + bounds[5])/2.0;

  m_BGPlaneSource->SetOrigin(center[0],bounds[2],bounds[4]);
  m_BGPlaneSource->SetPoint1(center[0],bounds[3],bounds[4]);
  m_BGPlaneSource->SetPoint2(center[0],bounds[2],bounds[5]);

  m_FGPlaneSource->SetOrigin(center[0],bounds[2],bounds[4]);
  m_FGPlaneSource->SetPoint1(center[0],bounds[3],bounds[4]);
  m_FGPlaneSource->SetPoint2(center[0],bounds[2],bounds[5]);

  m_BGPlane = vtkPlane::New();
  m_BGPlane->SetOrigin( m_BGPlaneSource->GetCenter() );
  m_BGPlane->SetNormal( m_BGPlaneSource->GetNormal() );

  m_FGPlane = vtkPlane::New();
  m_FGPlane->SetOrigin( m_FGPlaneSource->GetCenter() );
  m_FGPlane->SetNormal( m_FGPlaneSource->GetNormal() );

  m_Level = 0;
  m_Window = 0;

  m_VTKImageObserver = VTKImageObserver::New();

  m_ReslicerPlaneCenterObserver = ReslicerPlaneCenterObserver::New();
  m_ReslicerPlaneNormalObserver = ReslicerPlaneNormalObserver::New();

  igstkAddInputMacro( ValidImageSpatialObject );
  igstkAddInputMacro( InValidImageSpatialObject );
  igstkAddInputMacro( ValidReslicePlaneSpatialObject );
  igstkAddInputMacro( InValidReslicePlaneSpatialObject  );
  igstkAddInputMacro( ConnectVTKPipeline  );

  igstkAddStateMacro( Initial  );
  igstkAddStateMacro( ImageSpatialObjectSet );
  igstkAddStateMacro( ReslicePlaneSpatialObjectSet );

   //From Initial state

  igstkAddTransitionMacro( Initial, InValidImageSpatialObject, 
                           Initial, ReportInvalidImageSpatialObject );

  //From ImageSpatialObjectSet
  igstkAddTransitionMacro( ImageSpatialObjectSet,
                           ValidReslicePlaneSpatialObject,
                           ReslicePlaneSpatialObjectSet,
                           SetReslicePlaneSpatialObject);

  igstkAddTransitionMacro( ImageSpatialObjectSet,
                           InValidReslicePlaneSpatialObject,
                           ImageSpatialObjectSet,
                           ReportInvalidReslicePlaneSpatialObject );
 
  //From ReslicePlaneSpatialObjectSet
  igstkAddTransitionMacro( ReslicePlaneSpatialObjectSet, ConnectVTKPipeline,
                           ReslicePlaneSpatialObjectSet, ConnectVTKPipeline );
  
  igstkSetInitialStateMacro( Initial );
 
  m_StateMachine.SetReadyToRun();

}

/** Destructor */

template < class TBGImageSO,  class TFGImageSO  >
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::~ImageSliceRepresentationPlus()
{
  // This deletes also the m_BGActor
  this->DeleteActors();
  
   if ( m_BGImageSlice != NULL )
    {
    m_BGImageSlice->Delete();
    m_BGImageSlice = NULL;
    }

  if ( m_BGImageProperty != NULL )
    {
    m_BGImageProperty->Delete();
    m_BGImageProperty = NULL;
    }

  if ( m_BGPlaneSource != NULL )
    {
    m_BGPlaneSource->Delete();
    m_BGPlaneSource = NULL;
    }

  if ( m_BGPlane != NULL )
    {
    m_BGPlane->Delete();
    m_BGPlane = NULL;
    }

  if ( m_BGImageData )
    {
    m_BGImageData = NULL;
    }

   if ( m_FGImageSlice != NULL )
    {
    m_FGImageSlice->Delete();
    m_FGImageSlice = NULL;
    }

  if ( m_FGImageProperty != NULL )
    {
    m_FGImageProperty->Delete();
    m_FGImageProperty = NULL;
    }

  if ( m_FGPlaneSource != NULL )
    {
    m_FGPlaneSource->Delete();
    m_FGPlaneSource = NULL;
    }

  if ( m_FGPlane != NULL )
    {
    m_FGPlane->Delete();
    m_FGPlane = NULL;
    }

  if ( m_FGImageData )
    {
    m_FGImageData = NULL;
    }
  // Cleanup vtk classes
}


/** Overloaded DeleteActor function */
template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::DeleteActors( )
{
  igstkLogMacro( DEBUG, "igstk::ImageSliceRepresentationPlus\
                        ::DeleteActors called...\n");

  this->Superclass::DeleteActors();

  m_BGImageSlice = NULL;
  m_FGImageSlice = NULL;
}

/** Set the Image Spatial Object */
template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::RequestSetBGImageSO( BGImageSOType * image )
{
  m_ImageSpatialObjectToAdd = image;

 if( !m_ImageSpatialObjectToAdd )
    {
    m_StateMachine.PushInput( m_InValidImageSpatialObjectInput );
    }
 else {

  m_ImageSpatialObject = m_ImageSpatialObjectToAdd;

  this->RequestSetSpatialObject( m_ImageSpatialObject );
  
  unsigned int obsId = m_ImageSpatialObject->AddObserver( 
                                                        VTKImageModifiedEvent(),
                                                           m_VTKImageObserver );
  m_VTKImageObserver->Reset();

  m_ImageSpatialObject->RequestGetVTKImage();
  
  if( !m_VTKImageObserver->GotVTKImage() )
    {
    return;
    }

  this->SetImage( m_VTKImageObserver->GetVTKImage() );

  if( !m_BGImageData )
    {
    return;
    }

  m_ImageSpatialObject->RemoveObserver( obsId );

  m_BGImageData->UpdateInformation();

  double range[2];
  m_BGImageData->GetScalarRange(range);

  m_Window = range[1] - range[0];
  m_Level = 0.5*(range[0] + range[1]);
  
  if( fabs( m_Window ) < 0.001 )
    {
    m_Window = 0.001 * ( m_Window < 0.0 ? -1 : 1 );
    }

  if( fabs( m_Level ) < 0.001 )
    {
    m_Level = 0.001 * ( m_Level < 0.0 ? -1 : 1 );
    }

  this->SetWindowLevel( m_Window, m_Level );

  this->SetResliceInterpolate(m_ResliceInterpolate);

  m_BGImageData->GetWholeExtent(m_ImageExtent);

  m_BGImageData->GetOrigin(m_ImageOrigin);

  m_BGImageData->GetSpacing(m_ImageSpacing);

  // Prevent obscuring voxels by offsetting the plane geometry
  m_xbounds[0]= m_ImageOrigin[0] + m_ImageSpacing[0] * (m_ImageExtent[0] - 0.5);
  m_xbounds[1]= m_ImageOrigin[0] + m_ImageSpacing[0] * (m_ImageExtent[1] + 0.5);
  m_ybounds[0]= m_ImageOrigin[1] + m_ImageSpacing[1] * (m_ImageExtent[2] - 0.5);
  m_ybounds[1]= m_ImageOrigin[1] + m_ImageSpacing[1] * (m_ImageExtent[3] + 0.5);
  m_zbounds[0]= m_ImageOrigin[2] + m_ImageSpacing[2] * (m_ImageExtent[4] - 0.5);
  m_zbounds[1]= m_ImageOrigin[2] + m_ImageSpacing[2] * (m_ImageExtent[5] + 0.5);

   if ( m_ImageSpacing[0] < 0.0 )
    {
    double t = m_xbounds[0];
    m_xbounds[0] = m_xbounds[1];
    m_xbounds[1] = t;
    }

  if ( m_ImageSpacing[1] < 0.0 )
    {
    double t = m_ybounds[0];
    m_ybounds[0] = m_ybounds[1];
    m_ybounds[1] = t;
    }

  if ( m_ImageSpacing[2] < 0.0 )
    {
    double t = m_zbounds[0];
    m_zbounds[0] = m_zbounds[1];
    m_zbounds[1] = t;
    }

  m_BGPlaneSource->SetOrigin(m_xbounds[0],m_ybounds[0],m_zbounds[0]);

  m_BGPlane->SetOrigin( m_BGPlaneSource->GetCenter() );
  m_BGPlane->SetNormal( m_BGPlaneSource->GetNormal() );
  }
}

/** Set the Image Spatial Object */
template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::RequestSetFGImageSO( FGImageSOType * image )
{
  m_FGImageSOToAdd = image;

 if( !m_FGImageSOToAdd )
    {
    m_StateMachine.PushInput( m_InValidImageSpatialObjectInput );
    }
  else 
    {
    m_FGImageSO = m_FGImageSOToAdd;

    this->RequestSetSpatialObject( m_FGImageSO );

	m_FGImageSO->AddObserver( VTKImageModifiedEvent(),m_VTKImageObserver );

	m_VTKImageObserver->Reset();

	m_FGImageSO->RequestGetVTKImage();

	if( !m_VTKImageObserver->GotVTKImage() )
    {
    return;
    }

    if( m_VTKImageObserver->GotVTKImage() )
    {
      m_FGImageData = const_cast< vtkImageData *>
                                          ( m_VTKImageObserver->GetVTKImage() );
      m_FGImageData->Update();
    }

	if( !m_FGImageData )
    {
      return;
    }

    }

}

template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::ReportInvalidImageSpatialObjectProcessing( )
{  
  igstkLogMacro( DEBUG,"igstk::ImageSliceObjectRepresentationPlus\
                      ::ReportInvalidImageSpatialObjectProcessing called...\n");
}

template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::ReportInvalidReslicePlaneSpatialObjectProcessing( )
{  
  igstkLogMacro( DEBUG,"igstk::ImageSliceRepresentationPlus\
              ::ReportInvalidReslicePlaneSpatialObjectProcessing called...\n");
}

/** Set the Image Plane Spatial Object */
template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::RequestSetReslicePlaneSpatialObject( const ReslicerPlaneType *
reslicePlaneSpatialObject )
{  
  igstkLogMacro( DEBUG,"igstk::ImageSliceObjectRepresentationPlus\
                       ::RequestSetReslicePlaneSpatialObject called...\n");

  m_ReslicePlaneSpatialObjectToBeSet = 
                    const_cast< ReslicerPlaneType* >(reslicePlaneSpatialObject);

  if( !m_ReslicePlaneSpatialObjectToBeSet )
    {
    m_StateMachine.PushInput( m_InValidReslicePlaneSpatialObjectInput );
    }
  else
    {
    m_StateMachine.PushInput( m_ValidReslicePlaneSpatialObjectInput );
    }

  m_StateMachine.ProcessInputs();
}

template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::SetReslicePlaneSpatialObjectProcessing( )
{  
  igstkLogMacro( DEBUG,"igstk::ImageSliceRepresentationPlus\
                       ::SetReslicePlaneSpatialObjectProcessing called...\n");

  m_ReslicePlaneSpatialObject = m_ReslicePlaneSpatialObjectToBeSet;

  m_ReslicePlaneSpatialObject->AddObserver( 
                                  ReslicerPlaneType::ReslicerPlaneCenterEvent(),
                                                m_ReslicerPlaneCenterObserver );

  m_ReslicePlaneSpatialObject->AddObserver( 
                                  ReslicerPlaneType::ReslicerPlaneNormalEvent(),
                                                m_ReslicerPlaneNormalObserver );

  m_ReslicePlaneSpatialObject->RequestComputeReslicingPlane();

  typedef igstk::ReslicerPlaneSpatialObject::OrientationType OrientationType;

  OrientationType orientation
                        = m_ReslicePlaneSpatialObject->GetOrientationType();

  switch(orientation)
  {
    case ReslicerPlaneType::Axial:
      {
        m_BGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_BGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
		m_FGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_FGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
        break;
      }
    case ReslicerPlaneType::OffAxial:
      {
        m_BGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_BGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
		m_FGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_FGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
        break;
      }
    case ReslicerPlaneType::Coronal:
      {
        m_BGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_BGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[0],m_zbounds[1]);
		m_FGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_FGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[0],m_zbounds[1]);
        break;
      }
    case ReslicerPlaneType::OffCoronal:
      {
        m_BGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_BGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[0],m_zbounds[1]);
		m_FGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_FGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[0],m_zbounds[1]);
        break;
      }
    case ReslicerPlaneType::Sagittal:
      {
        m_BGPlaneSource->SetPoint1(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
        m_BGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[0],m_zbounds[1]);
		m_FGPlaneSource->SetPoint1(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
        m_FGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[0],m_zbounds[1]);
        break;
      }
    case ReslicerPlaneType::OffSagittal:
      {
        m_BGPlaneSource->SetPoint1(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
        m_BGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[0],m_zbounds[1]);
		m_FGPlaneSource->SetPoint1(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
        m_FGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[0],m_zbounds[1]);
        break;
      }
    case ReslicerPlaneType::PlaneOrientationWithXAxesNormal:
    case ReslicerPlaneType::PlaneOrientationWithYAxesNormal:
    case ReslicerPlaneType::PlaneOrientationWithZAxesNormal:
      {
        m_BGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_BGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
		m_FGPlaneSource->SetPoint1(m_xbounds[1],m_ybounds[0],m_zbounds[0]);
        m_FGPlaneSource->SetPoint2(m_xbounds[0],m_ybounds[1],m_zbounds[0]);
        break;
      }
    default:
      {
        std::cerr << "Invalid orientation" << std::endl;
        break;
      }
  }
}

/** Verify time stamp of the attached tool*/
template < class TBGImageSO,  class TFGImageSO  >
bool
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::VerifyTimeStamp( ) const
{
  igstkLogMacro( DEBUG, 
    "igstk::ImageSliceRepresentationPlus::VerifyTimeStamp called...\n");

  if( this->m_ReslicePlaneSpatialObject.IsNull() )
  return false;

  if( !this->m_ReslicePlaneSpatialObject->IsToolSpatialObjectSet() )
    return true;

  /* if a tool spatial object is driving the reslicing, compare the 
   *  tool spatial object transform with the view render time
   */
  if( this->GetRenderTimeStamp().GetExpirationTime() <
    this->m_ReslicePlaneSpatialObject->GetToolTransform().GetStartTime() ||
    this->GetRenderTimeStamp().GetStartTime() >
    this->m_ReslicePlaneSpatialObject->GetToolTransform().GetExpirationTime() )
    {
    // fixme
    double diff = 
      this->GetRenderTimeStamp().GetStartTime() - 
       m_ReslicePlaneSpatialObject->GetToolTransform().GetExpirationTime();

    if (diff > 450 )
      {
      return false;
      }
    else
      {
      return true;
      }
    }
  else
    {
    return true;
    }
}

template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::SetWindowLevel( double window, double level )
{
  igstkLogMacro( DEBUG, "igstk::ImageSliceRepresentationPlus\
                        ::SetWindowLevel called...\n");

  m_Window = window;
  m_Level = level;

  m_BGImageProperty->SetColorWindow(m_Window);
  m_BGImageProperty->SetColorLevel(m_Level);
  m_FGImageProperty->SetColorWindow(m_Window);
  m_FGImageProperty->SetColorLevel(m_Level);
}


/** Null Operation for a State Machine Transition */
template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::NoProcessing()
{
}

template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}

template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::UpdateRepresentationProcessing()
{
  igstkLogMacro( DEBUG, "igstk::ImageResliceObjectRepresentation::\
                         UpdateRepresentationProcessing called...\n");

  m_ReslicePlaneSpatialObject->RequestComputeReslicingPlane();

  m_ReslicerPlaneCenterObserver->Reset();
  m_ReslicerPlaneNormalObserver->Reset();  

  m_ReslicePlaneSpatialObject->RequestGetReslicingPlaneParameters();

  ReslicerPlaneType::VectorType reslicerPlaneCenter;

  if( m_ReslicerPlaneCenterObserver->GotReslicerPlaneCenter() )
    {
      reslicerPlaneCenter = 
                        m_ReslicerPlaneCenterObserver->GetReslicerPlaneCenter();
      m_BGPlane->SetOrigin( reslicerPlaneCenter[0], 
                          reslicerPlaneCenter[1], 
                          reslicerPlaneCenter[2] );
	  m_FGPlane->SetOrigin( reslicerPlaneCenter[0], 
                          reslicerPlaneCenter[1], 
                          reslicerPlaneCenter[2] );
    }
  else
    return;

  ReslicerPlaneType::VectorType reslicerPlaneNormal;

  if( m_ReslicerPlaneNormalObserver->GotReslicerPlaneNormal() )
    {
      reslicerPlaneNormal = 
                        m_ReslicerPlaneNormalObserver->GetReslicerPlaneNormal();
      m_BGPlane->SetNormal( reslicerPlaneNormal[0], 
                            reslicerPlaneNormal[1], 
                            reslicerPlaneNormal[2] );
	  m_FGPlane->SetNormal( reslicerPlaneNormal[0], 
                            reslicerPlaneNormal[1], 
                            reslicerPlaneNormal[2] );
    }
  else
    return;

  m_BGImageResliceMapper->SetSlicePlane(m_BGPlane);
  m_FGImageResliceMapper->SetSlicePlane(m_FGPlane);
}

/** Create the vtk Actors */
template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::CreateActors()
{
  igstkLogMacro( DEBUG, "igstk::ImageSliceRepresentationPlus\
                        ::CreateActors called...\n");

  // to avoid duplicates we clean the previous actors
  this->DeleteActors();

 // m_Stack     = vtkImageStack::New();
  m_BGImageSlice = vtkImageSlice::New();
  m_FGImageSlice = vtkImageSlice::New();
  this->AddActor( m_BGImageSlice );

 // m_Stack->RemoveAllInputs();
//  m_Stack->AddInput( m_BGImageSlice);
//  m_Stack->AddInput( m_FGImageSlice );
//  m_Actor->SetInput( m_Blender->GetOutput() );

//  this->AddActor( m_Actor );

  
  this->AddActor( m_FGImageSlice );

  igstkPushInputMacro( ConnectVTKPipeline );
  m_StateMachine.ProcessInputs(); 

}


/** Print Self function */
template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::SetImage( const vtkImageData * image )
{
  igstkLogMacro( DEBUG, "igstk::ImageSliceRepresentation\
                        ::SetImage called...\n");

  // This const_cast<> is needed here due to 
  // the lack of const-correctness in VTK 
  m_BGImageData = const_cast< vtkImageData *>( image );
  m_FGImageData = const_cast< vtkImageData *>( image );
}


/** Update the visual representation in response to changes in the geometric
 * object */
template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::ConnectVTKPipelineProcessing() 
{
  this->SetResliceInterpolate(m_ResliceInterpolate);

  // Setting of vtkImageResliceMapper
  m_BGImageResliceMapper = vtkImageResliceMapper::New();
  m_BGImageResliceMapper->SetInput(m_BGImageData);
  m_BGImageResliceMapper->SetSlicePlane(m_BGPlane);
  m_BGImageResliceMapper->BorderOn();

  m_FGImageResliceMapper = vtkImageResliceMapper::New();
  m_FGImageResliceMapper->SetInput(m_FGImageData);
  m_FGImageResliceMapper->SetSlicePlane(m_FGPlane);
  m_FGImageResliceMapper->BorderOn();

  m_BGImageSlice->SetMapper(m_BGImageResliceMapper);
  m_BGImageSlice->SetProperty(m_BGImageProperty);
  m_FGImageSlice->SetMapper(m_FGImageResliceMapper);
  m_FGImageSlice->SetProperty(m_FGImageProperty);
}

template < class TBGImageSO,  class TFGImageSO  >
void
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::SetResliceInterpolate(int value)
{
  if ( m_ResliceInterpolate == value )
    {
    return;
    }
    
  m_ResliceInterpolate = value;

  if ( m_BGImageSlice )
    {
    return;
    }
   if ( m_FGImageSlice )
    {
    return;
    }

  if ( value == VTK_NEAREST_RESLICE )
    {
      m_BGImageProperty->SetInterpolationTypeToNearest();
	  m_FGImageProperty->SetInterpolationTypeToNearest();
    }
  else if ( value == VTK_LINEAR_RESLICE)
    {
      m_BGImageProperty->SetInterpolationTypeToLinear();
	  m_FGImageProperty->SetInterpolationTypeToLinear();
    }
  else
    {
      m_BGImageProperty->SetInterpolationTypeToCubic();
	  m_FGImageProperty->SetInterpolationTypeToCubic();
    }
}

/** Create a copy of the current object representation */
template < class TBGImageSO,  class TFGImageSO  >
typename ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >::Pointer
ImageSliceRepresentationPlus < TBGImageSO,  TFGImageSO  >
::Copy() const
{
  igstkLogMacro( DEBUG, "igstk::ImageResliceObjectRepresentation\
                        ::Copy called...\n");

  Pointer newOR = ImageSliceObjectRepresentationPlus::New();
  newOR->SetColor( this->GetRed(),this->GetGreen(),this->GetBlue() );
  newOR->SetOpacity( this->GetOpacity() );
  newOR->RequestSetBGImageSO( m_ImageSpatialObject );
  newOR->RequestSetFGImageSO( m_FGImageSO );
  newOR->RequestSetReslicePlaneSpatialObject( m_ReslicePlaneSpatialObject );
  return newOR;
}

} // end namespace igstk

#endif
