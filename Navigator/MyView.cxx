#if defined(_MSC_VER)
#pragma warning ( disable : 4355 )
#endif

// vtk include files
#include "vtkVersion.h"
#include "vtkCommand.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#include "vtkViewport.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkInteractorStyle.h"
#include "vtkRenderer.h"
#include "vtkWorldPointPicker.h"


#if defined(__APPLE__) && defined(VTK_USE_CARBON)
#include "vtkCarbonRenderWindow.h"
#endif

//igstk include files
#include "MyView.h"

#include "itksys/SystemTools.hxx"

namespace igstk
{

/** Constructor */
MyView::MyView() : 
m_StateMachine(this)
{ 
 
  this->m_Renderer2 = vtkRenderer::New();
  
  // Set the default view to be axial
  
}

/** Destructor */
MyView::~MyView()
{
}

/** Get renderer */ 

 
vtkRenderer *  MyView::GetRenderer2() const
{
  return this->m_Renderer2; 
}


} // end namespace igstk