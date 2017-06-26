/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    $RCSfile: NavigatorRegistrationBase.cxx,v $
  Language:  C++
  Date:      $Date: 2015/04/23 23:34:22 $
  Version:   $Revision: 1.0 $
  Author: Sun Xiaofei

=========================================================================*/
#include "NavigatorRegistrationBase.h"

namespace igstk 
{

void NavigatorRegistrationBase::
  PrintSelf(std::ostream& os, itk::Indent indent)
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Image: " << m_ITKImage << std::endl;
  os << indent << "Transform: " << m_Transform << std::endl;
}

} // end namespace igstk
