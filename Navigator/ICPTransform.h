#ifndef __ICPTransform_h
#define __ICPTransform_h

#include "NavigatorRegistrationBase.h"

namespace igstk
{

class ICPTransform: 
 public NavigatorRegistrationBase
{

public:

  typedef ICPTransform              Self;
  typedef NavigatorRegistrationBase                      Superclass;
  typedef itk::SmartPointer< Self >                            Pointer;
  typedef itk::SmartPointer< const Self >                      ConstPointer;

  itkNewMacro( Self );
   
  virtual bool Execute();

protected:
  ICPTransform();
  virtual ~ICPTransform(){};
  void PrintSelf( std::ostream& os, itk::Indent indent );

private:

};

} // end namespace igstk

#endif