#include "Discriminant_Contour.h"

Discriminant_Contour::Discriminant_Contour() : ImplicitFunction() {
}

Discriminant_Contour::~Discriminant_Contour(){
}

double Discriminant_Contour::discriminant(const FluxFunction *f, const AccumulationFunction *a, const RealVector &p){
    JetMatrix F_jet(2), G_jet(2);

    f->jet(p, F_jet, 1);
    a->jet(p, G_jet, 1);

    DoubleMatrix FJ = F_jet.Jacobian();
    DoubleMatrix GJ = G_jet.Jacobian();

    return discriminant(F_jet.Jacobian(), G_jet.Jacobian());
}

double Discriminant_Contour::discriminant(const DoubleMatrix &FJ, const DoubleMatrix &GJ){
    // p(lambda) = a*lambda^2 + b*lambda + c = 0.
    // 
    double a = GJ(0, 0)*GJ(1, 1) - GJ(1, 0)*GJ(0, 1);
    double b = (FJ(1, 0)*GJ(0, 1) + FJ(0, 1)*GJ(1, 0)) - (FJ(0, 0)*GJ(1, 1) + FJ(1, 1)*GJ(0, 0));
    double c = FJ(0, 0)*FJ(1, 1) - FJ(1, 0)*FJ(0, 1);

    return b*b - 4.0*a*c;
}

RealVector Discriminant_Contour::abc(const DoubleMatrix &FJ, const DoubleMatrix &GJ){
    RealVector abc_vec(3);

    // a.
    //
    abc_vec(0) = GJ(0, 0)*GJ(1, 1) - GJ(1, 0)*GJ(0, 1);

    // b.
    //
    abc_vec(1) = (FJ(1, 0)*GJ(0, 1) + FJ(0, 1)*GJ(1, 0)) - (FJ(0, 0)*GJ(1, 1) + FJ(1, 1)*GJ(0, 0));

    // c.
    //
    abc_vec(2) = FJ(0, 0)*FJ(1, 1) - FJ(1, 0)*FJ(0, 1);   

    return abc_vec;
}

double Discriminant_Contour::derivative_discriminant(const FluxFunction *flux, const AccumulationFunction *accum, const RealVector &p, int u){
    JetMatrix F_jet(2), G_jet(2);

    flux->jet(p, F_jet, 2);
    accum->jet(p, G_jet, 2);   

    DoubleMatrix FJ = F_jet.Jacobian();
    DoubleMatrix GJ = G_jet.Jacobian();

    RealVector abc_vec = abc(FJ, GJ);
    double a = abc_vec(0);
    double b = abc_vec(1);
    double c = abc_vec(2);

    double da_du =  G_jet.get(0, 0, u)*G_jet.get(1, 1) + G_jet.get(0, 0)*G_jet.get(1, 1, u) - 
                   (G_jet.get(1, 0, u)*G_jet.get(0, 1) + G_jet.get(1, 0)*G_jet.get(0, 1, u));

    double db_du =  F_jet.get(1, 0, u)*G_jet.get(0, 1) + F_jet.get(1, 0)*G_jet.get(0, 1, u) + 
                    F_jet.get(0, 1, u)*G_jet.get(1, 0) + F_jet.get(0, 1)*G_jet.get(1, 0, u) -
                   (F_jet.get(0, 0, u)*G_jet.get(1, 1) + F_jet.get(0, 0)*G_jet.get(1, 1, u) + 
                    F_jet.get(1, 1, u)*G_jet.get(0, 0) + F_jet.get(1, 1)*G_jet.get(0, 0, u));

    double dc_du =  F_jet.get(0, 0, u)*F_jet.get(1, 1) + F_jet.get(0, 0)*F_jet.get(1, 1, u) - 
                   (F_jet.get(1, 0, u)*F_jet.get(0, 1) + F_jet.get(1, 0)*F_jet.get(0, 1, u));

    return 2.0*b*db_du - 4.0*(a*dc_du + da_du*c); 
}

int Discriminant_Contour::function_on_square(double *foncub, int i, int j){
    return (*fos)(this, foncub, i, j);
}

int Discriminant_Contour::discriminant_function_on_square(Discriminant_Contour *obj, double *foncub, int i, int j){
    int is_square = obj->gv->cell_type(i, j);

    GridValues *gv = obj->gv;
    double level = obj->level;

    foncub[0] = obj->discriminant(gv->JF_on_grid(i + 1, j + 0), gv->JG_on_grid(i + 1, j + 0)) - level;

    foncub[1] = obj->discriminant(gv->JF_on_grid(i + 0, j + 0), gv->JG_on_grid(i + 0, j + 0)) - level;

    foncub[3] = obj->discriminant(gv->JF_on_grid(i + 0, j + 1), gv->JG_on_grid(i + 0, j + 1)) - level;

    if (is_square == CELL_IS_SQUARE){
        foncub[2] = obj->discriminant(gv->JF_on_grid(i + 1, j + 1), gv->JG_on_grid(i + 1, j + 1)) - level;
    }

    return 1;
}

