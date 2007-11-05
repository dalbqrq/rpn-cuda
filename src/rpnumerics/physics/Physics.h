/**
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) Physics.h
 **/

#ifndef _Physics_H
#define	_Physics_H

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "Boundary.h"
#include "Space.h"
//#include "AccumulationFunction.h"
#include "FluxFunction.h"

//! Definition of class Physics.
/*!

 TODO:
 NOTE :

 @ingroup rpnumerics
 */

class Physics {
    
public :
	virtual ~Physics();
        
        virtual const FluxFunction & fluxFunction() const = 0;

        //virtual const AccumulationFunction & accumulation() const = 0;

        virtual const Space & domain() const = 0;

        virtual const Boundary & boundary() const= 0;

	virtual void boundary(const Boundary & boundary) = 0;
        
};

inline Physics::~Physics() { };

#endif	//! _Physics_H
