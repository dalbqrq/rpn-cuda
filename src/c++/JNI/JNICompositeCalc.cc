/*
 * IMPA - Fluid Dynamics Laboratory
 *
 * RPn Project
 *
 * @(#) JNIRarefactionOrbitCalc.cc
 */

/*
 * ---------------------------------------------------------------
 * Includes:
 */
#include "rpnumerics_CompositeCalc.h"
#include "RpNumerics.h"
#include "RealVector.h"
#include "JNIDefs.h"
#include "Stone.h"
#include "StoneAccumulation.h"
#include "RectBoundary.h"
#include <vector>
#include "Rarefaction.h"

#include "CompositeCurve.h"
#include "Shock.h"

using std::vector;

/*
 * ---------------------------------------------------------------
 * Definitions:
 */


JNIEXPORT jobject JNICALL Java_rpnumerics_CompositeCalc_nativeCalc(JNIEnv * env, jobject obj, jobject initialPoint, jint increase, jint familyIndex) {

    cout << "chamando JNI composite calc" << endl;

    unsigned int i;

    jclass classOrbitPoint = (env)->FindClass(ORBITPOINT_LOCATION);
    jclass classRarefactionOrbit = (env)->FindClass(COMPOSITECURVE_LOCATION);

    jmethodID rarefactionOrbitConstructor = (env)->GetMethodID(classRarefactionOrbit, "<init>", "([Lrpnumerics/OrbitPoint;II)V");
    jmethodID orbitPointConstructor = (env)->GetMethodID(classOrbitPoint, "<init>", "([D)V");
    jmethodID toDoubleMethodID = (env)->GetMethodID(classOrbitPoint, "toDouble", "()[D");

    //Input processing
    jdoubleArray inputPhasePointArray = (jdoubleArray) (env)->CallObjectMethod(initialPoint, toDoubleMethodID);

    double input [env->GetArrayLength(inputPhasePointArray)];

    env->GetDoubleArrayRegion(inputPhasePointArray, 0, env->GetArrayLength(inputPhasePointArray), input);

    RealVector realVectorInput(env->GetArrayLength(inputPhasePointArray));


    for (i = 0; i < (unsigned int) realVectorInput.size(); i++) {

        realVectorInput.component(i) = input[i];

    }

    env->DeleteLocalRef(inputPhasePointArray);


    // Storage space for the segments:




    std::vector<RealVector> rarefactionCurve;
    std::vector<RealVector> compositeCurve;


    if (RpNumerics::getPhysics().ID().compare("Stone") == 0) {

        cout << "Chamando com stone" << endl;

        int dimension = 2;

        FluxFunction * stoneflux = (FluxFunction *) RpNumerics::getPhysics().fluxFunction().clone();

        AccumulationFunction * stoneaccum = (AccumulationFunction *) RpNumerics::getPhysics().accumulation().clone();

        Boundary * tempBoundary = RpNumerics::getPhysics().boundary().clone();


        //        const RealVector & pmin = RpNumerics::getPhysics().boundary().minimums();
        //        const RealVector & pmax = RpNumerics::getPhysics().boundary().maximums();

        //        RealVector pmin(2);
        //        RealVector pmax(2);
        //
        //
        //        pmin.component(0) = 0.0;
        //        pmin.component(1) = 0.0;
        //
        //
        //        pmax.component(0) = 1.0;
        //        pmax.component(1) = 1.0;
        //
        //
        //
        //        cout << pmin << endl;
        //        cout << pmax << endl;
        //
        //


        cout << "Increase: " << increase << endl;

//        double deltaxi = 1e-3;
        double deltaxi = 1e-2;

        //Compute rarefaction



        cout << "Increase da rarefacao: " << increase << endl;


        Rarefaction::curve(realVectorInput,
                RAREFACTION_INITIALIZE_YES,
                0,
                familyIndex,
                increase,
                CHECK_RAREFACTION_MONOTONY_TRUE,
                deltaxi,
                stoneflux, stoneaccum,
                RAREFACTION_GENERAL_ACCUMULATION,
                tempBoundary,
                rarefactionCurve);


        increase = WAVE_FORWARD;

        cout << "Rarefaction curve" << rarefactionCurve.size() << endl;

        CompositeCurve::curve(rarefactionCurve, COMPOSITE_FROM_NORMAL_RAREFACTION, familyIndex, increase, stoneflux, stoneaccum, tempBoundary, compositeCurve);


    }

    //Orbit members creation

    cout << "Tamanho da curva: " << compositeCurve.size() << endl;

    jobjectArray orbitPointArray = (jobjectArray) (env)->NewObjectArray(compositeCurve.size(), classOrbitPoint, NULL);

    for (i = 0; i < compositeCurve.size(); i++) {

        RealVector tempVector = compositeCurve.at(i);


        //        cout << "Ponto " << i << " " << tempVector<<" dimensao:"<<tempVector.size() << endl;



        RealVector newVector(3);

        newVector.component(0) = tempVector(0);
        newVector.component(1) = tempVector(1);
        newVector.component(2) = 0;


        double * dataCoords = newVector;



        jdoubleArray jTempArray = (env)->NewDoubleArray(newVector.size());

        (env)->SetDoubleArrayRegion(jTempArray, 0, newVector.size(), dataCoords);

        jobject orbitPoint = (env)->NewObject(classOrbitPoint, orbitPointConstructor, jTempArray);

        (env)->SetObjectArrayElement(orbitPointArray, i, orbitPoint);

        env->DeleteLocalRef(jTempArray);

        env->DeleteLocalRef(orbitPoint);

    }

    //Building the orbit

    jobject rarefactionOrbit = (env)->NewObject(classRarefactionOrbit, rarefactionOrbitConstructor, orbitPointArray, increase, familyIndex);


    //Cleaning up

    compositeCurve.clear();

    env->DeleteLocalRef(orbitPointArray);
    env->DeleteLocalRef(classOrbitPoint);
    env->DeleteLocalRef(classRarefactionOrbit);

    return rarefactionOrbit;



}
