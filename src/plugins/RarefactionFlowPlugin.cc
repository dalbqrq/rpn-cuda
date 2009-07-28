#include "RarefactionFlowPlugin.h"
#include "Quad2FluxFunction.h"



int RarefactionFlowPlugin::flux(const RealVector &input , RealVector & output)  {

    return 2;
}

int RarefactionFlowPlugin::fluxDeriv(const RealVector & input, JacobianMatrix & output) {

    return 2;
}

int RarefactionFlowPlugin::fluxDeriv2(const RealVector & input, HessianMatrix & output) {

    return 2;
}


RarefactionFlowPlugin::RarefactionFlowPlugin(const int familyIndex, const int direction, const FluxFunction &flux) : RarefactionFlow(familyIndex, direction, flux) {

}

RarefactionFlowPlugin::~RarefactionFlowPlugin() {

}


WaveFlow * RarefactionFlowPlugin::clone()const {
     Quad2FluxParams fluxParams;

    Quad2FluxFunction fluxfunction(fluxParams);
     return new RarefactionFlowPlugin(0, 1, fluxfunction);
}


extern "C" RpnPlugin * createRarefaction() {

    Quad2FluxParams fluxParams;

    Quad2FluxFunction fluxfunction(fluxParams);

    return new RarefactionFlowPlugin(0, 1, fluxfunction);
}

extern "C" void destroyRarefaction(RpnPlugin * plugin) {
    delete plugin;
}

