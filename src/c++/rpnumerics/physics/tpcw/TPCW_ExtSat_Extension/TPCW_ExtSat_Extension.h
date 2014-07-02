#ifndef _TPCW_EXTSAT_EXTENSION_
#define _TPCW_EXTSAT_EXTENSION_

#include "Extension.h"

#include "Flux2Comp2PhasesAdimensionalized.h"
#include "Utilities.h"

// Only valid for TPCW-like classes.
//
class TPCW_ExtSat_Extension : public Extension {
    private:
    protected:
        // The component index such that p(i) = extension(p(i)).
        //
        unsigned int index_of_constant;

        // The other component.
        //
        unsigned int index_of_non_constant;

        const Flux2Comp2PhasesAdimensionalized *flux_;
    public:
        TPCW_ExtSat_Extension(const Flux2Comp2PhasesAdimensionalized *f);
        virtual ~TPCW_ExtSat_Extension();

        // 
        int extension(const RealVector &p, RealVector &ext_p);

        std::string name() const {
            return std::string("External saturation extension");
        }

        int extension_type(){return EXPLICIT_EXTENSION;}
};

#endif // _EXTSAT_EXTENSION_

