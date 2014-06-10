#include "Integral_Curve.h"
#include "Debug.h"

void Integral_Curve::integral_curve(const RealVector &init, 
                                    const FluxFunction *f, const AccumulationFunction *a, 
                                    double deltaxi, int curve_family,
                                    const Boundary *boundary,
                                    std::vector<RealVector> &ic,
                                    std::vector<RealVector> &ip){

    ic.clear();
    ip.clear();

    std::vector<RealVector> rarcurve;
    std::vector<RealVector> inflection_points;
    
    
    if ( Debug::get_debug_level() == 5 ) {
        //cout <<"Flag: "<<RAREFACTION_SPEED_DECREASE<<" "<< RAREFACTION_AS_ENGINE_FOR_INTEGRAL_CURVE <<" "<< RAREFACTION_GENERAL_ACCUMULATION<<" "<< RAREFACTION_INITIALIZE_YES<<" "<<RAREFACTION_INITIALIZE_YES<<endl;
    }

    int info_decrease = Rarefaction::curve(init, 
                                           RAREFACTION_INITIALIZE_YES,
                                           0, 
                                           curve_family, 
                                           RAREFACTION_SPEED_DECREASE, 
                                           RAREFACTION_AS_ENGINE_FOR_INTEGRAL_CURVE,
                                           deltaxi,
                                           f, a,
                                           RAREFACTION_GENERAL_ACCUMULATION,
                                           boundary,
                                           rarcurve,
                                           inflection_points);

    for (int i = rarcurve.size() - 1; i >= 0; i--)          ic.push_back(rarcurve[i]); 
    for (int i = inflection_points.size() - 1; i >= 0; i--) ip.push_back(inflection_points[i]);


    int info_increase = Rarefaction::curve(init, 
                                           RAREFACTION_INITIALIZE_YES,
                                           0, 
                                           curve_family, 
                                           RAREFACTION_SPEED_INCREASE, 
                                           RAREFACTION_AS_ENGINE_FOR_INTEGRAL_CURVE,
                                           deltaxi,
                                           f, a,
                                           RAREFACTION_GENERAL_ACCUMULATION,
                                           boundary,
                                           rarcurve,
                                           inflection_points);

    // Do not add the initial point twice, thus i = 1 and not i = 0.
    //
    for (int i = 1; i < rarcurve.size(); i++)          ic.push_back(rarcurve[i]); 

    // This as usual.
    for (int i = 0; i < inflection_points.size(); i++) ip.push_back(inflection_points[i]);

    return;
}

// This method is a wrapper for the static one.
//
void Integral_Curve::integral_curve(const RealVector &init, double deltaxi, int family, std::vector<RealVector> &ic, std::vector<RealVector> &ip){

    Integral_Curve::integral_curve(init, this->flux, this->accum, deltaxi, family, this->boundary, ic, ip);
    return;
}

Integral_Curve::Integral_Curve(const FluxFunction *ff, const AccumulationFunction *aa, const Boundary *b){
    flux = ff;
    accum = aa;
    boundary = b;

    lambda_size = 1.0;
}

Integral_Curve::~Integral_Curve(){
}

// Set the size of lambda.
// The tolerance below should depend on the physics.
//
void Integral_Curve::set_lambda_level(double l){
    lambda_size = max(fabs(l), 1e-10); // Tolerance is 1e-10, should depend on the physics.

    return;
}

// Think if it is worthwhile to set delta_xi.
//
//void Integral_Curve(){
//}

// Compute the arrows
//
void Integral_Curve::levels(const std::vector<RealVector> &integral_curve, std::vector<RealVector> &position, std::vector<RealVector> &orientation){
    position.clear();
    orientation.clear();

    if (integral_curve.size() < 2) return;

    int lp = integral_curve[0].size() - 1; // Position of lambda in the RealVector.

    for (int i = 0; i < integral_curve.size() - 1; i++){
        // Find the integer levels of the ends of each segment
        double lambda_first = integral_curve[i].component(lp) ;
        double lambda_last  = integral_curve[i + 1].component(lp);

        int lambda_first_level;// = (int)ceil(lambda_first/lambda_size);
        int lambda_last_level;//  = (int)ceil(lambda_last/lambda_size);
        int lambda_j_first, lambda_j_last;

        if (lambda_last > lambda_first){
            lambda_first_level = (int)floor(lambda_first/lambda_size);
            lambda_last_level  = (int)floor(lambda_last/lambda_size);

            lambda_j_first = lambda_first_level;
            lambda_j_last = lambda_last_level;
        }
        else {
            lambda_first_level = (int)ceil(lambda_first/lambda_size);
            lambda_last_level  = (int)ceil(lambda_last/lambda_size);

            lambda_j_first = lambda_last_level;
            lambda_j_last = lambda_first_level;
        }       
        if ( Debug::get_debug_level() == 5 ) {
            printf("lambda_first = %f, lambda_first_level = %d, lambda_last = %f, lambda_last_level = %d\n", lambda_first, lambda_first_level, lambda_last, lambda_last_level);
        }

        if (lambda_first_level == lambda_last_level) continue;

        int delta_j = (int)sgn(lambda_last_level - lambda_first_level);
        if ( Debug::get_debug_level() == 5 ) {
            printf("delta_j = %d\n", delta_j);
        }
        for (int j = lambda_j_first; j <= lambda_j_last; j++){
            double lambda = j*lambda_size;
            double alpha = (lambda - lambda_last)/(lambda_first - lambda_last);
            if (alpha < 0.0 || alpha > 1.0) continue;
            double beta = 1.0 - alpha;
            if ( Debug::get_debug_level() == 5 ) {
                 printf("    lambda_first = %f, lambda = %f, lambda_last = %f\n", lambda_first, lambda, lambda_last);
                 printf("    alpha = %f, beta = %f\n", alpha, beta);
            }

            RealVector pos(lp), ornt(lp);
            for (int k = 0; k < lp; k++){
                pos.component(k) = alpha*integral_curve[i].component(k) + beta*integral_curve[i + 1].component(k);
            }
            if ( Debug::get_debug_level() == 5 ) {
                //cout << "    First = " << integral_curve[i] << ", pos = " << pos << ", last = " << integral_curve[i + 1] << std::endl;
            }

            if (lambda_last > lambda_first){
                for (int k = 0; k < lp; k++){
                    ornt.component(k) = integral_curve[i + 1].component(k) - integral_curve[i].component(k);
                }
            }
            else {
                for (int k = 0; k < lp; k++){
                    ornt.component(k) = integral_curve[i].component(k) - integral_curve[i + 1].component(k);
                }
            }
            if ( Debug::get_debug_level() == 5 ) {
                //cout << "    Arrow = " << ornt << std::endl;
            }

            position.push_back(pos);
            orientation.push_back(ornt);
        }
    }

    // Normalize the orientations
    for (int i = 0; i < orientation.size(); i++){
        double norm = 0.0;
        for (int j = 0; j < orientation[i].size(); j++) norm += orientation[i].component(j)*orientation[i].component(j);
        double inv_norm = 1.0/sqrt(norm);
        for (int j = 0; j < orientation[i].size(); j++) orientation[i].component(j) *= inv_norm;
    }

    return;
}

