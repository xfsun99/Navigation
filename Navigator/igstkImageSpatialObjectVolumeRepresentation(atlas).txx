#ifndef __igstkImageSpatialObjectVolumeRepresentation_txx
#define __igstkImageSpatialObjectVolumeRepresentation_txx

#include "igstkImageSpatialObjectVolumeRepresentation.h"

namespace igstk
{

/** Constructor */
template < class TImageSpatialObject >
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::ImageSpatialObjectVolumeRepresentation():m_StateMachine(this)
{
  // We create the image spatial object
  m_ImageSpatialObject = NULL;

  this->RequestSetSpatialObject( m_ImageSpatialObject );

  // Create classes for displaying images
  m_BoneActor = vtkActor::New();
  this->AddActor( m_BoneActor );
  m_SkinActor = vtkActor::New();//F
  this->AddActor( m_SkinActor );//F
  m_ImageData  = NULL;

  // Create the observer to VTK image events 
  m_VTKImageObserver = VTKImageObserver::New();

  m_ShiftBy = 600;

  m_ImageThreshold = vtkImageThreshold::New();
  m_ShiftScale = vtkImageShiftScale::New();

  //Bone rendering 
  m_BoneExtractor  = vtkContourFilter::New();  //F
  m_BoneNormals  = vtkPolyDataNormals::New();  //F
  m_BoneStripper = vtkStripper::New();   //F
  m_BoneMapper = vtkPolyDataMapper::New();//F

  //Skin Rendering
  m_SkinExtractor  = vtkContourFilter::New();  //F
  m_SkinNormals  = vtkPolyDataNormals::New();  //F
  m_SkinStripper = vtkStripper::New();   //F
  m_SkinMapper = vtkPolyDataMapper::New();//F

  igstkAddInputMacro( ValidImageSpatialObject );
  igstkAddInputMacro( NullImageSpatialObject  );
  igstkAddInputMacro( EmptyImageSpatialObject  );
  igstkAddInputMacro( ConnectVTKPipeline );

  igstkAddStateMacro( NullImageSpatialObject );
  igstkAddStateMacro( ValidImageSpatialObject );
 
  igstkAddTransitionMacro( NullImageSpatialObject, NullImageSpatialObject,
                           NullImageSpatialObject,  No );
  igstkAddTransitionMacro( NullImageSpatialObject, EmptyImageSpatialObject,
                           NullImageSpatialObject,  No );
  igstkAddTransitionMacro( NullImageSpatialObject, ValidImageSpatialObject,
                           ValidImageSpatialObject,  SetImageSpatialObject );
  
  igstkAddTransitionMacro( ValidImageSpatialObject, NullImageSpatialObject,
                           NullImageSpatialObject,  No ); 
  igstkAddTransitionMacro( ValidImageSpatialObject, EmptyImageSpatialObject,
                           NullImageSpatialObject,  No ); 
  igstkAddTransitionMacro( ValidImageSpatialObject, ValidImageSpatialObject,
                           ValidImageSpatialObject,  No ); 
  
  igstkAddTransitionMacro( NullImageSpatialObject, ConnectVTKPipeline,
                           NullImageSpatialObject, No );
  igstkAddTransitionMacro( ValidImageSpatialObject, ConnectVTKPipeline,
                           ValidImageSpatialObject, ConnectVTKPipeline );

  igstkSetInitialStateMacro( NullImageSpatialObject );

  m_StateMachine.SetReadyToRun();
} 

/** Destructor */
template < class TImageSpatialObject >
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::~ImageSpatialObjectVolumeRepresentation()  
{


  // This deletes also the Actors
  this->DeleteActors();

  if(m_BoneExtractor )
    {
    m_BoneExtractor->Delete();
    }
  if(m_BoneNormals) 
    {
    m_BoneNormals->Delete();
    }
  if(m_BoneStripper)
    {
	m_BoneStripper ->Delete();
	}
  if(m_BoneMapper)
    {
	m_BoneMapper->Delete();
	}
  if(m_SkinExtractor )
    {
    m_SkinExtractor->Delete();
    }
  if(m_SkinNormals) 
    {
    m_SkinNormals->Delete();
    }
  if(m_SkinStripper)
    {
	m_SkinStripper ->Delete();
	}
  if(m_SkinMapper)
    {
	m_SkinMapper->Delete();
	}
}

/** Overloaded DeleteActor function*/
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::DeleteActors( )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::DeleteActors called...\n");
  
