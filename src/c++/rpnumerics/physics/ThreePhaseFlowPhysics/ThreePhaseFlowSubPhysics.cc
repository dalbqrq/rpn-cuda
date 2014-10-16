#include "ThreePhaseFlowSubPhysics.h"

ThreePhaseFlowSubPhysics::ThreePhaseFlowSubPhysics() : SubPhysics() {
    boundary_ = new Three_Phase_Boundary;

    G_vertex.resize(2);
    G_vertex(0) = 0.0;
    G_vertex(1) = 0.0;

    W_vertex.resize(2);
    W_vertex(0) = 1.0;
    W_vertex(1) = 0.0;

    O_vertex.resize(2);
    O_vertex(0) = 0.0;
    O_vertex(1) = 1.0;

    
    
//    
//    cout << G_vertex << endl;
//    cout<< W_vertex<<endl;
//    cout<<W_vertex<<endl;
//    
    
    
    
    info_subphysics_ = std::string("ThreePhaseFlow base class");
    xlabel_ = std::string("sw");
    ylabel_ = std::string("so");

    // Extension curve.
    //
    iec = new Implicit_Extension_Curve;
    extension_curve.push_back(iec);

    // Flux and accumulation will be instantiated in the derived classes.
    //
    flux_ = 0;
    accumulation_ = new StoneAccumulation;

    // HugoniotContinuation will be instantiated in the derived classes because of the flux and accumulation.
    //
    hugoniotcontinuation_ = 0;

    // The permeability will be instantiated in the derived classes.
    permeability_ = 0;

    // Projection matrix.
    //
    double m[9] = {1.0, .5, 0.0, 0.0, sqrt(3)/2, 0.0, 0.0, 0.0, 1.0};
    transformation_matrix_ = DoubleMatrix(3, 3, m);
}

ThreePhaseFlowSubPhysics::~ThreePhaseFlowSubPhysics(){
    if (accumulation_ != 0) delete accumulation_;
    delete iec;
    delete boundary_;
}

