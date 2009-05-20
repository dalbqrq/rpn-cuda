/**
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) ShockFlowPlugin.h
 **/

#ifndef _SHOCKFLOWPLUGIN_H
#define	_SHOCKFLOWPLUGIN_H


/*!
 *
 *
 * TODO:
 * NOTE :
 *
 * @ingroup plugins
 */


#include "RpnPlugin.h"
#include "ShockFlow.h"
#include "Quad2FluxFunction.h"

class ShockFlowPlugin : public RpnPlugin, public ShockFlow {

private:

    RealVector * fx0_;

public:

    ShockFlowPlugin(const ShockFlowParams &, const FluxFunction &);

    const ShockFlowParams & getParams()const;
    void updateZeroTerms();
    void setParams(const ShockFlowParams & params);


    int flux(const RealVector &, RealVector &) const;
    int fluxDeriv(const RealVector &, JacobianMatrix &)const;
    int fluxDeriv2(const RealVector &, HessianMatrix &)const;

    WaveFlow * clone()const;
    
    virtual~ShockFlowPlugin();
    

};



#endif	/* _FLOWPLUGIN_H */

