#include <vector>

#include "Extension_Curve.h"

int Extension_Curve::species_physic(Extension_Curve *ec, double *foncub, int domain_i, int domain_j, int kl){
    double lambda;

    if (ec->characteristic_where == CHARACTERISTIC_ON_CURVE) {
        lambda = ec->segment_lambda[kl];
    } else {
        lambda = ec->gv->e(domain_i, domain_j)[ec->family].r;
    }

    foncub[0] = lambda * (ec->gv->G_on_grid(domain_i, domain_j).component(0) - ec->segment_accum(0, kl))
            - (ec->gv->F_on_grid(domain_i, domain_j).component(0) - ec->segment_flux(0, kl));
    foncub[1] = lambda * (ec->gv->G_on_grid(domain_i, domain_j).component(1) - ec->segment_accum(1, kl))
            - (ec->gv->F_on_grid(domain_i, domain_j).component(1) - ec->segment_flux(1, kl));

    return VALID_FUNCTION_ON_VERTICES;
}

int Extension_Curve::compositional_physic(Extension_Curve *ec, double *foncub, int domain_i, int domain_j, int kl){
    double lambda;

    double F10 = ec->segment_flux(0, kl);
    double F20 = ec->segment_flux(1, kl);
    double F30 = ec->segment_flux(2, kl);

    double dG1 = ec->gv->G_on_grid(domain_i, domain_j).component(0) - ec->segment_accum(0, kl);
    double dG2 = ec->gv->G_on_grid(domain_i, domain_j).component(1) - ec->segment_accum(1, kl);
    double dG3 = ec->gv->G_on_grid(domain_i, domain_j).component(2) - ec->segment_accum(2, kl);

    double F1 = ec->gv->F_on_grid(domain_i, domain_j).component(0);
    double F2 = ec->gv->F_on_grid(domain_i, domain_j).component(1);
    double F3 = ec->gv->F_on_grid(domain_i, domain_j).component(2);

    double X12 = F1 * dG2 - F2 * dG1;
    double X31 = F3 * dG1 - F1 * dG3;
    double X23 = F2 * dG3 - F3 * dG2;

    double X12_0 = F10 * dG2 - F20 * dG1;
    double X31_0 = F30 * dG1 - F10 * dG3;
    double X23_0 = F20 * dG3 - F30 * dG2;

    double Y21 = F2 * F10 - F1*F20;
    double Y13 = F1 * F30 - F3*F10;
    double Y32 = F3 * F20 - F2*F30;

    double red_shock_speed;
    double den = X12 * X12 + X31 * X31 + X23*X23;
    double scaling_factor = (X12_0 * X12 + X31_0 * X31 + X23_0 * X23) / den;

    if ( fabs(den) < 1.0e-8) {

        den = X12_0 * X12_0 + X31_0 * X31_0 + X23_0 * X23_0;
        red_shock_speed = (Y21 * X12_0 + Y13 * X31_0 + Y32 * X23_0) / den;

        if ( fabs(den) < 1.0e-12) return INVALID_FUNCTION_ON_VERTICES;

    } else {
        red_shock_speed = (Y21 * X12 + Y13 * X31 + Y32 * X23) / den;
    }
        
    if (ec->characteristic_where == CHARACTERISTIC_ON_CURVE) {
        lambda = ec->segment_lambda[kl];
    } else {
        lambda = scaling_factor * ec->gv->e(domain_i, domain_j)[ec->family].r;
    }
        
    foncub[0] = dG1 * (F2 * F30 - F3 * F20) - dG2 * (F1 * F30 - F3 * F10) + dG3 * (F1 * F20 - F2 * F10);
    foncub[1] = red_shock_speed - lambda;

    return VALID_FUNCTION_ON_VERTICES;
}

