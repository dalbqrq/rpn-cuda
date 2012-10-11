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


#include "rpnumerics_InflectionCurveCalc.h"
#include "JNIDefs.h"
#include "RpNumerics.h"
#include <vector>
#include <iostream>
#include "Inflection_Curve.h"
#include "TPCW.h"


using std::vector;
using namespace std;

JNIEXPORT jobject JNICALL Java_rpnumerics_InflectionCurveCalc_nativeCalc(JNIEnv * env, jobject obj, jint family, jintArray resolution) {


    jclass realVectorClass = env->FindClass(REALVECTOR_LOCATION);

    jclass realSegmentClass = env->FindClass(REALSEGMENT_LOCATION);

    jclass arrayListClass = env->FindClass("java/util/ArrayList");

    jclass inflectionCurveClass = env->FindClass(INFLECTIONCURVE_LOCATION);



    jmethodID realVectorConstructorDoubleArray = env->GetMethodID(realVectorClass, "<init>", "([D)V");

    jmethodID realSegmentConstructor = (env)->GetMethodID(realSegmentClass, "<init>", "(Lwave/util/RealVector;Lwave/util/RealVector;)V");

    jmethodID arrayListConstructor = env->GetMethodID(arrayListClass, "<init>", "()V");
    jmethodID arrayListAddMethod = env->GetMethodID(arrayListClass, "add", "(Ljava/lang/Object;)Z");

    jmethodID inflectionCurveConstructor = env->GetMethodID(inflectionCurveClass, "<init>", "(Ljava/util/List;)V");


    jobject segmentsArray = env->NewObject(arrayListClass, arrayListConstructor, NULL);


    int dimension = RpNumerics::getPhysics().domain().dim();

    int cells [dimension];

    env->GetIntArrayRegion(resolution, 0, dimension, cells);

    GridValues * gv = RpNumerics::getGridFactory().getGrid("bifurcationcurve");

    Inflection_Curve inflectionCurve;

    std::vector<RealVector> left_vrs;
//    TPCW & tpcw = (TPCW &) RpNumerics::getPhysics().getSubPhysics(0);
//
//    Flux2Comp2PhasesAdimensionalized * fluxFunction = (Flux2Comp2PhasesAdimensionalized *) & tpcw.fluxFunction();
//
//    Accum2Comp2PhasesAdimensionalized * accumulationFunction = (Accum2Comp2PhasesAdimensionalized *) & tpcw.accumulation();
//
//    cout << "Parametros de acumulacao" << accumulationFunction->accumulationParams().params() << endl;
//
//    cout << "Parametros de fluxo" << tpcw.fluxFunction().fluxParams().params() << endl;
//
//    Thermodynamics_SuperCO2_WaterAdimensionalized * thermo = fluxFunction->getThermo();
//
//    cout << "Parametros de thermo: " << thermo->U_typical() << " " << thermo->T_typical() << endl;

    inflectionCurve.curve(& RpNumerics::getPhysics().fluxFunction(), & RpNumerics::getPhysics().accumulation(), *gv, family, left_vrs);

    int tamanho = left_vrs.size();
    cout << "Tamanho do vetor de pontos: " << tamanho << endl;

    if (left_vrs.size() == 0)
        return NULL;

    cout << "Familia da inflexao: " << family << endl;


    for (int i = 0; i < left_vrs.size() / 2; i++) {



        RpNumerics::getPhysics().getSubPhysics(0).postProcess(left_vrs[2 * i]);
        RpNumerics::getPhysics().getSubPhysics(0).postProcess(left_vrs[2 * i + 1]);



        jdoubleArray eigenValRLeft = env->NewDoubleArray(dimension);
        jdoubleArray eigenValRRight = env->NewDoubleArray(dimension);


        double * leftCoords = (double *) left_vrs[2 * i];
        double * rightCoords = (double *) left_vrs[2 * i + 1];


        env->SetDoubleArrayRegion(eigenValRLeft, 0, dimension, leftCoords);
        env->SetDoubleArrayRegion(eigenValRRight, 0, dimension, rightCoords);


        //Construindo left e right points
        jobject realVectorLeftPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRLeft);
        jobject realVectorRightPoint = env->NewObject(realVectorClass, realVectorConstructorDoubleArray, eigenValRRight);


        jobject realSegment = env->NewObject(realSegmentClass, realSegmentConstructor, realVectorLeftPoint, realVectorRightPoint);
        env->CallObjectMethod(segmentsArray, arrayListAddMethod, realSegment);
    }




    jobject result = env->NewObject(inflectionCurveClass, inflectionCurveConstructor, segmentsArray);


    if (result == NULL)cout << "Eh nulo" << endl;
    // Limpando

    env->DeleteLocalRef(realSegmentClass);
    env->DeleteLocalRef(realVectorClass);
    env->DeleteLocalRef(arrayListClass);


    return result;


}
















