/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) ShockContinuationMethod3D2D.h
 */

#ifndef _ShockContinuationMethod3D2D_H
#define _ShockContinuationMethod3D2D_H

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "ShockMethod.h"
#include "FluxFunction.h"
#include "AccumulationFunction.h"
#include "Boundary.h"
#include "eigen.h"
#include "ShockMethod.h"

/*
 * ---------------------------------------------------------------
 * Definitions:
 */
#define _SHOCK_SIMPLE_ACCUMULATION_  10  // Traditional rarefaction, using dgeev.
#define _SHOCK_GENERAL_ACCUMULATION_ 11  // Rarefaction with generalized eigenpairs, using dggev.

#define _SHOCK_INIT_IS_REF_     20
#define _SHOCK_INIT_IS_NOT_REF_ 21

struct Plane {
public:
    std::vector<RealVector> vec; // Array of vectors
    RealVector point; // Point

    Plane(int n) {
        vec.resize(n - 1);
        for (int i = 0; i < n - 1; i++) vec[i].resize(n);
        point.resize(n);
    }

    ~Plane() {
        vec.clear();
    }
};

extern "C" {
    void dgesv_(int*, int*, double*, int*, int*, double*, int*, int*);
}

class ShockContinuationMethod3D2D:public ShockMethod {
private:


private:

    FluxFunction *shock_flux_object;
    AccumulationFunction *shock_accumulation_object;

    Boundary *boundary;

    int family_;
    int n; // Dimension
    double eps; // Epsilon, typically = 1e-3
    int type;
    double tolerance;
    double delta; // Maximum distance

    double *Uref; // Uref is the same as the initial point
    double *p_outside; // Delete later
    double *v1_outside, *v2_outside;

    

   

    void vectprod(double *a, double *b, double *c);
    double dotprod(int n, double x[], double y[]);
    void normalize(int n, double x[]);
    double euclidean_norm(int n, double x[]);

    double shockspeed(int n, int family, double Um[], double Up[]);

    int plane_start(int family, double Upr[], double Upl[], double v1[], double v2[], double epsilon);
    void fill_with_jet(const RpFunction & flux_object, int n, double *in, int degree, double *F, double *J, double *H);

    void jet_H1(double *p, double &H1, double *nabla_H1);
    void jet_H2(double *p, double &H2, double *nabla_H2);

    void plane_mapping(double plane_point[], double a1, double a2, double v1[], double v2[], double p[]);
    void jet_N(double plane_point[], double a1, double a2, double v1[], double v2[], double N[], double *DN);

    int solver(int n, double *A, double *b, double *x);
    void Newton_plane(double plane_point[], double v1[], double v2[], double *pnew);

    void print_matrix(const char *name, int m, int n, double *A);

    int init(int family, int increase, Plane &plane, RealVector &refvec, double &rebounds_first_step, double &epsilon_start);
    int init(const Plane &init_plane, const RealVector &init_refvec, Plane &plane, RealVector &refvec, double &rebounds_first_step, double &epsilon_start);
    int curve(int family, double maxnum, int increase, std::vector<RealVector> &out, int &edge); // If _SHOCK_INIT_IS_REF_

public:


    ShockContinuationMethod3D2D(int dim,int family, const FluxFunction &, const AccumulationFunction &, const Boundary &, double Ur[], double tol, double epsilon, int t);
    virtual ~ShockContinuationMethod3D2D();

    ShockContinuationMethod3D2D(const ShockContinuationMethod3D2D &);

    ShockMethod * clone() const ;


    void curve(const RealVector &, int direction, vector<RealVector> &);



    //int curve(int family, double maxnum, int increase, double U0[], double refplane[], std::vector<RealVector> &out); // If _SHOCK_INIT_IS_NOT_REF_

    int plane(double Uprevious[], double direction[], double vec_previous1[], double vec_previous2[], double plane_point[], double v1[], double v2[], double epsilon);

    void set_outside(double *p, double *v1, double *v2) {
        p_outside = p;
        v1_outside = v1;
        v2_outside = v2;
    } // Delete later











};

#endif //! _ShockContinuationMethod3D2D_H
