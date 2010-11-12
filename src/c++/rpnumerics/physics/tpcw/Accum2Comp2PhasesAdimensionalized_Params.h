#ifndef _ACCUM2COMP_PHASESADIMENSIONALIZED_PARAMS_
#define _ACCUM2COMP_PHASESADIMENSIONALIZED_PARAMS_

#include <stdlib.h>

#include "AccumulationParams.h"
#include "Thermodynamics_SuperCO2_WaterAdimensionalized.h"

class Accum2Comp2PhasesAdimensionalized_Params : public AccumulationParams {
    private:
        double *vec;
        Thermodynamics_SuperCO2_WaterAdimensionalized *TD_;
    protected:
    public:
        Accum2Comp2PhasesAdimensionalized_Params(Thermodynamics_SuperCO2_WaterAdimensionalized*, double*);
        ~Accum2Comp2PhasesAdimensionalized_Params();

        Thermodynamics_SuperCO2_WaterAdimensionalized * get_thermodynamics(void) const;
};

#endif //_ACCUM2COMP_PHASESADIMENSIONALIZED_PARAMS_