  this->Superclass::DeleteActors();
  
  m_BoneActor = NULL;
  
  m_SkinActor = NULL; 

}
 
/** Set the Image Spatial Object */
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::RequestSetImageSpatialObject( const ImageSpatialObjectType * image )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::RequestSetImageSpatialObject called...\n");
  
  m_ImageSpatialObjectToAdd = image;

  if( !m_ImageSpatialObjectToAdd )
    {
    m_StateMachine.PushInput( m_NullImageSpatialObjectInput );
    }
  else 
    {
    if( m_ImageSpatialObjectToAdd->IsEmpty() )
      {
      m_StateMachine.PushInput( m_EmptyImageSpatialObjectInput );
      }
    else
      {
      m_StateMachine.PushInput( m_ValidImageSpatialObjectInput );
      }
    }
  
  m_StateMachine.ProcessInputs();
}


/** Null Operation for a State Machine Transition */
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::NoProcessing()
{
}

/** Set the Image Spatial Object */
template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::SetImageSpatialObjectProcessing()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::SetImageSpatialObjectProcessing called...\n");

  // We create the image spatial object
  m_ImageSpatialObject = m_ImageSpatialObjectToAdd;

  m_ImageSpatialObject->AddObserver( VTKImageModifiedEvent(), 
                                     m_VTKImageObserver );

  this->RequestSetSpatialObject( m_ImageSpatialObject );
  
  // This method gets a VTK image data from the private method of the
  // ImageSpatialObject and stores it in the representation by invoking the
  // private SetImage method.
  

  this->m_ImageSpatialObject->RequestGetVTKImage();

  if( this->m_VTKImageObserver->GotVTKImage() ) 
    {
    m_ImageData = this->m_VTKImageObserver->GetVTKImage();
    if( m_ImageData )
    {
    m_ImageData->Update();
     }
    }

  vtkImageResample *resample = vtkImageResample::New();
  resample->SetInput( m_ImageData );
  resample->SetDimensionality(3); 
  resample->SetAxisMagnificationFactor(0,0.4); 
  resample->SetAxisMagnificationFactor(1,0.4); 
  resample->SetAxisMagnificationFactor(2,0.4);

  m_ImageThreshold->SetInput(resample->GetOutput());
  m_ImageThreshold->ThresholdBetween(50.00,563.00);
  m_ImageThreshold->SetOutValue(0);
  m_ImageThreshold->Update();

  m_ShiftScale->SetInput( m_ImageThreshold->GetOutput() );

  m_BoneExtractor->SetInput( m_ShiftScale->GetOutput() );  //F
  m_BoneNormals->SetInput( m_BoneExtractor->GetOutput() );  //F
  m_BoneStripper->SetInput( m_BoneNormals->GetOutput() );   //F
  m_BoneMapper->SetInput( m_BoneStripper->GetOutput() );//F
  m_BoneActor->SetMapper(m_BoneMapper);//F
  m_BoneMapper->ScalarVisibilityOff();//F

  m_SkinExtractor->SetInput( m_ShiftScale->GetOutput() );  //F
  m_SkinNormals->SetInput( m_SkinExtractor->GetOutput() );  //F
  m_SkinStripper->SetInput( m_SkinNormals->GetOutput() );   //F
  m_SkinMapper->SetInput( m_SkinStripper->GetOutput() );//F
  m_SkinActor->SetMapper(m_SkinMapper);//F
  m_SkinMapper->ScalarVisibilityOff();//F

}


/** Print Self function */
template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf(os, indent);
}


/** Update the visual representation in response to changes in the geometric
 * object */
template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::UpdateRepresentationProcessing()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                       ::UpdateRepresentationProcessing called...\n");
  if( m_ImageData )
    {
  //   m_ShiftScale->SetInput( m_ImageData );

	m_ImageThreshold->SetInput(m_ImageData);
    m_ImageThreshold->ThresholdBetween(50.00,563.00);
	m_ImageThreshold->SetOutValue(0);
    m_ImageThreshold->Update();

    m_ShiftScale->SetInput( m_ImageThreshold->GetOutput() );

    m_BoneExtractor->SetInput( m_ShiftScale->GetOutput() );  //F
    m_BoneNormals->SetInput( m_BoneExtractor->GetOutput() );  //F
    m_BoneStripper->SetInput( m_BoneNormals->GetOutput() );   //F
    m_BoneMapper->SetInput( m_BoneStripper->GetOutput() );//F

	m_SkinExtractor->SetInput( m_ShiftScale->GetOutput() );  //F
    m_SkinNormals->SetInput( m_SkinExtractor->GetOutput() );  //F
    m_SkinStripper->SetInput( m_SkinNormals->GetOutput() );   //F
    m_SkinMapper->SetInput( m_SkinStripper->GetOutput() );//F

    }
}


