/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) TPCWLiquid.cc
 */

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "TPCWLiquid.h"
#include "rpnumerics/physics/tpcw/Accum2Comp2PhasesAdimensionalized.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */


TPCWLiquid::TPCWLiquid(const RealVector & paramsVector, Thermodynamics * thermo, VLE_Flash_TPCW * flash, const string & rpnHomePath) :
tc_(thermo), flash_(flash), SubPhysics(*defaultBoundary(flash),
*new Space("R2", 2), "TPCWLiquid", _GENERAL_ACCUMULATION_) {


    //    // Thermo
    double mc = 0.044;
    double mw = 0.018;

    double phi = 0.38;

    std::string temp(rpnHomePath);

    temp.append("/src/c++/rpnumerics/physics/tpcw/hsigmaC_spline.txt");


    // Liquid
   
    fluxFunction_ = new FluxSinglePhaseLiquidAdimensionalized(tc_);

    accumulationFunction_ = new AccumulationSinglePhaseLiquidAdimensionalized(phi, tc_);

//    setHugoniotFunction(new Hugoniot_TP());

    
    setHugoniotContinuationMethod(new HugoniotContinuation3D2D(fluxFunction_, accumulationFunction_, &getBoundary()));
     
     
     
    setDoubleContactFunction(new Double_Contact_TP());


}

void TPCWLiquid::boundary(const Boundary & newBoundary) {

    SubPhysics::boundary(newBoundary);
}

void TPCWLiquid::setParams(vector<string> params) {

    cout << "Chamando set Param em TPCWLiquid" << endl;



    double paramValue;


    RealVector paramsVector(params.size());

    for (int i = 0; i < params.size(); i++) {

        std::stringstream stream(params[i]);

        stream >> paramValue;

        paramsVector.component(i) = paramValue;

    }


    
//    
//    FluxSinglePhaseLiquidAdimensionalized_Params * fluxParams = (FluxSinglePhaseLiquidAdimensionalized_Params *) & fluxFunction_->fluxParams();
//    
//    fluxParams->component(0,paramsVector.component(0));
//    fluxParams->component(1,paramsVector.component(1));
//    fluxParams->component(2,paramsVector.component(2));
//    fluxParams->component(3,paramsVector.component(3));
//    fluxParams->component(4,paramsVector.component(4));
//    fluxParams->component(5,paramsVector.component(5));
//    fluxParams->component(6,paramsVector.component(6));
//    fluxParams->component(7,paramsVector.component(7));
//    fluxParams->component(8,paramsVector.component(11));
//    fluxParams->component(9,paramsVector.component(12));
////    fluxParams->component(10,paramsVector.component(13));
//    
    
    
    
    
    
    
    
    
    
    


   accumulationFunction_->accumulationParams().component(0,paramsVector.component(8));

    





//    RealVector fluxParamVector(8);
//
//    //Flux params
//    for (int i = 0; i < fluxParamVector.size(); i++) {
//
//        double paramValue = atof(params[i].c_str());
//        fluxParamVector.component(i) = paramValue;
//    }
//
//    fluxFunction_->fluxParams(FluxSinglePhaseLiquidAdimensionalized_Params(fluxParamVector));



}

vector<double> * TPCWLiquid::getParams() {



    vector<double> * paramsVector = new vector<double>();

    for (int i = 0; i < fluxFunction_->fluxParams().params().size(); i++) {
        paramsVector->push_back(fluxFunction_->fluxParams().params().component(i));

    }

    for (int i = 0; i < accumulationFunction_->accumulationParams().params().size(); i++) {


        paramsVector->push_back(accumulationFunction_->accumulationParams().component(i));


    }




    return paramsVector;

}

TPCWLiquid::TPCWLiquid(const TPCWLiquid & copy) :
SubPhysics(copy.fluxFunction(), copy.accumulation(), *copy.getBoundary().clone(), *new Space("R3", 3), "TPCWLiquid", _GENERAL_ACCUMULATION_) {


//    setHugoniotFunction(new Hugoniot_TP());
    setDoubleContactFunction(new Double_Contact_TP());
//    setHugoniotContinuationMethod(new HugoniotContinuation3D2D(fluxFunction_, accumulationFunction_, getBoundary()));

}

SubPhysics * TPCWLiquid::clone() const {

    return new TPCWLiquid(*this);
}

Boundary * TPCWLiquid::defaultBoundary()const {

    double Theta_min = 0.099309;
    double Theta_max = 0.576511;

    SinglePhaseBoundary * liquidBoundary = new SinglePhaseBoundary(flash_, Theta_min, Theta_max, DOMAIN_IS_LIQUID, &Thermodynamics::Theta2T);

    return liquidBoundary;

}

Boundary * TPCWLiquid::defaultBoundary(VLE_Flash_TPCW * flash)const {

    double Theta_min = 0.099309;
    double Theta_max = 0.576511;

    SinglePhaseBoundary * liquidBoundary = new SinglePhaseBoundary(flash, Theta_min, Theta_max, DOMAIN_IS_LIQUID, &Thermodynamics::Theta2T);


    return liquidBoundary;

}

void TPCWLiquid::preProcess(RealVector & input) {
    //    input.component(1) = TD->T2Theta(input.component(1));
    //    if (input.size() == 3) {
    //
    //        input.component(2) = TD->u2U(input.component(2));
    //
    //    }

}

void TPCWLiquid::postProcess(RealVector & input) {

    //    RealVector temp(input);
    //
    //
    //    input.resize(3);
    //    input.component(0) = temp.component(0);
    //    input.component(1) = TD->Theta2T(temp.component(1));
    //    input.component(2) = getBoundary().maximums().component(2);
    //
    //    //    input.component(2) = TD->U2u(temp.component(2));
}

void TPCWLiquid::postProcess(vector<RealVector> & input) {

    //    int inputSize = input[0].size();
    //
    //    for (int i = 0; i < input.size(); i++) {
    //
    //        switch (inputSize) {
    //            case 4://Rarefaction
    //                input[i].component(1) = TD->Theta2T(input[i].component(1));
    //                input[i].component(2) = TD->U2u(input[i].component(2)); // redimensionaliaztion of the darcy speed.
    //                input[i].component(3) = TD->U2u(input[i].component(3)); // redimensionalization of the the eigenvalue.
    //                break;
    //
    //            case 8://Shock
    //                input[i].component(1) = TD->Theta2T(input[i].component(1));
    //                input[i].component(2) = TD->U2u(input[i].component(2));
    //                input[i].component(5) = TD->U2u(input[i].component(5));
    //                break;
    //
    //            case 7: //Coincidence,BL,etc 
    //                input[i].component(2) = TD->U2u(input[i].component(2));
    //                input[i].component(1) = TD->Theta2T(input[i].component(1));
    //
    //                break;
    //
    //            case 2://Double contact , etc
    //                RealVector temp(input[i]);
    //
    //                input[i].resize(3);
    //                input[i].component(0) = temp.component(0);
    //                input[i].component(1) = TD->Theta2T(temp.component(1));
    //                input[i].component(2) = getBoundary().maximums().component(2);
    //
    //                break;
    //
    //        }
    //
    //
    //    }

}

TPCWLiquid::~TPCWLiquid() {
    //    delete tc;


}





