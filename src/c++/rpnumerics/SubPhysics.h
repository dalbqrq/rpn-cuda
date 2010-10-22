/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) SubPhysics.h
 */

#ifndef _SubPhysics_H
#define _SubPhysics_H

/*
 * ---------------------------------------------------------------
 * Includes:
 */

#include "FluxFunction.h"
#include "AccumulationFunction.h"
#include "Boundary.h"
#include "Space.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */



class SubPhysics {

private:

    FluxFunction * fluxFunction_;
    AccumulationFunction * accumulationFunction_;
    Boundary * boundary_;

public:

    SubPhysics(const FluxFunction &,const AccumulationFunction &);
    void fluxParams(const FluxParams &);

    void accumulationParams(const AccumulationParams &);

    const AccumulationFunction & accumulation() const;

    const Boundary & boundary() const;

    const FluxFunction & fluxFunction() const;

    virtual const Space & domain() const=0;

    virtual const char * ID() const=0;

    virtual SubPhysics * clone()const=0;


};

#endif //! _SubPhysics_H