double Discriminant_Contour::max_discriminant(Discriminant_Contour *obj, int i, int j){
    int is_square = obj->gv->cell_type(i, j);

    GridValues *gv = obj->gv;

    double max_disc = 0.0;

    max_disc = std::max(max_disc, obj->discriminant(gv->JF_on_grid(i + 1, j + 0), gv->JG_on_grid(i + 1, j + 0)));

    max_disc = std::max(max_disc, obj->discriminant(gv->JF_on_grid(i + 0, j + 0), gv->JG_on_grid(i + 0, j + 0)));

    max_disc = std::max(max_disc, obj->discriminant(gv->JF_on_grid(i + 0, j + 1), gv->JG_on_grid(i + 0, j + 1)));

    if (is_square == CELL_IS_SQUARE){
        max_disc = std::max(max_disc, obj->discriminant(gv->JF_on_grid(i + 1, j + 1), gv->JG_on_grid(i + 1, j + 1)));
    }

    return max_disc;
}

// Replace the following method by a more robust one, based on finding the minimum of the discriminant
// at each edge of the triangles of the square. Use William Brent's code.
//
int Discriminant_Contour::derivative_discriminant_function_on_square(Discriminant_Contour *obj, double *foncub, int i, int j){
    int is_square = obj->gv->cell_type(i, j);

    GridValues *gv = obj->gv;
    const FluxFunction *ff = obj->ff;
    const AccumulationFunction *aa = obj->aa;
    int u = obj->index_derivative_discriminant_variable;

//    double min_der_disc;

    foncub[0] = obj->derivative_discriminant(ff, aa, gv->grid(i + 1, j + 0), u);
//    min_der_disc = std::abs(foncub[0]);

    foncub[1] = obj->derivative_discriminant(ff, aa, gv->grid(i + 0, j + 0), u);
//    min_der_disc = std::min(min_der_disc, std::abs(foncub[1]));

    foncub[3] = obj->derivative_discriminant(ff, aa, gv->grid(i + 0, j + 1), u);
//    min_der_disc = std::min(min_der_disc, std::abs(foncub[3]));

    if (is_square == CELL_IS_SQUARE){
        foncub[2] = obj->derivative_discriminant(ff, aa, gv->grid(i + 1, j + 1), u);
//        min_der_disc = std::min(min_der_disc, std::abs(foncub[2]));
    }

//    // Find the maximum of the discriminant.
//    //
//    double abs_max_disc = max_discriminant(obj, i, j);

//    double du = RealVector(gv->grid(1, 1) - gv->grid(0, 0))(u);

//    if (20.0*du*min_der_disc < abs_max_disc) return 0;
//    else return 1;

    return 1;
}

int Discriminant_Contour::curve(const FluxFunction *f, const AccumulationFunction *a, 
                              GridValues &g, 
                              double lev,
                              std::vector<RealVector> &discriminant_contour){

    ff = f;
    aa = a;

    g.fill_Jacobians_on_grid(f, a);
    gv = &g;

    level = lev;

    fos = &discriminant_function_on_square;
    is_improvable = false;

    discriminant_contour.clear();

    int info = ContourMethod::contour2d(this, discriminant_contour);

    return info;
}

void Discriminant_Contour::curve(const FluxFunction *f, const AccumulationFunction *a, 
                                 GridValues &g, 
                                 const std::vector<double> &level,
                                 std::vector<std::vector<RealVector> > &discriminant_contour){

    discriminant_contour.clear();

    for (int i = 0; i < level.size(); i++){
        std::vector<RealVector> dc;

        curve(f, a, g, level[i], dc);

        discriminant_contour.push_back(dc);
    }

    return;
}

int Discriminant_Contour::curve(const FluxFunction *f, const AccumulationFunction *a, 
                                GridValues &g, 
                                const RealVector &p,
                                std::vector<RealVector> &discriminant_contour, double &lev){

    lev = discriminant(f, a, p);
    int info = curve(f, a, g, lev, discriminant_contour);

    return info;
}

int Discriminant_Contour::derivative_curve(const FluxFunction *f, const AccumulationFunction *a, 
                                           GridValues &g, int u,
                                           std::vector<RealVector> &derivative_discriminant_contour, std::vector<double> &discriminant_on_segment){

    ff = f;
    aa = a;

    g.fill_Jacobians_on_grid(f, a);
    gv = &g;

    fos = &derivative_discriminant_function_on_square;
    is_improvable = true;
    index_derivative_discriminant_variable = u;

    derivative_discriminant_contour.clear();
    discriminant_on_segment.clear();

    int info = ContourMethod::contour2d(this, derivative_discriminant_contour);

    for (int i = 0; i < derivative_discriminant_contour.size(); i++){
        discriminant_on_segment.push_back(discriminant(f, a, derivative_discriminant_contour[i]));
    }

    return info;
}