int Extension_Curve::function_on_vertices(double *foncub, int domain_i, int domain_j, int kl) {
    if (gv == 0 || oc == 0) return INVALID_FUNCTION_ON_VERTICES;

    if (!gv->eig_is_real(domain_i, domain_j)[family]) return INVALID_FUNCTION_ON_VERTICES;

//    double lambda;
    
    int info = (*type_of_physic)(this, foncub, domain_i, domain_j, kl);

//    if (gv->grid(0, 0).size() == 2) {
//        if (characteristic_where == CHARACTERISTIC_ON_CURVE) {
//            lambda = segment_lambda[kl];
//        } else {
//            lambda = gv->e(domain_i, domain_j)[family].r;
//        }

//        foncub[0] = lambda * (gv->G_on_grid(domain_i, domain_j).component(0) - segment_accum(0, kl))
//                - (gv->F_on_grid(domain_i, domain_j).component(0) - segment_flux(0, kl));
//        foncub[1] = lambda * (gv->G_on_grid(domain_i, domain_j).component(1) - segment_accum(1, kl))
//                - (gv->F_on_grid(domain_i, domain_j).component(1) - segment_flux(1, kl));
//    }

//    else {
//        double F10 = segment_flux(0, kl);
//        double F20 = segment_flux(1, kl);
//        double F30 = segment_flux(2, kl);

//        double dG1 = gv->G_on_grid(domain_i, domain_j).component(0) - segment_accum(0, kl);
//        double dG2 = gv->G_on_grid(domain_i, domain_j).component(1) - segment_accum(1, kl);
//        double dG3 = gv->G_on_grid(domain_i, domain_j).component(2) - segment_accum(2, kl);

//        double F1 = gv->F_on_grid(domain_i, domain_j).component(0);

//        double F2 = gv->F_on_grid(domain_i, domain_j).component(1);
//        double F3 = gv->F_on_grid(domain_i, domain_j).component(2);

//        double X12 = F1 * dG2 - F2 * dG1;
//        double X31 = F3 * dG1 - F1 * dG3;
//        double X23 = F2 * dG3 - F3 * dG2;

//        double X12_0 = F10 * dG2 - F20 * dG1;
//        double X31_0 = F30 * dG1 - F10 * dG3;
//        double X23_0 = F20 * dG3 - F30 * dG2;

//        double Y21 = F2 * F10 - F1*F20;
//        double Y13 = F1 * F30 - F3*F10;
//        double Y32 = F3 * F20 - F2*F30;

//        double red_shock_speed;
//        double den = X12 * X12 + X31 * X31 + X23*X23;
//        double scaling_factor = (X12_0 * X12 + X31_0 * X31 + X23_0 * X23) / den;
//        if ( fabs(den) < 1.0e-8) {

//           den = X12_0 * X12_0 + X31_0 * X31_0 + X23_0 * X23_0;
//           red_shock_speed = (Y21 * X12_0 + Y13 * X31_0 + Y32 * X23_0) / den;

//          if ( fabs(den) < 1.0e-12) return INVALID_FUNCTION_ON_VERTICES;

//        } else {
//        red_shock_speed = (Y21 * X12 + Y13 * X31 + Y32 * X23) / den;
//        }
//        
////        //cout<<"red : "<<red_shock_speed<<endl;


//        if (characteristic_where == CHARACTERISTIC_ON_CURVE) {

//            lambda = segment_lambda[kl];
////            //cout << "Valor do lambda: " << lambda << endl;

//        } else {

//                    lambda = scaling_factor * gv->e(domain_i, domain_j)[family].r;

//        }if (gv->grid(0, 0).size() == 2) {
//        


//        foncub[0] = dG1 * (F2 * F30 - F3 * F20) - dG2 * (F1 * F30 - F3 * F10) + dG3 * (F1 * F20 - F2 * F10);
//        foncub[1] = red_shock_speed - lambda;


//    }

    return info;
}

bool Extension_Curve::valid_segment(int i) {
    if (oc == 0) return false;

    double epsilon = 1e-7;

    int dim = oc->at(i).size();

    double F[dim], G[dim], JF[dim][dim], JG[dim][dim];

    std::vector<eigenpair> e;

    for (int j = 0; j < 2; j++) {
        curve_ff->fill_with_jet(dim, oc->at(i + j).components(), 1, F, &JF[0][0], 0);
        curve_aa->fill_with_jet(dim, oc->at(i + j).components(), 1, G, &JG[0][0], 0);

        e.clear();
        Eigen::eig(dim, &JF[0][0], &JG[0][0], e);

        if (characteristic_where == CHARACTERISTIC_ON_CURVE) {
            if (fabs(e[family].i) > epsilon) return false;
            segment_lambda.component(j) = e[family].r;
        }
        //        else {
        //
        //        }
        for (int k = 0; k < dim; k++) {
            segment_flux(k, j) = F[k];
            segment_accum(k, j) = G[k];
        }

    }

    return true;
}

