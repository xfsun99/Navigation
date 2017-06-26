#ifndef __MyView3D_h
#define __MyView3D_h

#include "MyView.h"

namespace igstk {

/** \class View3D
 *
 * \brief Provides the functionality of rendering a scene in a 3D window. 
 *
 * This class derives from the View class, and represents the
 * abstraction of a window in a GUI in which 3D objects will be displayed but
 * from a point of view in which the camera can change its orientation and
 * position.
 *
 *
 *  \image html  igstkView3D.png  "View3D State Machine Diagram"
 *  \image latex igstkView3D.eps  "View3D State Machine Diagram" 
 *
 *  \sa View
 *  \sa View2D
 *
 *  \ingroup View
 *
 */
class MyView3D : public MyView 
{
public:

  igstkStandardClassTraitsMacro( MyView3D, MyView ); 

  /** Print the object information in a stream. */
  void PrintSelf( std::ostream& os, ::itk::Indent indent ) const; 

protected:
  /** Constructor */
  MyView3D( );

  /** Destructor */
  virtual ~MyView3D( void );

private:
  MyView3D(const MyView3D& ); // purposely not implemented
  MyView3D& operator=(const MyView3D& ); // purposely not implemented

};

} // end namespace igstk

#endif 