int Discriminant_Contour::complete(const RealVector &p0, const RealVector &p1, const RealVector &p_init, RealVector &p_completed){

    double t = 1e-10;
    double flip = 1.0;

    double disc = localm_(p0, p1, flip, t, p_completed);
//    std::cout << "Disc @" << p_completed << " = " << disc << std::endl;

//    p_completed = p_init;

    // TODO:
    // The tolerance below must be adjusted, maybe as a user-given parameter.
    //
    if (disc < 1e-8) return IMPROVABLE_OK;
    else             return IMPROVABLE_ERROR;
}

double Discriminant_Contour::localm_(const RealVector &xa, const RealVector &xb, double flip, double t, RealVector &xn){
    /* Initialized data */

    double eps = (double)1e-13;

    /* System generated locals */
    double ret_val/*, r__1, r__2*/;

    /* Local variables */
    double d__, e, m, p, q, r__, u, v, w, x, t2, sa, sb, fu, fv, fw, fx, tol;

/* #ifdef DECLARE */
/* #endif */
/*     this routine finds the local minimum of the function  f  in */
/*     the interval (a, b) for flip= 1.0, maximum for flip = -1.0. */
/*     r. p. brent, algorithms for minimization without derivatives, */
/*     prentice-hall, 1973. */

    /* Function Body */
    sa = 0.;
    sb = 1.;
    x = sa + (sb - sa)*.381966;
    w = x;
    v = w;
    e = 0.;
//    r__1 = xa(0) + x*(xb(0) - xa(0));
//    r__2 = xa(1) + x*(xb(1) - xa(1));

    fx = flip*discriminant(ff, aa, xa + x*(xb - xa));

//    fx = flip*(*f)(&r__1, &r__2);
    fw = fx;
    fv = fw;
L10:
    m = (sa + sb)*.5;
    tol = eps*std::abs(x) + t;
    t2 = tol*2.;

    if (std::abs(x - m) <= t2 - (sb - sa)*.5) {
	goto L190;
    }
    r__ = 0.;
    q = r__;
    p = q;
    if (std::abs(e) <= tol) {
	goto L40;
    }
    r__ = (x - w) * (fx - fv);
    q = (x - v) * (fx - fw);
    p = (x - v) * q - (x - w) * r__;
    q = (q - r__)*2.;
    if (q <= 0.) {
	goto L20;
    }
    p = -p;
    goto L30;
L20:
    q = -q;
L30:
    r__ = e;
    e = d__;
L40:
    if (std::abs(p) >= std::abs(q*.5*r__)) {
	goto L60;
    }
    if (p <= q * (sa - x) || p >= q * (sb - x)) {
	goto L60;
    }
    d__ = p / q;
    u = x + d__;
    if (u - sa >= t2 && sb - u >= t2) {
	goto L90;
    }
    if (x >= m) {
	goto L50;
    }
    d__ = tol;
    goto L90;
L50:
    d__ = -tol;
    goto L90;
L60:
    if (x >= m) {
	goto L70;
    }
    e = sb - x;
    goto L80;
L70:
    e = sa - x;
L80:
    d__ = e*.381966;
L90:
    if (std::abs(d__) < tol) {
	goto L100;
    }
    u = x + d__;
    goto L120;
L100:
    if (d__ <= 0.) {
	goto L110;
    }
    u = x + tol;
    goto L120;
L110:
    u = x - tol;
L120:
//    r__1 = xa[1] + u * (xb[1] - xa[1]);
//    r__2 = xa[2] + u * (xb[2] - xa[2]);
    fu = flip*discriminant(ff, aa, xa + u*(xb - xa));
    if (fu > fx) {
	goto L150;
    }
    if (u >= x) {
	goto L130;
    }
    sb = x;
    goto L140;
L130:
    sa = x;
L140:
    v = w;
    fv = fw;
    w = x;
    fw = fx;
    x = u;
    fx = fu;
    goto L10;
L150:
    if (u >= x) {
	goto L160;
    }
    sa = u;
    goto L170;
L160:
    sb = u;
L170:
    if (fu > fw && w != x) {
	goto L180;
    }
    v = w;
    fv = fw;
    w = u;
    fw = fu;
    goto L10;
L180:
    if (fu > fv && v != x && v != w) {
	goto L10;
    }
    v = u;
    fv = fu;
    goto L10;
L190:
    ret_val = flip*fx;
//    xn[1] = xa[1] + x * (xb[1] - xa[1]);
//    xn[2] = xa[2] + x * (xb[2] - xa[2]);
    xn = xa + x*(xb - xa);

    return ret_val;
} /* localm_ */

