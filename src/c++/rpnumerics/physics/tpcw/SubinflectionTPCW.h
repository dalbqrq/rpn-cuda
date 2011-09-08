#ifndef _SUBINFLECTIONTPCW_
#define _SUBINFLECTIONTPCW_

#include "Thermodynamics_SuperCO2_WaterAdimensionalized.h"
#include "HugoniotFunctionClass.h"
#include "Flux2Comp2PhasesAdimensionalized.h"
#include "Accum2Comp2PhasesAdimensionalized.h"

class SubinflectionTPCW : public HugoniotFunctionClass {
private:
    const Thermodynamics_SuperCO2_WaterAdimensionalized *td;
    const Flux2Comp2PhasesAdimensionalized * fluxFunction_;
    double phi;

    void subinflection_function(double & reduced_lambdae, double & numeratorchiu, double & denominatorchiu, const RealVector &u);
protected:
public:
    SubinflectionTPCW(const Flux2Comp2PhasesAdimensionalized *, const Accum2Comp2PhasesAdimensionalized *);
    double HugoniotFunction(const RealVector &u);

    void completeCurve(std::vector<RealVector> &);
};

#endif // _SUBINFLECTIONTPCW_

