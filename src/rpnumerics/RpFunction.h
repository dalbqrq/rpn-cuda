/**
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) RpFunction.h
 **/

#ifndef RPFUNCTION_H
#define RPFUNCTION_H


#define RP_SUCCESSFUL 1

/*
** ---------------------------------------------------------------
** Includes:
*/
#include "RealVector.h"
#include "JetMatrix.h"


/*
** ---------------------------------------------------------------
** Definitions:
*/

//! Definition of class RpFunction. 
/*!
	The RpFunction class defines a generic function prototype. 
	
	f:u C Rm -> v C Rn 

	The jet method returns the nth derivative
TODO:
	make the jet method return f by default. In order to do that we must have
RealVector v parameter for f as a static or dynamic cast !!!
NOTE : 

@ingroup rpnumerics
*/
class RpFunction {

public:
	//! m coordinates function evaluation at u
	//! this is the nth derivative calculation that might be available or not
	virtual int jet(const int degree, const RealVector &u,JetMatrix &m) = 0;
};


#endif //RPFUNCTION_H