// Simplified version
//
void Extension_Curve::curve(const FluxFunction *f, const AccumulationFunction *a,
                            GridValues &g, int where_is_characteristic,
                            bool is_singular, int fam,
                            std::vector<RealVector> &original_curve,
                            std::vector<RealVector> &extension_on_curve,
                            std::vector<RealVector> &extension_on_domain) {

    curve(f, a, f, a, g, where_is_characteristic, is_singular, fam, 
          original_curve, extension_on_curve, extension_on_domain);

    return;
}

// Generic version
//
void Extension_Curve::curve(const FluxFunction *df, const AccumulationFunction *da, // Over the domain
                            const FluxFunction *cf, const AccumulationFunction *ca, // Over the curve 
                            GridValues &g, int where_is_characteristic,
                            bool is_singular, int fam,  
                            std::vector<RealVector> &original_curve,
                            std::vector<RealVector> &extension_on_curve,
                            std::vector<RealVector> &extension_on_domain){

    domain_ff = df;
    domain_aa = da;

    curve_ff = cf;
    curve_aa = ca;

    family = fam;

    characteristic_where = where_is_characteristic;
    singular = is_singular;

    gv = &g;
    oc = &original_curve;

//    std:://cout << "Dimension of the curve: " << oc->at(0).size() << std::endl;
//    std:://cout << "Characteristic: " << characteristic_where << std::endl;

    gv->fill_eigenpairs_on_grid(domain_ff, domain_aa);

    extension_on_curve.clear();
    extension_on_domain.clear();

    if (gv->grid(0, 0).size() == 2) type_of_physic = &species_physic;
    else                            type_of_physic = &compositional_physic;

    //std:://cout << "Inside Extension Curve: gv->grid(0, 0).size() = " << gv->grid(0, 0).size() << std::endl;

    Contour2p5_Method::contour2p5(this, extension_on_curve, extension_on_domain);

    return;
}

// ON SUBDOMAINS //

void Extension_Curve::curve_out_of_subdomain(const FluxFunction *f, const AccumulationFunction *a,
                                             GridValues &g, std::vector<RealVector> &polygon,
                                             int where_is_characteristic,
                                             bool is_singular, int fam,  
                                             std::vector<RealVector> &original_curve,
                                             std::vector<RealVector> &extension_on_curve,
                                             std::vector<RealVector> &extension_on_domain){

    curve_out_of_subdomain(f, a, f, a, g, polygon, where_is_characteristic, is_singular, fam, 
                           original_curve, extension_on_curve, extension_on_domain);
    return;
}

// Generic version
void Extension_Curve::curve_out_of_subdomain(const FluxFunction *df, const AccumulationFunction *da, // Over the domain
                                             const FluxFunction *cf, const AccumulationFunction *ca, // Over the curve 
                                             GridValues &g, std::vector<RealVector> &polygon,
                                             int where_is_characteristic,
                                             bool is_singular, int fam,  
                                             std::vector<RealVector> &original_curve,
                                             std::vector<RealVector> &extension_on_curve,
                                             std::vector<RealVector> &extension_on_domain){

    domain_ff = df;
    domain_aa = da;

    curve_ff = cf;
    curve_aa = ca;

    family = fam;

    characteristic_where = where_is_characteristic;
    singular = is_singular;

    gv = &g;
    oc = &original_curve;

    gv->fill_eigenpairs_on_grid(domain_ff, domain_aa);

    //  Find the convex hull of the polygon.
    //
    

    std::vector<RealVector> convex_hull_points;
    
    for (int i = 0; i < polygon.size(); i++) {
        //cout<<polygon[i]<<endl;
    }

    
    convex_hull(polygon, convex_hull_points);

    // Copy of the cells, later it will be used to restore it.
    Matrix<int> cell_type_copy;

    if (convex_hull_points.size() > 2){
        cell_type_copy = g.cell_type;

        // Mark the cells that are not completely contained in the polygon as invalid.
        // After the extension is computed, all the cells will be restored.
        // TODO: Use a list of the cells to restore, instead of restoring all cells.
        //
        for (int i = 0; i < g.cell_type.rows(); i++){
            for (int j = 0; j < g.cell_type.cols(); j++){
                if (inside_convex_polygon(convex_hull_points, g.grid(i,     j)) ||
                    inside_convex_polygon(convex_hull_points, g.grid(i + 1, j)) ||
                    inside_convex_polygon(convex_hull_points, g.grid(i,     j + 1)) ||
                    inside_convex_polygon(convex_hull_points, g.grid(i + 1, j + 1))
                   ) g.cell_type(i, j) = CELL_IS_INVALID;
            }
        }
    }

    // Continue as usual.
    //
    extension_on_curve.clear();
    extension_on_domain.clear();

    if (gv->grid(0, 0).size() == 2) type_of_physic = &species_physic;
    else                            type_of_physic = &compositional_physic;

    //std:://cout << "Inside Extension Curve: gv->grid(0, 0).size() = " << gv->grid(0, 0).size() << std::endl;

    Contour2p5_Method::contour2p5(this, extension_on_curve, extension_on_domain);

    if (convex_hull_points.size() > 2){
        // Restore the grid.
        g.cell_type = cell_type_copy;

        // Return the convex hull
        polygon = convex_hull_points;
    }

    return;
}


