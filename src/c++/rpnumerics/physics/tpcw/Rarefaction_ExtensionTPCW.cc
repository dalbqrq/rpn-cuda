#include "Rarefaction_ExtensionTPCW.h"

void Rarefaction_ExtensionTPCW::extension_curve(const Flux2Comp2PhasesAdimensionalized *curve_flux, const Accum2Comp2PhasesAdimensionalized *curve_accum,
        const RealVector &initial_point,
        double deltaxi,
        int curve_family,
        int increase,
        const Boundary *boundary,
        const RealVector &pmin, const RealVector &pmax, int *number_of_grid_points, // For the domain.
        int domain_family,
        const Flux2Comp2PhasesAdimensionalized *domain_flux, const Accum2Comp2PhasesAdimensionalized *domain_accum,
        int characteristic_where, int singular,
        std::vector<RealVector> &curve_segments,
        std::vector<RealVector> &domain_segments) {
    curve_segments.clear();
    domain_segments.clear();

    std::vector<RealVector> rarefaction_curve;

    //    int info = Rarefaction::curve(initial_point,
    //                                  curve_family,
    //                                  increase,
    //                                  deltaxi,
    //                                  curve_flux, curve_accum,
    //                                  RAREFACTION_GENERAL_ACCUMULATION,
    //                                  (Boundary*)boundary,
    //                                  rarefaction_curve);
    //


    int info = Rarefaction::curve(initial_point,
            RAREFACTION_INITIALIZE_YES,
            (const RealVector *) 0,
            curve_family,
            increase,
            deltaxi,
            curve_flux, curve_accum,
            RAREFACTION_GENERAL_ACCUMULATION,
            (Boundary*) boundary,
            rarefaction_curve);



    if (rarefaction_curve.size() < 2) return;

    int n = initial_point.size();

    // Turn the curve of points into a curve of segments.
    vector <RealVector> rarefaction_segments;
    rarefaction_segments.resize(2 * rarefaction_curve.size() - 2);
    for (int i = 0; i < rarefaction_curve.size() - 1; i++) {
        rarefaction_segments[2 * i].resize(n);
        rarefaction_segments[2 * i + 1].resize(n);
        for (int j = 0; j < n; j++) {
            rarefaction_segments[2 * i].component(j) = rarefaction_curve[i].component(j);
            rarefaction_segments[2 * i + 1].component(j) = rarefaction_curve[i + 1].component(j);
        }
    }

    // Compute the extension curve for the rarefaction
    Extension_CurveTPCW extension_curvetpcw(pmin, pmax, number_of_grid_points,
            domain_flux, domain_accum);
    //                                            domain_reduced_flux, domain_reduced_accum);

    extension_curvetpcw.compute_extension_curve(characteristic_where, singular,
            rarefaction_segments, curve_family,
            curve_flux, curve_accum,
            domain_family,
            curve_segments,
            domain_segments);

    return;
}

