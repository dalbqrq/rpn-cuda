#include <FL/Fl.H>
#include <FL/x.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Float_Input.H>
#include <FL/fl_ask.H>

#include "canvas.h"
#include "curve2d.h"
#include "segmentedcurve.h"
#include "MultiColoredCurve.h"

#include "Rarefaction.h"
#include "Thermodynamics.h"
#include "Hugoniot_TP.h"
#include "RectBoundary.h"
#include "SinglePhaseBoundary.h"

#include "FluxSinglePhaseVaporAdimensionalized_Params.h"
#include "FluxSinglePhaseVaporAdimensionalized.h"

#include "AccumulationSinglePhaseVaporAdimensionalized_Params.h"
#include "AccumulationSinglePhaseVaporAdimensionalized.h"

#include "Flux2Comp2PhasesAdimensionalized.h"
#include "Accum2Comp2PhasesAdimensionalized.h"

#include "FluxSinglePhaseLiquidAdimensionalized_Params.h"
#include "FluxSinglePhaseLiquidAdimensionalized.h"

#include "AccumulationSinglePhaseLiquidAdimensionalized_Params.h"
#include "AccumulationSinglePhaseLiquidAdimensionalized.h"

#include "ReferencePoint.h"
#include "ViscosityJetMatrix.h"
#include "ColorCurve.h"

// Input here...
Fl_Double_Window   *canvaswin = (Fl_Double_Window*)0;
    Canvas       *canvas_tpcw = (Canvas*)0;
    Canvas      *canvas_vapor = (Canvas*)0;
    Canvas     *canvas_liquid = (Canvas*)0;

Flux2Comp2PhasesAdimensionalized  *tpcw_flux;
Accum2Comp2PhasesAdimensionalized *tpcw_accum;
RectBoundary                      *tpcw_boundary;
GridValues                        *tpcw_grid;

FluxSinglePhaseVaporAdimensionalized         *vapor_flux;
AccumulationSinglePhaseVaporAdimensionalized *vapor_accum;
SinglePhaseBoundary                          *vapor_boundary;
GridValues                                   *vapor_grid;

FluxSinglePhaseLiquidAdimensionalized         *liquid_flux;
AccumulationSinglePhaseLiquidAdimensionalized *liquid_accum;
SinglePhaseBoundary                           *liquid_boundary;
GridValues                                    *liquid_grid;

// Common
FluxFunction *fref;
AccumulationFunction *aref;

RealVector ref(3);

void clickcb(Fl_Widget *w, void*){
    Canvas *c = (Canvas*)w;
    double x, y;

    c->getxy(ref.component(0), ref.component(1));

    ref.component(2) = 1.0;

    std::cout << "Ref.: " << ref << std::endl;

    // These two below will be used by the rarefaction
    const Boundary *rar_boundary;
    Canvas *rar_canvas;

    if (c == canvas_vapor){
        fref = vapor_flux;
        aref = vapor_accum;

        rar_boundary = vapor_boundary;
        rar_canvas = canvas_vapor;
    }
    else if (c == canvas_tpcw){
        fref = tpcw_flux;
        aref = tpcw_accum;

        rar_boundary = tpcw_boundary;
        rar_canvas = canvas_tpcw;
    }
    else{
        fref = liquid_flux;
        aref = liquid_accum;

        rar_boundary = liquid_boundary;
        rar_canvas = canvas_liquid;
    }

    // Reference point
    Viscosity_Matrix v;
    ReferencePoint referencepoint(ref, fref, aref, &v);

    // Rarefaction, only in the domain where the click was received.
    int rar_increase_table[2] = {RAREFACTION_SPEED_INCREASE, RAREFACTION_SPEED_DECREASE};

    for (int family = 0; family < 2; family++){
        for (int increase = 0; increase < 2; increase++){
            double deltaxi = 1e-3;
            std::vector<RealVector> rarcurve;
            std::vector<RealVector> inflection_points;

            Rarefaction::curve(ref, 
                               RAREFACTION_INITIALIZE_YES, // int initialize,
                               0, //  const RealVector *initial_direction,
                               family, 
                               rar_increase_table[increase],
                               RAREFACTION_FOR_ITSELF, // int type_of_rarefaction,
                               deltaxi,
                               fref, aref,
                               RAREFACTION_GENERAL_ACCUMULATION, // int type_of_accumulation,
                               rar_boundary,
                               rarcurve,
                               inflection_points);

            Curve2D *rarcurve_for_canvas = new Curve2D(rarcurve, 238.0/255.0, 210.0/255.0, 238.0/255.0, CURVE2D_MARKERS);
            rar_canvas->add(rarcurve_for_canvas);
        }
    }

    // Compute the Hugoniot curves
    //
    Hugoniot_TP htp;

    std::vector<FluxFunction*> flux(3);
    flux[0] = vapor_flux;
    flux[1] = tpcw_flux;
    flux[2] = liquid_flux;

    std::vector<AccumulationFunction*> accum(3);
    accum[0] = vapor_accum;
    accum[1] = tpcw_accum;
    accum[2] = liquid_accum;

    std::vector<Canvas*> canvas(3);
    canvas[0] = canvas_vapor;
    canvas[1] = canvas_tpcw;
    canvas[2] = canvas_liquid;

    std::vector<GridValues*> grid(3);
    grid[0] = vapor_grid;
    grid[1] = tpcw_grid;
    grid[2] = liquid_grid;

    for (int k = 0; k < 3; k++){
        std::vector<RealVector> hugoniot_curve;
 
        htp.curve(flux[k], accum[k], 
                  *(grid[k]), 
                  referencepoint, 
                  hugoniot_curve);

        std::cout << "Hugoniot " << k << " has " << hugoniot_curve.size()/2 << " points." << std::endl;

        if (hugoniot_curve.size() > 2){
            std::vector<Point2D> p;
            for (int i = 0; i < hugoniot_curve.size()/2; i++){
                p.push_back(Point2D(hugoniot_curve[2*i].component(0),     hugoniot_curve[2*i].component(1)));
                p.push_back(Point2D(hugoniot_curve[2*i + 1].component(0), hugoniot_curve[2*i + 1].component(1)));
            }

            SegmentedCurve *curve2d = new SegmentedCurve(p, 0, 0, 1);

            // Classified curve
            ColorCurve cc(*(flux[k]), *(accum[k]));    
            std::vector<HugoniotPolyLine> classified_curve;
            std::vector<RealVector> transition_list;

            cc.classify_segmented_curve(hugoniot_curve, referencepoint,
                                        classified_curve,
                                        transition_list);

            // Color
            MultiColoredCurve *mcc = new MultiColoredCurve(classified_curve);
            canvas[k]->add(mcc);
        }
    }

    Fl::check();

    return;
}

