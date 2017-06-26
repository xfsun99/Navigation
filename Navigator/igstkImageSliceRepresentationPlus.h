/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    $RCSfile: igstkImageSliceRepresentationPlus.h,v $
  Language:  C++

=========================================================================*/
#ifndef __igstkImageSliceRepresentationPlus_h
#define __igstkImageSliceRepresentationPlus_h

#include "igstkMacros.h"
#include "igstkTransformObserver.h"
#include "igstkObjectRepresentation.h"
#include "igstkImageSpatialObject.h"
#include "igstkStateMachine.h"
#include "igstkReslicerPlaneSpatialObject.h"

#include "vtkImageMapToColors.h"
#include "vtkPlaneSource.h"
#include "vtkPlane.h"
#include "vtkImageSlice.h"
#include "vtkImageResliceMapper.h"
#include "vtkImageProperty.h"

#include "vtkImageMapToWindowLevelColors.h"
#include "vtkLookupTable.h"
#include "vtkImageReslice.h"
#include "vtkImageActor.h"
#include "vtkImageBlend.h"
#include "vtkImageStack.h"

//#include "vtkMatrix4x4.h"
//#include "vtkCamera.h"

namespace igstk
{
#define VTK_NEAREST_RESLICE 0
#define VTK_LINEAR_RESLICE  1
#define VTK_CUBIC_RESLICE   2

/** \class ImageSliceRepresentationPlus
 *
 * \brief This class represents an oblique image object.
 *
 * \ingroup ObjectRepresentation
 */
template < class TBGImageSO,  class TFGImageSO  >
class ImageSliceRepresentationPlus : public ObjectRepresentation
{
public:
  /** Macro with standard traits declarations. */
  igstkStandardTemplatedClassTraitsMacro(
                                ImageSliceRepresentationPlus, ObjectRepresentation )
public:
  typedef TBGImageSO                                      BGImageSOType;
  typedef TFGImageSO                                      FGImageSOType;
  typedef typename BGImageSOType::ConstPointer            BGImageSOConstPointer;
  typedef typename FGImageSOType::ConstPointer            FGImageSOConstPointer;
  typedef typename BGImageSOType::PointType               PointType;
 // typedef typename FGImageSOType::PointType               PointType;
  typedef ReslicerPlaneSpatialObject                      ReslicerPlaneType;

  typedef ReslicerPlaneType::Pointer                      ReslicerPlanePointerType;
  typedef ReslicerPlaneType::Pointer                      ReslicerPlanePointerType;

  typedef ReslicerPlaneType::VectorType                   BGVectorType;
  typedef ReslicerPlaneType::VectorType                   FGVectorType;

  /** Return a copy of the current object representation */
  Pointer Copy() const;

  void RequestSetReslicePlaneSpatialObject( const ReslicerPlaneType *
                                                            planeSpatialObject);
                                                    
  /** Connect this representation class to the spatial object */
  void RequestSetBGImageSO( BGImageSOType * imageSO);
  void RequestSetFGImageSO( FGImageSOType * imageSO);
 //Sun void GetExtent(int & min, int & max);

  /** Set the window level of the resliced image plane */
  void SetWindowLevel( double window, double level );


    /** Set the reslice interpolation type internally used in
   * vtkImageReslice. VTK_NEAREST_RESLICE 0 (default)
   * VTK_LINEAR_RESLICE   1, VTK_CUBIC_RESLICE   2
   * */
  void SetResliceInterpolate(int value);

  /** Print the object information in a stream. */
  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const;

protected:

  /** Constructor */
  ImageSliceRepresentationPlus();
 
  /** Destructor */
  ~ImageSliceRepresentationPlus();

    /** Overloaded function to delete actors */
  void DeleteActors();

  /** Create the VTK actors for displaying geometry */
  void CreateActors();

  /** Verify time stamp. Use the reslicing tool transform to verify 
  * the time stamp */
  virtual bool VerifyTimeStamp() const;


private:

  ImageSliceRepresentationPlus(const Self&);
  void operator=(const Self&);   //purposely not implemented

  /** Internal itkSpatialObject */
  BGImageSOConstPointer                   m_ImageSpatialObject;
  BGImageSOConstPointer                   m_ImageSpatialObjectToAdd;
  FGImageSOConstPointer                   m_FGImageSO;
  FGImageSOConstPointer                   m_FGImageSOToAdd;

