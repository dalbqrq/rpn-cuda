/**
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) JNIHugoniotCurveCalc.cc
 **/


//! Definition of JNIHugoniotCurveCalc
/*!
	
TODO:
	
NOTE : 

@ingroup JNI
 */


#include "rpnumerics_BoundaryExtensionCurveCalc.h"

#include "JNIDefs.h"
#include "RpNumerics.h"
#include <vector>
#include <iostream>
#include "TPCW.h"
#include "Extension_Curve.h"
#include "Boundary_ExtensionTPCW.h"
#include "Boundary_ExtensionStone.h"




using std::vector;
using namespace std;

JNIEXPORT jobject JNICALL Java_rpnumerics_BoundaryExtensionCurveCalc_nativeCalc
(JNIEnv * env, jobject obj, jint edgeResolution, jint domainFamily, jint edge, jint characteristicWhere) {



    jclass realVectorClass = env->FindClass(REALVECTOR_LOCATION);

    jclass realSegmentClass = env->FindClass(REALSEGMENT_LOCATION);

    jclass arrayListClass = env->FindClass("java/util/ArrayList");

    jclass extensionCurveClass = env->FindClass(BOUNDARYEXTENSIONCURVE_LOCATION);

    jmethodID realVectorConstructorDoubleArray = env->GetMethodID(realVectorClass, "<init>", "([D)V");

    jmethodID realSegmentConstructor = (env)->GetMethodID(realSegmentClass, "<init>", "(Lwave/util/RealVector;Lwave/util/RealVector;)V");


    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");
    jmethodID extensionCurveConstructor = env->GetMethodID(extensionCurveClass, "<init>", "(Ljava/util/List;Ljava/util/List;)V");

    //Input processing

    //Calculations using the input

    jobject leftSegmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);

    jobject rightSegmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);

    // Storage space for the segments:

    vector<RealVector> curve_segments;
    vector<RealVector> domain_segments;

    int dimension = RpNumerics::getPhysics().domain().dim();

    Boundary * physicsBoundary = (Boundary *) & RpNumerics::getPhysics().boundary();

    const FluxFunction * flux = &RpNumerics::getPhysics().fluxFunction();
    const AccumulationFunction * accum = &RpNumerics::getPhysics().accumulation();

    GridValues * gv = RpNumerics::getGridFactory().getGrid("bifurcationcurve");

    physicsBoundary->extension_curve(flux, accum, *gv, edge, edgeResolution, true, domainFamily, characteristicWhere, curve_segments, domain_segments);


    if (curve_segments.size() == 0)return NULL;


    for (unsigned int i = 0; i < curve_segments.size() / 2; i++) {

        jdoubleArray eigenValRLeft = env->NewDoubleArray(dimension);
        jdoubleArray eigenValRRight = env->NewDoubleArray(dimension);

        RpNumerics::getPhysics().getSubPhysics(0).postProcess(curve_segments.at(2 * i));

        RpNumerics::getPhysics().getSubPhysics(0).postProcess(curve_segments.at(2 * i + 1));


        double * leftCoords = (double *) curve_segments.at(2 * i);
        double * rightCoords = (double *) curve_segments.at(2 * i + 1);

        env->SetDoubleArrayRegion(eigenValRLeft, 0, dimension, leftCoords);
        env->SetDoubleArrayRegion(eigenValRRight, 0, dimension, rightCoords);


        //Construindo left e right points
        jobject realVectorLeftPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRLeft);

        jobject realVectorRightPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRRight);

        jobject realSegment = env->NewObject(realSegmentClass, realSegmentConstructor, realVectorLeftPoint, realVectorRightPoint);
        env->CallObjectMethod(leftSegmentsArray, arrayListAddMethod, realSegment);

    }

    for (unsigned int i = 0; i < domain_segments.size() / 2; i++) {

        jdoubleArray eigenValRLeft = env->NewDoubleArray(dimension);
        jdoubleArray eigenValRRight = env->NewDoubleArray(dimension);

        RpNumerics::getPhysics().getSubPhysics(0).postProcess(domain_segments.at(2 * i));

        RpNumerics::getPhysics().getSubPhysics(0).postProcess(domain_segments.at(2 * i + 1));

        double * leftCoords = (double *) domain_segments.at(2 * i);
        double * rightCoords = (double *) domain_segments.at(2 * i + 1);


        env->SetDoubleArrayRegion(eigenValRLeft, 0, dimension, leftCoords);
        env->SetDoubleArrayRegion(eigenValRRight, 0, dimension, rightCoords);


        //Construindo left e right points
        jobject realVectorLeftPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRLeft);

        jobject realVectorRightPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRRight);



        jobject realSegment = env->NewObject(realSegmentClass, realSegmentConstructor, realVectorLeftPoint, realVectorRightPoint);
        env->CallObjectMethod(rightSegmentsArray, arrayListAddMethod, realSegment);

    }

    jobject result = env->NewObject(extensionCurveClass, extensionCurveConstructor, leftSegmentsArray, rightSegmentsArray);


    return result;


}
