int main(){
//    ReferencePoint::print();

    // Thermo
    double mc = 0.044;
    double mw = 0.018;

    MolarDensity mdv(MOLAR_DENSITY_VAPOR,  100.900000e5);
    MolarDensity mdl(MOLAR_DENSITY_LIQUID, 100.900000e5);

    VLE_Flash_TPCW *flash = new VLE_Flash_TPCW(&mdl, &mdv);

    Thermodynamics *tc = new Thermodynamics(mc, mw, "./c++/rpnumerics/physics/tpcw/hsigmaC_spline.txt");
    tc->set_flash(flash);

    // ************************* Physics ************************* //

    // TPCW
    double abs_perm = 3e-12; 
    double sin_beta = 0.0;
    double const_gravity = 9.8;
    bool has_gravity = false;
    bool has_horizontal = true;

    RealVector fpp(12);
    fpp.component(0) = abs_perm;
    fpp.component(1) = sin_beta;
    fpp.component(2) = (double)has_gravity;
    fpp.component(3) = (double)has_horizontal;
    
    fpp.component(4) = 0.0;
    fpp.component(5) = 0.0;
    fpp.component(6) = 2.0;
    fpp.component(7) = 2.0;
    fpp.component(8) = 0.38;
    fpp.component(9) = 304.63;
    fpp.component(10) = 998.2;
    fpp.component(11) = 4.22e-3;

    Flux2Comp2PhasesAdimensionalized_Params fp(fpp, tc);
    tpcw_flux = new Flux2Comp2PhasesAdimensionalized(fp);

    double phi = 0.38;

    Accum2Comp2PhasesAdimensionalized_Params ap(tc, phi);
    tpcw_accum = new Accum2Comp2PhasesAdimensionalized(ap);

    // Vapor
    FluxSinglePhaseVaporAdimensionalized_Params vapor_fluxpar(mc, mw, 0.0, 
                                                    "./c++/rpnumerics/physics/tpcw/hsigmaC_spline.txt",
                                                    0.0, 0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0);

    vapor_flux = new FluxSinglePhaseVaporAdimensionalized(vapor_fluxpar, tc);

    AccumulationSinglePhaseVaporAdimensionalized_Params vapor_accumpar(mc, mw, 0.0, 
                                                    "./c++/rpnumerics/physics/tpcw/hsigmaC_spline.txt",
                                                    0.0, 0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    phi);

    vapor_accum = new AccumulationSinglePhaseVaporAdimensionalized(vapor_accumpar, tc);

    // Liquid
    FluxSinglePhaseLiquidAdimensionalized_Params liquid_fluxpar(mc, mw, 0.0, 
                                                    "./c++/rpnumerics/physics/tpcw/hsigmaC_spline.txt",
                                                    0.0, 0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0);

    liquid_flux = new FluxSinglePhaseLiquidAdimensionalized(liquid_fluxpar, tc);

    AccumulationSinglePhaseLiquidAdimensionalized_Params liquid_accumpar(mc, mw, 0.0, 
                                                    "./c++/rpnumerics/physics/tpcw/hsigmaC_spline.txt",
                                                    0.0, 0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    0.0,
                                                    phi);

    liquid_accum = new AccumulationSinglePhaseLiquidAdimensionalized(liquid_accumpar, tc);
    // ************************* Physics ************************* //


    // ************************* Boundaries ************************* //

    // TPCW
    double Theta_min = 0.099309;
    double Theta_max = 0.576511;

    RealVector pmin(3), pmax(3);

    pmin.component(0) = 0.0;
    pmin.component(1) = Theta_min;
    pmin.component(2) = 0.0;

    pmax.component(0) = 1.0;
    pmax.component(1) = Theta_max;
    pmax.component(2) = 2.0;

    std::vector<int> number_of_cells(2);
    number_of_cells[0] = number_of_cells[1] = 256;

    tpcw_boundary = new RectBoundary(pmin, pmax);

    // Vapor
    vapor_boundary = new SinglePhaseBoundary(flash, Theta_min, Theta_max, DOMAIN_IS_VAPOR, &Thermodynamics::Theta2T);

    // Liquid
    liquid_boundary = new SinglePhaseBoundary(flash, Theta_min, Theta_max, DOMAIN_IS_LIQUID, &Thermodynamics::Theta2T);
    // ************************* Boundaries ************************* //

    // ************************* GridValues ************************* //
    tpcw_grid = new GridValues(tpcw_boundary, pmin, pmax, number_of_cells);

    RealVector vapor_max(pmax);
    vapor_max.component(0) = vapor_boundary->maximums().component(0); // Fix later
    vapor_grid = new GridValues(vapor_boundary, pmin, vapor_max, number_of_cells);

    RealVector liquid_max(pmax);
    liquid_max.component(0) = liquid_boundary->maximums().component(0); // Fix later
    liquid_grid = new GridValues(liquid_boundary, pmin, liquid_max, number_of_cells);
    // ************************* GridValues ************************* //

    // Window
    int main_w  = Fl::w();
    int main_h  = Fl::h();

    canvaswin = new Fl_Double_Window((Fl::w() - main_w)/2, (Fl::h() - main_h)/2, main_w, main_h, "Hugoniot between physics");
    {
        double mirror[9] = {-1.0, 0.0, 0.0, 
                             0.0, 1.0, 0.0,
                             0.0, 0.0, 1.0};

        // vapor to the left
        canvas_vapor = new Canvas(10, 10, (main_w - 20)/3, main_h - 20);
        canvas_vapor->xlabel("yw");
        canvas_vapor->ylabel("Theta");
        canvas_vapor->setextfunc(&clickcb, canvas_vapor, 0);

        // TPCW at the middle
        canvas_tpcw = new Canvas(canvas_vapor->x() + canvas_vapor->w(), canvas_vapor->y(), canvas_vapor->w(), canvas_vapor->h());
        canvas_tpcw->xlabel("s");
        canvas_tpcw->ylabel("Theta");
        canvas_tpcw->setextfunc(&clickcb, canvas_tpcw, 0);
        canvas_tpcw->set_transform_matrix(mirror);

        // liquid to the right, mirrored.
        canvas_liquid = new Canvas(canvas_tpcw->x() + canvas_tpcw->w(), canvas_vapor->y(), canvas_vapor->w(), canvas_vapor->h());
        canvas_liquid->xlabel("xc");
        canvas_liquid->ylabel("Theta");
        canvas_liquid->setextfunc(&clickcb, canvas_liquid, 0);
        canvas_liquid->set_transform_matrix(mirror);
        //canvas_liquid->axis(-liquid_boundary->maximums().component(0), 0.0, Theta_min, Theta_max);
    }
    canvaswin->end();
    canvaswin->resizable(canvaswin);

    // ************************* Draw Boundaries ************************* //
    std::vector<RealVector> edges;
    std::vector<Point2D> edges2d;
    Curve2D *b;

    // TPCW
    edges2d.push_back(Point2D(pmin.component(0), pmin.component(1)));
    edges2d.push_back(Point2D(pmax.component(0), pmin.component(1)));
    edges2d.push_back(Point2D(pmax.component(0), pmax.component(1)));
    edges2d.push_back(Point2D(pmin.component(0), pmax.component(1)));
    edges2d.push_back(edges2d[0]);
    b = new Curve2D(edges2d, 0, 0, 0, CURVE2D_SOLID_LINE);

    canvas_tpcw->add(b);
    canvas_tpcw->nozoom();

    // Vapor
    vapor_boundary->physical_boundary(edges);
    edges.push_back(edges[0]);

    b = new Curve2D(edges, 0, 0, 0, CURVE2D_SOLID_LINE);
    canvas_vapor->add(b);
    canvas_vapor->nozoom();

    // Liquid
    liquid_boundary->physical_boundary(edges);
    edges.push_back(edges[0]);

    b = new Curve2D(edges, 0, 0, 0, CURVE2D_SOLID_LINE);
    canvas_liquid->add(b);
    canvas_liquid->nozoom();
    // ************************* Draw Boundaries ************************* //

    canvaswin->show();

    return Fl::run();
}

