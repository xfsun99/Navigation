/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    $RCSfile: NavigatorRegistrationBase.h,v $
  Language:  C++
  Date:      $Date: 2015/04/23 23:34:22 $
  Version:   $Revision: 1.0 $
  Author; Sun Xiaofei
=========================================================================*/
#ifndef __NavigatorRegistrationBase_h
#define __NavigatorRegistrationBase_h

#include "itkImage.h"
#include "igstkCTImageSpatialObject.h"
#include "igstkTransform.h"

namespace igstk 
{

class NavigatorRegistrationBase: public itk::Object
{

public:

  typedef NavigatorRegistrationBase   Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer< Self >         Pointer;
  typedef itk::SmartPointer< const Self >   ConstPointer;

  typedef igstk::CTImageSpatialObject       ImageSOType;
  typedef ImageSOType::ImageType            ImageType;

  typedef igstk::Transform                  TransformType;

  itkGetMacro( ITKImage, ImageType::ConstPointer );
  itkSetMacro( ITKImage, ImageType::ConstPointer );
   
  virtual bool Execute() = 0;

  itkGetMacro( Transform, TransformType );  

protected:

  void PrintSelf( std::ostream& os, itk::Indent indent );
  
  ImageType::ConstPointer              m_ITKImage;
  TransformType                        m_Transform;

private:

};

} // end namespace igstk

#endif