/** Create the vtk Actors */
template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::CreateActors()
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::CreateActors called...\n");

  // to avoid duplicates we clean the previous actors
  this->DeleteActors();

  m_BoneActor = vtkActor::New();
  this->AddActor( m_BoneActor );

  m_SkinActor = vtkActor::New();//F
  this->AddActor(m_SkinActor);//F

  m_ShiftScale = vtkImageShiftScale::New();
  m_ImageThreshold->SetInput(m_ImageData);
  m_ImageThreshold->ThresholdBetween(50.00,563.00);
  m_ImageThreshold->SetOutValue(0);
  m_ImageThreshold->Update();
  m_ShiftScale->SetInput( m_ImageThreshold->GetOutput() );
//  m_ShiftScale->SetInput( m_ImageData );
  m_ShiftScale->SetShift( m_ShiftBy );
  m_ShiftScale->SetOutputScalarTypeToUnsignedShort();

  //Bone Rendering  
  m_BoneExtractor->SetValue(0,0.0);
  m_BoneExtractor->Update();
  m_BoneNormals->SetFeatureAngle(60.0);
  m_BoneNormals->Update();
  m_BoneStripper->Update();

   //Skin Rendering  
//  m_SkinExtractor->SetValue(3,100£¬300);
//  m_SkinExtractor->GenerateValues(3, 100, 300);
//  m_SkinExtractor->SetValue(3, 400); 
//  m_SkinExtractor->Update();
//  m_SkinNormals->SetFeatureAngle(120.0);
//  m_SkinNormals->Update();

  igstkPushInputMacro( ConnectVTKPipeline );
  m_StateMachine.ProcessInputs(); 

}

/** Create a copy of the current object representation */
template < class TImageSpatialObject >
typename ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >::Pointer
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::Copy() const
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::Copy called...\n");

  Pointer newOR = 
         ImageSpatialObjectVolumeRepresentation<TImageSpatialObject>::New();
  newOR->SetColor(this->GetRed(),this->GetGreen(),this->GetBlue()); 
  newOR->SetOpacity(this->GetOpacity());
  newOR->RequestSetImageSpatialObject(m_ImageSpatialObject);

  return newOR;
}

template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::SetImage( const vtkImageData * image )
{
  igstkLogMacro( DEBUG, "igstk::ImageSpatialObjectRepresentation\
                        ::SetImage called...\n");

  // This const_cast<> is needed here due to the lack of 
  // const-correctness in VTK 
  m_ImageData = const_cast< vtkImageData *>( image );
}


template < class TImageSpatialObject >
void
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::ConnectVTKPipelineProcessing() 
{

m_BoneActor->SetMapper(m_BoneMapper);
m_BoneActor->GetProperty()->SetRepresentationToSurface();
m_BoneActor->GetProperty()->SetOpacity(1.0);
m_BoneActor->GetProperty()->SetDiffuseColor(1.000000, 0.937033, 0.954531);
m_BoneActor->GetProperty()->SetAmbient(0.04);
m_BoneActor->GetProperty()->SetDiffuse(0.9);
m_BoneActor->GetProperty()->SetSpecular(0.5);
m_BoneActor->GetProperty()->SetSpecularPower(60.0);
m_BoneActor->SetVisibility( 1 );
m_BoneActor->SetPickable( 0 );

/**m_SkinActor->SetMapper(m_SkinMapper);//F
m_SkinActor->GetProperty()->SetRepresentationToSurface();//F
m_SkinActor->GetProperty()->SetOpacity(1.0);
m_SkinActor->GetProperty()->SetDiffuseColor(1.0,0.49,0.25);
m_SkinActor->SetVisibility( 1 );
m_SkinActor->SetPickable( 0 );**/

}

template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::updateOpacityTransferFunction( double value )
{
	m_BoneActor->GetProperty()->SetOpacity(value);
}

template < class TImageSpatialObject >
void 
ImageSpatialObjectVolumeRepresentation< TImageSpatialObject >
::updateSkinOpacityTransferFunction( double value )
{ 
	m_SkinActor->GetProperty()->SetOpacity(value);

}

} // end namespace igstk

#endif