void Extension_Curve::curve_in_subdomain(const FluxFunction *f, const AccumulationFunction *a,
                                         GridValues &g, std::vector<RealVector> &polygon,
                                         int where_is_characteristic,
                                         bool is_singular, int fam,  
                                         std::vector<RealVector> &original_curve,
                                         std::vector<RealVector> &extension_on_curve,
                                         std::vector<RealVector> &extension_on_domain){

    curve_in_subdomain(f, a, f, a, g, polygon, where_is_characteristic, is_singular, fam, 
                           original_curve, extension_on_curve, extension_on_domain);
    return;
}

// Generic version
void Extension_Curve::curve_in_subdomain(const FluxFunction *df, const AccumulationFunction *da, // Over the domain
                                         const FluxFunction *cf, const AccumulationFunction *ca, // Over the curve 
                                         GridValues &g, std::vector<RealVector> &polygon,
                                         int where_is_characteristic,
                                         bool is_singular, int fam,  
                                         std::vector<RealVector> &original_curve,
                                         std::vector<RealVector> &extension_on_curve,
                                         std::vector<RealVector> &extension_on_domain){

    domain_ff = df;
    domain_aa = da;

    curve_ff = cf;
    curve_aa = ca;

    family = fam;

    characteristic_where = where_is_characteristic;
    singular = is_singular;

    gv = &g;
    oc = &original_curve;

    gv->fill_eigenpairs_on_grid(domain_ff, domain_aa);

    //  Find the convex hull of the polygon.
    //
    std::vector<RealVector> convex_hull_points;
    

    
    for (int i = 0; i < polygon.size(); i++) {
        //cout<<polygon[i]<<endl;
    }
    
    
    convex_hull(polygon, convex_hull_points);

    // Copy of the cells, later it will be used to restore it.
    Matrix<int> cell_type_copy;

    if (convex_hull_points.size() > 2){
        cell_type_copy = g.cell_type;

        // Mark the cells that are not completely contained in the polygon as invalid.
        // After the extension is computed, all the cells will be restored.
        // TODO: Use a list of the cells to restore, instead of restoring all cells.
        //
        for (int i = 0; i < g.cell_type.rows(); i++){
            for (int j = 0; j < g.cell_type.cols(); j++){
                if (!inside_convex_polygon(convex_hull_points, g.grid(i,     j)) ||
                    !inside_convex_polygon(convex_hull_points, g.grid(i + 1, j)) ||
                    !inside_convex_polygon(convex_hull_points, g.grid(i,     j + 1)) ||
                    !inside_convex_polygon(convex_hull_points, g.grid(i + 1, j + 1))
                   ) g.cell_type(i, j) = CELL_IS_INVALID;
            }
        }
    }

    // Continue as usual.
    //
    extension_on_curve.clear();
    extension_on_domain.clear();

    if (gv->grid(0, 0).size() == 2) type_of_physic = &species_physic;
    else                            type_of_physic = &compositional_physic;

    //std:://cout << "Inside Extension Curve: gv->grid(0, 0).size() = " << gv->grid(0, 0).size() << std::endl;

    Contour2p5_Method::contour2p5(this, extension_on_curve, extension_on_domain);

    if (convex_hull_points.size() > 2){
        // Restore the grid.
        g.cell_type = cell_type_copy;

        // Return the convex hull
        polygon = convex_hull_points;
    }

    return;
}

