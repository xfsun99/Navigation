/*=========================================================================

  Program:   Image Guided Surgery Software Toolkit
  Module:    igstkView2D.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) ISC  Insight Software Consortium.  All rights reserved.
  See IGSTKCopyright.txt or http://www.igstk.org/copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __MyView2D_h
#define __MyView2D_h

#include "MyView.h"

namespace igstk {


/** \class View2D
 *
 * \brief Provides functionality of rendering a scene in a 2D window. 
 *
 * This class derives from the View class, and represents the
 * abstraction of a window in a GUI in which 3D objects will be displayed but
 * from a point of view in which the camera never changes the orientation of
 * the field of view. Zooming and Panning operations are possible in this
 * window.
 *
 *  \image html  igstkView2D.png  "View2D State Machine Diagram"
 *  \image latex igstkView2D.eps  "View2D State Machine Diagram" 
 *
 *  \sa View
 *  \ingroup View
 *
 */
class MyView2D : public MyView 
{
public:

  igstkStandardClassTraitsMacro( MyView2D, MyView );

  /** Print the object information in a stream. */
  void PrintSelf( std::ostream& os, ::itk::Indent indent ) const; 

  /** Orientation Type: Publically declared
   * orientation types supported for slice viewing.  */
  typedef enum
    { 
    Sagittal, 
    Coronal, 
    Axial 
    } 
  OrientationType;


  /** Select a slice orientation */
  void RequestSetOrientation( const OrientationType & orientation );

protected:
  /** Constructor */
  MyView2D( );

  /** Destructor */
  virtual ~MyView2D( void );

private:

  /** Variables for managing the Orientation of the slices */
  OrientationType      m_OrientationToBeSet;
  OrientationType      m_Orientation;

  MyView2D(const MyView2D& ); // purposely not implemented
  MyView2D& operator=(const MyView2D& ); // purposely not implemented

};

} // end namespace igstk

#endif 
