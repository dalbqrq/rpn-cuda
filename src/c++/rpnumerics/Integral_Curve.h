#ifndef _INTEGRAL_CURVE_
#define _INTEGRAL_CURVE_

#include "Rarefaction.h"
#include <math.h>

class Integral_Curve{
    private:
    protected:
        const FluxFunction *flux;
        const AccumulationFunction *accum;
        const Boundary *boundary;

        double lambda_size;

        template <typename T> int sgn(T val){
            return (val > T(0)) - (val < T(0));
        }

    public:
        Integral_Curve(const FluxFunction *ff, const AccumulationFunction *aa, const Boundary *b);
        ~Integral_Curve();

        void set_lambda_level(double l);

        static void integral_curve(const RealVector &init, 
                                   const FluxFunction *f, const AccumulationFunction *a, 
                                   double deltaxi, int curve_family,
                                   const Boundary *boundary,
                                   std::vector<RealVector> &ic,  // ic = integral curve
                                   std::vector<RealVector> &ip); // ip = inflection points along the integral curve

        void integral_curve(const RealVector &init, double deltaxi, int family, std::vector<RealVector> &ic, std::vector<RealVector> &ip);

        void levels(const std::vector<RealVector> &integral_curve, std::vector<RealVector> &pos, std::vector<RealVector> &orientation);
};

#endif // _INTEGRAL_CURVE_

