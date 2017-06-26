/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkView3D.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "MyView3D.h"

#include "vtkInteractorStyleTrackballCamera.h"

namespace igstk {

/** Constructor */
MyView3D::MyView3D( ) : m_StateMachine(this)
{
  vtkInteractorStyleTrackballCamera * interactorStyle = 
                                     vtkInteractorStyleTrackballCamera::New();
  this->SetInteractorStyle( interactorStyle );
  interactorStyle->Delete();
}

/** Destructor */
MyView3D::~MyView3D()
{
  this->SetInteractorStyle( NULL );
}

/** Print object information */
void MyView3D::PrintSelf( std::ostream& os, ::itk::Indent indent ) const
{
  this->Superclass::PrintSelf(os,indent);
}


} // end namespace igstk
