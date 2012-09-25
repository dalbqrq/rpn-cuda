#ifndef _WAVECURVE_
#define _WAVECURVE_

#include <vector>

#include "eigen.h"

#include "Rarefaction.h"
#include "Shock.h"
#include "CompositeCurve.h"

#include "Boundary.h"
#include "FluxFunction.h"
#include "AccumulationFunction.h"

#include "HyperOctree.h"

#define RAREFACTION_CURVE 1
#define SHOCK_CURVE       2
#define COMPOSITE_CURVE   3

#define WAVE_CURVE_REACHED_BOUNDARY                             10
#define WAVE_CURVE_SHOCK_RIGHT_CHARACTERISTIC_WITH_OTHER_FAMILY 11

#define WAVE_CURVE_OK                                           100
#define WAVE_CURVE_ERROR                                        200

#define WAVE_CURVE_INTERSECTION_FOUND                           300
#define WAVE_CURVE_INTERSECTION_NOT_FOUND                       400

// Struct 
//
//
struct Curve {
    public:
        std::vector<RealVector> curve;
        std::vector<int> corresponding_point_in_related_curve;
        std::vector<int> related_curve;

        int type;
        int index_related_curve;

//        RealVector reference_point;

        bool initial_subcurve;

        int increase;

        Curve(const std::vector<RealVector> &orig, 
              const std::vector<int> &curve_corresponding, const std::vector<int> &orig_corresponding, 
              int t, int index, bool isc){

            curve.resize(orig.size());
            related_curve.resize(orig_corresponding.size());
            corresponding_point_in_related_curve.resize(orig_corresponding.size());

            for (int i = 0; i < orig.size(); i++){
                curve[i].resize(orig[i].size());
                for (int j = 0; j < orig[i].size(); j++) curve[i].component(j) = orig[i].component(j);

                related_curve[i] = curve_corresponding[i];
                corresponding_point_in_related_curve[i] = orig_corresponding[i];
            }

            type = t;

            index_related_curve = index;

            initial_subcurve = isc;
        }

        Curve(const std::vector<RealVector> &orig, const std::vector<int> &orig_corresponding, int t, int index){

            curve.resize(orig.size());
            corresponding_point_in_related_curve.resize(orig_corresponding.size());

            for (int i = 0; i < orig.size(); i++){
                curve[i].resize(orig[i].size());
                for (int j = 0; j < orig[i].size(); j++) curve[i].component(j) = orig[i].component(j);

                corresponding_point_in_related_curve[i] = orig_corresponding[i];
            }

            type = t;

            index_related_curve = index;
        }

        ~Curve(){
            curve.clear();
            corresponding_point_in_related_curve.clear();
        }
};

class WaveCurveSegment {
    private:
    protected:
        static bool point_inside_box(const RealVector &p, const BoxND &box){
            bool test = true;

            int pos = 0;
            while (test && pos < 2){
                test = p.component(pos) >= box.pmin(pos) && p.component(pos) <= box.pmax(pos);
                pos++;
            }

            return test;
        }
    public:
        RealVector *rv;

        int curve_position, segment_position;

        WaveCurveSegment(const RealVector &p1, const RealVector &p2, int cp, int sp){
            rv = new RealVector[2];

            rv[0] = p1;
            rv[1] = p2;

            curve_position   = cp;
            segment_position = sp;
        }
        ~WaveCurveSegment(){
            delete [] rv;
        }

        bool intersect(const BoxND &box){
            return point_inside_box(rv[0], box) || point_inside_box(rv[1], box);
        }
};

class WaveCurve {
    private:
    protected:
        const FluxFunction *ff;
        const AccumulationFunction *aa;
        const Boundary *boundary;

        
        //int half_wavecurve(int initial_curve, const RealVector &init, const RealVector &ref, bool first_curve, int family, int increase, std::vector<Curve> &c);
        double ddot(int n, double *x, double *y);

        static bool segment_intersection(double *p1, double *p2, double *q1, double *q2, double *r);
    public:
        WaveCurve(const FluxFunction *f, const AccumulationFunction *a, const Boundary *b);
        ~WaveCurve();
    int half_wavecurve(int initial_curve, const RealVector &init, int family, int increase, std::vector<Curve> &c);
        int wavecurve(const RealVector &init, int family, int increase, std::vector<Curve> &c);

        static int intersection(const std::vector<Curve> &c1, const std::vector<Curve> &c2, const RealVector &pmin, const RealVector &pmax, 
                                 RealVector &p, int &subc1, int &subc1_point, int &subc2, int &subc2_point);
};

#endif // _WAVECURVE_

