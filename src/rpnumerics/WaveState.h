/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) WaveState.h
 */

#ifndef _WaveState_H
#define _WaveState_H

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "RealVector.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */

class WaveState {
private:

public:
	WaveState(const int dim);
	virtual ~WaveState();
	
	//TODO  mudar component para operator 
	double operator()(const int comp) const ;
	double & operator()(const int comp);

protected:
	RealVector * coords_;

};

#endif //! _WaveState_H
