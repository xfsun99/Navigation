#ifndef __MyView_h
#define __MyView_h

#ifdef _MSC_VER
#pragma warning ( disable : 4018 )
//Warning about: identifier was truncated to '255' characters in the debug
//information (MVC6.0 Debug)
#pragma warning( disable : 4284 )
#endif

// VTK declarations
class vtkRenderWindow;
class vtkCamera;
class vtkProp;
class vtkInteractorStyle;
class vtkRenderer;


// ITK headers
#include "itkCommand.h"

// IGSTK headers

#include "igstkView.h"


namespace igstk {

/** \class MyView
 *  
 *  \brief Display graphical representations of surgical scenes.
 * 
 *  The functionality of the View class is to aggregate all the graphical
 *  representations of spatial objects into one scene. igstk::View2D and
 *  igstk::View3D classes are derived from this class for 2D and 3D viewing
 *  capabilities. The view class encapsulate VTK classes into a restrictive
 *  API subjected to control of a state machine.
 *
 * \image html igstkView.png  "State Machine Diagram"
 * \image latex igstkView.eps "State Machine Diagram" 
 *
 * \sa View2D
 * \sa View3D
 *
 * \ingroup Object
 * \ingroup View
 */
class MyView : public View
{

public:


  igstkStandardClassTraitsMacro( MyView, View );  
  /** Get renderer2 */ 
  vtkRenderer * GetRenderer2() const; //New 


public:

 vtkRenderer           * m_Renderer2;
	 
protected:

  MyView( );
  virtual ~MyView( void );


}; // end namespace igstk
std::ostream& operator<<(std::ostream& os, const MyView& o);
}
#endif