  /** Variables for managing reslice plane spatial object */
  ReslicerPlanePointerType               m_ReslicePlaneSpatialObjectToBeSet;
  ReslicerPlanePointerType               m_ReslicePlaneSpatialObject;
  
  //Sun double                                  m_BGOpacity;
  
  /** VTK classes that support display of an image */
  vtkImageData                          * m_BGImageData;
  vtkImageData                          * m_FGImageData;

  vtkPlaneSource                        * m_BGPlaneSource;
  vtkPlaneSource                        * m_FGPlaneSource;

  vtkPlane                              * m_BGPlane;
  vtkPlane                              * m_FGPlane;
  vtkImageSlice                         * m_BGImageSlice;
  vtkImageSlice                         * m_FGImageSlice;
  vtkImageResliceMapper                 * m_BGImageResliceMapper;
  vtkImageResliceMapper                 * m_FGImageResliceMapper;
  vtkImageProperty                      * m_BGImageProperty;
  vtkImageProperty                      * m_FGImageProperty;
  vtkImageStack                         * m_Stack;
  vtkImageActor                         * m_Actor;

  int    m_ResliceInterpolate;

  /** Variables that store window and level values for 2D image display */
  double                                 m_Level;
  double                                 m_Window;

   /** Variables that store image information, that not change */
  double                                 m_ImageSpacing[3];
  double                                 m_ImageOrigin[3];
  int                                    m_ImageExtent[6];
  double                                 m_xbounds[2];
  double                                 m_ybounds[2];
  double                                 m_zbounds[2];
  
  /** Update the visual representation with changes in the geometry */
  virtual void UpdateRepresentationProcessing();

   /** Null operation for State Machine transition */
  void NoProcessing();

  /** Set the reslice plane spatial object */
  void SetReslicePlaneSpatialObjectProcessing();

  /** Connect this representation class to the spatial object. Only to be
   * called by the State Machine. */
 // void SetImageSpatialObjectProcessing();

    /** Report invalid image spatial object type */
  void ReportInvalidImageSpatialObjectProcessing( void );

  /** Report invalid tool spatial object type */
  void ReportInvalidReslicePlaneSpatialObjectProcessing( void );

  /** Report invalid request */
  void ReportInvalidRequestProcessing( void );


  /** Sets the vtkImageData from the ImageSpatialObject. This method MUST be
   * private in order to prevent unsafe access from the VTK image layer. */
  void SetImage( const vtkImageData * image );

  /** Connect VTK pipeline */
  void ConnectVTKPipelineProcessing();

  /** Declare the observer that will receive a VTK image from the
   * ImageSpatialObject */
  igstkObserverMacro( VTKImage, VTKImageModifiedEvent,
                      EventHelperType::VTKImagePointerType);

   /** Declare the observers that will receive the reslicing plane parameters 
   * from the ReslicerPlaneSpatialObject */

  igstkObserverMacro( ReslicerPlaneCenter, 
                                    ReslicerPlaneType::ReslicerPlaneCenterEvent,
                                                 ReslicerPlaneType::VectorType);

  typename ReslicerPlaneCenterObserver::Pointer  m_ReslicerPlaneCenterObserver;

  igstkObserverMacro( ReslicerPlaneNormal, 
                                    ReslicerPlaneType::ReslicerPlaneNormalEvent,
                                                 ReslicerPlaneType::VectorType);

  typename ReslicerPlaneNormalObserver::Pointer  m_ReslicerPlaneNormalObserver;


  /** todo: see if we need this observer*/
  igstkObserverMacro( ImageTransform, CoordinateSystemTransformToEvent, 
     CoordinateSystemTransformToResult );

  typename VTKImageObserver::Pointer  m_VTKImageObserver;

private:

  /** Inputs to the State Machine */
  igstkDeclareInputMacro( ValidImageSpatialObject );
  igstkDeclareInputMacro( InValidImageSpatialObject );
  igstkDeclareInputMacro( ValidReslicePlaneSpatialObject );
  igstkDeclareInputMacro( InValidReslicePlaneSpatialObject );
  igstkDeclareInputMacro( ConnectVTKPipeline );

  /** States for the State Machine */
  igstkDeclareStateMacro( Initial );
  igstkDeclareStateMacro( ImageSpatialObjectSet );
  igstkDeclareStateMacro( ReslicePlaneSpatialObjectSet );

};

} // end namespace igstk

#ifndef IGSTK_MANUAL_INSTANTIATION
#include "igstkImageSliceRepresentationPlus.txx"
#endif


#endif // __igstkImageSliceRepresentationPlus_h
